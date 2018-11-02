#include "Communcation.h"

Communcation::Communcation()
{
    //ctor
}

Communcation::~Communcation()
{
    //dtor
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

bool Communcation::pushIntoAssembler(Message input){

    assemblerLock.lock();

    if(assemblerStructure.find(input.getMessageUID()) != assemblerStructure.end()){
        // message exists
        assemblerStructure[input.getMessageUID()].insert(input);
    }
    else{
        // message does not exist
        set<Message, Message::MessageCompare>& newSet = *(new set<Message, Message::MessageCompare>);
        newSet.insert(input);
        assemblerStructure[input.getMessageUID()] = newSet;
    }

    assemblerLock.unlock();

    return true;
}

bool Communcation::messageComplete(Message input){
    std::map<string, set<Message, Message::MessageCompare> >::iterator it = assemblerStructure.find(input.getMessageUID());
    if(it!=assemblerStructure.end()){
        return it->second.size() == (unsigned int)input.getSegmentCount();
    }
    else{
        return false;
    }
}

void Communcation::startListeners(int numOfListeners, int listenerPort){
    server.initializeServer((char*)string("").c_str(),listenerPort);
    for(int i = 0; i< numOfListeners; i++){
        listener[i] = thread(&Communcation::listen, this ,i);
    }
}

void Communcation::pushBigMessage(Message bigMessage){
    bigMessageQueueLock.lock();
    bigMessageQueue.push(bigMessage);
    bigMessageQueueLock.unlock();
}

void Communcation::listen(int threadNumber){
    while(true){
        char* message = (char*) malloc(MAX_PACKET_LENGTH*sizeof(char));
        int messageSize = server.readFromSocketWithBlock(message, MAX_PACKET_LENGTH);
        cout<<"Listener number "<<threadNumber<<" recieved a message of size :"<<messageSize<<endl;
        Message newMessage;
        newMessage.unmarshal(string(message));
        pushIntoAssembler(newMessage);
        if(messageComplete(newMessage)){
        }
    }
}
