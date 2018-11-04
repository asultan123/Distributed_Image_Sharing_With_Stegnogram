#include "Communcation.h"

Communcation::Communcation(int _numberOfListeners, int _numberOfSenders,int _maxPacketLength, \
                            int _listenerPort, int _maxRetriesPerPacket)
{
    maxPacketLength = _maxPacketLength;
    numOfListeners = _numberOfListeners;
    numOfSenders = _numberOfSenders;
    listenerPort = _listenerPort;
    globalId = 0;
    //ctor
}

Communcation::~Communcation()
{
    //dtor
}

bool Communcation::sendMessage(string username,string type, string data, string& response){

    Request req;
    req.username = username;
    req.type = type;
    req.data = data;

    pushRequest(req);

    while(!popResponse(req));

    response = req.response;

    return req.status;
}

bool Communcation::popBigMessage(Message& bigMessage){
    bigMessageQueueLock.lock();
    bool success = false;
    if(!bigMessageQueue.empty()){
        bigMessage = bigMessageQueue.front();
        bigMessageQueue.pop();
        success = true;
    }
    bigMessageQueueLock.unlock();
    return success;
}

int Communcation::getAndIncrementGlobalId(){
    int res;
    globalIdLock.lock();
    res = globalId;
    globalId++;
    globalIdLock.unlock();
    return res;
}

bool Communcation::pushIntoAssembler(Message input){

    assemblerLock.lock();

    if(assemblerStructure.find(input.getMessageUID()) != assemblerStructure.end()){
        // message exists
        assemblerStructure[input.getMessageUID()].insert(input);
    }
    else{
        // message does not exist
        auto newSet = *(new set<Message, Message::MessageCompare>);
        newSet.insert(input);
        assemblerStructure[input.getMessageUID()] = newSet;
    }

    assemblerLock.unlock();

    return true;
}

bool Communcation::messageComplete(Message input){
    assemblerLock.lock();
    auto it = assemblerStructure.find(input.getMessageUID());
    if(it!=assemblerStructure.end()){
        assemblerLock.unlock();
        return it->second.size() == (unsigned int)input.getSegmentCount();
    }
    else{
        assemblerLock.unlock();
        return false;
    }
}

void Communcation::startListeners(){
    server.initializeServer(listenerPort);
    for(int i = 0; i< numOfListeners; i++){

        cout<<"Started thread: "<<i<<" listening on port: "\
            <<listenerPort<<" for packets of size: "<<maxPacketLength<<endl;

        listener.push_back(thread(&Communcation::listen, this ,i));

        listener[i].detach();
    }
}

void Communcation::startSenders(){
    for(int i = 0; i< numOfSenders; i++){
        cout<<"Started thread: "<<i<<" sender"<<endl;
        UDPSocket newClient;
        client.push_back(newClient);
        sender.push_back(thread(&Communcation::send, this ,i));
        sender[i].detach();
    }
}

bool Communcation::popRequest(Request& req){
    RequestQueueLock.lock();
    bool success = false;
    if(!RequestQueue.empty()){
        req = RequestQueue.front();
        RequestQueue.pop();
        success = true;
    }
    RequestQueueLock.unlock();
    return success;
}

bool Communcation::resolveUserInfo(UserInfo& info,Request& req){
    auto it = usernameCache.find(req.username);
    info = it->second;
    return it!=usernameCache.end();
}

bool Communcation::pushRequest(Request req){
    RequestQueueLock.lock();
    RequestQueue.push(req);
    RequestQueueLock.unlock();
    return true;
}

bool Communcation::Request::operator==(const Request& rhs) const{
    bool result;
    result = response == rhs.response;
    result = result && username == rhs.username;
    result = result && data == rhs.data;
    result = result && type == rhs.type;
    return result;
}

bool Communcation::pushResponse(Request fullResponse){
    responseVectorLock.lock();
    if(find(responseVector.begin(),responseVector.end(),fullResponse)==responseVector.end()){
        responseVector.push_back(fullResponse);
    }
    responseVectorLock.unlock();
    return true;
}

bool Communcation::popResponse(Request& responseToFind){

    bool success = false;
    responseVectorLock.lock();

    if(!responseVector.empty()){

        auto reqIt = find_if(responseVector.begin(), responseVector.end(), \
                            [responseToFind](const Request& req){
                                bool temp = responseToFind.data == req.data;
                                temp = temp && responseToFind.type == req.type;
                                temp = temp && responseToFind.username == req.username;
                                return temp;
                            });

        if(reqIt == responseVector.end()){
            success = false;
        }
        else{
            responseVector.erase(reqIt);
            success = true;
        }
    }

    responseVectorLock.unlock();
    return success;
}

bool Communcation::checkForAck(Message msg, int waitTime){
    //WAIT FOR ACK RESPONSE, TIME OUT IF NOT RECIEVED
    return true;
}

bool Communcation::sendMiniMessages(vector<Message> miniMessages, UserInfo userInfo, \
                                    Request newReq, int threadNumber)
{
    client[threadNumber].initializeClient((char*)userInfo.ip.c_str(),userInfo.port);

    bool messageDelivered = true;

    for(auto msg : miniMessages){
        int trials = 0;
        do{
            client[threadNumber].writeToSocket((char*)msg.marshal().c_str(),maxPacketLength);
            trials++;
        }while(!checkForAck(msg, 0) && trials < maxRetriesPerPacket);

        if(trials == maxRetriesPerPacket){
            messageDelivered = false;
            break;
        }
    }

    return messageDelivered;
}

void Communcation::send(int threadNumber){
    while(true){
        Request newReq;
        if(popRequest(newReq)){
            cout<<"Sender thread no: "<<threadNumber<<" recieved request"<<endl;
            UserInfo userInfo;

            if(!resolveUserInfo(userInfo,newReq)){
                cout<<"Invalid username info recieved by thread "<<threadNumber<<" requested username: "\
                    <<newReq.username<<endl;
                newReq.response="Request Send Failed, User not found";
                newReq.status = false;
                pushResponse(newReq);
                continue;
            }

            if(!userInfo.online){
                cout<<"User "<<newReq.username<<" not online"<<endl;
                newReq.response = "Request Send Failed, User not online";
                newReq.status = false;
                pushResponse(newReq);
                continue;
            }

            vector<Message> miniMessages =
                Message::createMessages(getAndIncrementGlobalId(),userInfo.ip,\
                                        userInfo.port,newReq.type,\
                                        newReq.data,maxPacketLength);

            bool messageDelivered = sendMiniMessages(miniMessages, userInfo,  newReq, threadNumber);

            if(!messageDelivered){
                newReq.response = "Request send failed, Network error";
                newReq.status = false;
            }else{
                newReq.response = "Request delivered";
                newReq.status = true;
            }
            pushResponse(newReq);
        }
    }
}

bool Communcation::updateUserInfoFromDirectory(){
    // MUST BE CHANGED!!
    UserInfo temp;
    temp.ip = "172.16.115.196";
    temp.online = true;
    temp.port = 5000;
    usernameCache["SELF"] = temp;
    return true;
}

Message Communcation::assembleBigMessage(Message lastMiniMessage){
    assemblerLock.lock();
    auto it = assemblerStructure.find(lastMiniMessage.getMessageUID());
    Message bigMessage = Message::assembleBigMessage(it->second);
    assemblerStructure.erase(it);
    assemblerLock.unlock();
    return bigMessage;
}

void Communcation::pushBigMessage(Message bigMessage){
    bigMessageQueueLock.lock();
    bigMessageQueue.push(bigMessage);
    bigMessageQueueLock.unlock();
}

void Communcation::listen(int threadNumber){
    while(true){

        char* message = (char*) malloc(maxPacketLength*sizeof(char));
        server.readFromSocketWithBlock(message, maxPacketLength);
        //cout<<"Listener number "<<threadNumber<<" recieved a message of size :"<<messageSize<<endl;
        Message newMessage;
        newMessage.unmarshal(string(message));
        pushIntoAssembler(newMessage);
        Message bigMessage;

        if(messageComplete(newMessage)){
            bigMessage = assembleBigMessage(newMessage);
            pushBigMessage(bigMessage);
        }
    }
}
