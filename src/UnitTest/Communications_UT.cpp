#include<iostream>
#include<Communcation.h>
#include<vector>
#include<Message.h>
#include<algorithm>
#include<assert.h>
#include<UDPSocket.h>
#include<chrono>

using namespace std;

bool messageCompare(const Message& lhs, const Message& rhs){
    return lhs.getSegmentOrder()<rhs.getSegmentOrder();
}

//void testAssemblerAndDispatcherBuffer(){
//
//    const int collectionSize = 100;
//    const int numOfMessages = 100;
//    Communcation comm(3,3,8000,5000,5);
//
//    vector<vector<Message>> randomMessageCollections;
//
//    //create several mini message sets with random data
//
//    for(int i = 0; i<collectionSize; i++){
//
//        vector<Message> msgSet = Message::randomMessagesWith(i,1,"2",to_string(i),numOfMessages);
//        random_shuffle(msgSet.begin(),msgSet.end());
//        randomMessageCollections.push_back(msgSet);
//    }
//
//    //push them into buffer with duplicates randomly occuring
//
//    for(auto msgSet : randomMessageCollections){
//        for(auto msg : msgSet){
//            comm.pushIntoAssembler(msg);
//            if(rand()%2>0){
//            comm.pushIntoAssembler(msg);
//            }
//        }
//    }
//
//    //start test
//
//    for(auto msgSet : randomMessageCollections){
//        for(auto msg : msgSet){
//            vector<Message> temp = msgSet;
//            //check if message is ready to pack (they should all be ready)
//            if(comm.messageComplete(msg)){
//                cout << "message with id: "<<msg.getMessageId()<<" complete, now packing..."<<endl;
//                int currentAssemblerSize = comm.assemblerStructure.size();
//                Message bigMessage = comm.assembleBigMessage(msg);
//                sort(temp.begin(), temp.end(), messageCompare);
//                Message originalBigMessage = Message::assembleBigMessage(temp);
//
//                if(bigMessage == originalBigMessage && \
//                    comm.assemblerStructure.size() == (unsigned int)currentAssemblerSize-1)
//                {
//                    cout << "message with id: "<<msg.getMessageId()<<" successfully reassembled, message valid"<<endl;
//                    currentAssemblerSize = comm.assemblerStructure.size();
//                    cout << "pushing message with id:"<<msg.getMessageId()<<" into dispatcherBuffer"<<endl;
//                    comm.pushBigMessage(bigMessage);
//                    break;
//                }
//                else{
//                    cout << "message with id: "<<msg.getMessageId()<<" mismatch"<<endl;
//                    throw"";
//                }
//            }
//            else{
//                cout << "message with id: "<<msg.getMessageId()<<" not complete, error..."<<endl;
//                throw "";
//            }
//        }
//    }
//
//    Message frontMessage;
//    for(int i = 0; i<100; i++){
//        if(comm.popBigMessage(frontMessage)){
//            assert(frontMessage.getMessageId() == i);
//            cout<<"message with id: "<<i<<" successully popped from buffer"<<endl;
//        }
//    }
//}
string randomLowerCaseString(int length){
    string temp = "";
    for(int i = 0; i<length; i++){
        temp+= char(rand()%26 + 'a');
    }
    return temp;
}

void testReqQueue();
void testResponseVector();
void testSender(){
    //Test Sending 1 large message (must be segmented) and seeing if its recieved by a listener, verify data
    //from bigMessageQueu with data from reqResponseVector
    //Test Sending 1 message with invalid user (one other than cached dummy user) --> modify resolveUsername fn
    //Test Sending 1 message with valid user but offline --> modify resolveUsername fn
    //Test Sending 1 message with random packet less (80% probability of drop) --> modify checkForAck fn
    cout<<"test1, generic send and listen"<<endl;
    Communcation comm(20, 1,8000,9999, 6000, 5);

    comm.startListeners();
    comm.startSenders();
    comm.updateUserInfoFromDirectory();

    string username = "SELF";
    string data = randomLowerCaseString(25*1000*1000);

    string type = "7amada";

    string response = "DEFAULT TEXT";

    if(comm.sendMessage(username,type,data,response)){
        cout<<"Message sent successfully"<<endl;
    }
    else{
        cout<<"Message not sent"<<endl;
    }

    cout<<response<<endl;

    Message bigMessage;

    while(!comm.popBigMessage(bigMessage));

    cout<<"data = bigMessage.data: "<<endl;

    assert(data == bigMessage.getData());









}

void testListener(){

    string ip = "172.16.115.196";

    Communcation comm(20, 1,8000,9999, 5000, 5);
    comm.startListeners();

    const int collectionSize = 3;

    vector<Message> randomMessages;
    vector<Message> equivelentBigMessages;

    //create randomBrokenDownMessages
    for(int i = 0; i<collectionSize; i++){
        vector<Message> temp = \
            Message::randomMessageBrokenDownToMiniMessages(i,5000,ip,"7amada",25*1000*1000,8000,9999);
            equivelentBigMessages.push_back(Message::assembleBigMessage(temp));
        for(auto msg : temp){
            randomMessages.push_back(msg);
        }
    }

    random_shuffle(randomMessages.begin(), randomMessages.end());

    UDPSocket client;

    client.initializeClient((char*)string("172.16.115.196").c_str(),5000);
    int i = 0;
    for(Message msg : randomMessages){
        string marshaledMessage = msg.marshal(4,8000);
        client.writeToSocket((char*)marshaledMessage.c_str(),8000);
        this_thread::sleep_for(chrono::milliseconds(1));
        cout<<"Sent Message no: "<<i++<<endl;
    }

    this_thread::sleep_for(chrono::milliseconds(1000));

    vector<Message> bigMessages;
    Message temp;
    while(comm.popBigMessage(temp)){
        bigMessages.push_back(temp);
    }

    assert(bigMessages.size() == equivelentBigMessages.size());
    cout<<"Nice start, sent messages was equal to recieved messages"<<endl;

    for(Message msg : equivelentBigMessages){
        auto it = find(bigMessages.begin(),bigMessages.end(),msg);
        assert(it != bigMessages.end());
        cout<<"Message with id: "<<it->getMessageId()<<" was successfully delivered"<<endl;
    }



}

int main(){
    srand(time(NULL));

    testSender();

    //testListener();
    return 0;
}
