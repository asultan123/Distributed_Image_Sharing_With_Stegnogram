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

void testAssemblerAndDispatcherBuffer(){

    const int collectionSize = 100;
    const int numOfMessages = 100;
    Communcation comm(3,3,8000,5000,5);

    vector<vector<Message>> randomMessageCollections;

    //create several mini message sets with random data

    for(int i = 0; i<collectionSize; i++){

        vector<Message> msgSet = Message::randomMessagesWith(i,1,"2",to_string(i),numOfMessages);
        random_shuffle(msgSet.begin(),msgSet.end());
        randomMessageCollections.push_back(msgSet);
    }

    //push them into buffer with duplicates randomly occuring

    for(auto msgSet : randomMessageCollections){
        for(auto msg : msgSet){
            comm.pushIntoAssembler(msg);
            if(rand()%2>0){
            comm.pushIntoAssembler(msg);
            }
        }
    }

    //start test

    for(auto msgSet : randomMessageCollections){
        for(auto msg : msgSet){
            vector<Message> temp = msgSet;
            //check if message is ready to pack (they should all be ready)
            if(comm.messageComplete(msg)){
                cout << "message with id: "<<msg.getMessageId()<<" complete, now packing..."<<endl;
                int currentAssemblerSize = comm.assemblerStructure.size();
                Message bigMessage = comm.assembleBigMessage(msg);
                sort(temp.begin(), temp.end(), messageCompare);
                Message originalBigMessage = Message::assembleBigMessage(temp);

                if(bigMessage == originalBigMessage && \
                    comm.assemblerStructure.size() == (unsigned int)currentAssemblerSize-1)
                {
                    cout << "message with id: "<<msg.getMessageId()<<" successfully reassembled, message valid"<<endl;
                    currentAssemblerSize = comm.assemblerStructure.size();
                    cout << "pushing message with id:"<<msg.getMessageId()<<" into dispatcherBuffer"<<endl;
                    comm.pushBigMessage(bigMessage);
                    break;
                }
                else{
                    cout << "message with id: "<<msg.getMessageId()<<" mismatch"<<endl;
                    throw"";
                }
            }
            else{
                cout << "message with id: "<<msg.getMessageId()<<" not complete, error..."<<endl;
                throw "";
            }
        }
    }

    Message frontMessage;
    for(int i = 0; i<100; i++){
        if(comm.popBigMessage(frontMessage)){
            assert(frontMessage.getMessageId() == i);
            cout<<"message with id: "<<i<<" successully popped from buffer"<<endl;
        }
    }
}

void testReqQueue();
void testResponseVector();
void testSender(){
    //Test Sending 1 large message (must be segmented) and seeing if its recieved by a listener, verify data
    //from bigMessageQueu with data from reqResponseVector
    //Test Sending 1 message with invalid user (one other than cached dummy user) --> modify resolveUsername fn
    //Test Sending 1 message with valid user but offline --> modify resolveUsername fn
    //Test Sending 1 message with random packet less (80% probability of drop) --> modify checkForAck fn
}

void testListener(){
    Communcation comm(20,3,8000,5000,5);
    comm.startListeners();

    const int collectionSize = 100;
    const int numOfMessages = 10;

    vector<vector<Message>> randomMessageCollections;

    //create several mini message sets with random data

    for(int i = 0; i<collectionSize; i++){

        vector<Message> msgSet = Message::randomMessagesWith(i,1,"2",to_string(i),numOfMessages);
        random_shuffle(msgSet.begin(),msgSet.end());
        randomMessageCollections.push_back(msgSet);
    }


    UDPSocket client;

    client.initializeClient((char*)string("172.16.115.196").c_str(),5000);
    int i = 0;
    for(auto msgSet : randomMessageCollections){
        for(auto msg : msgSet){
            client.writeToSocket((char*)msg.marshal().c_str(),8000);
            this_thread::sleep_for(chrono::milliseconds(10));
        }
        cout<<"Sent Message no: "<<i++<<endl;
    }

    for(auto msgSet : randomMessageCollections){
        sort(msgSet.begin(),msgSet.end(),messageCompare);
        Message originalBigMessage = Message::assembleBigMessage(msgSet);
        Message sentBigMessage;
        comm.popBigMessage(sentBigMessage);
        assert(originalBigMessage==sentBigMessage);
        cout<<"Message of id: "<<originalBigMessage.getMessageId()<<" delivered successfully"<<endl;
    }

}

int main(){
    srand(time(NULL));
    testAssemblerAndDispatcherBuffer();
    //testListener();


    return 0;
}
