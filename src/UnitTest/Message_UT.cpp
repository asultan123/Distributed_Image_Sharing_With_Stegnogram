#include<cstdlib>
#include<cstdio>
#include<iostream>
#include<string>
#include<algorithm>
#include<stdlib.h>
#include "../Message.h"
#include "Communcation.h"

using namespace std;

int main() {

    vector<Message> temp = \
            Message::randomMessageBrokenDownToMiniMessages(0,5000,"192.168.1.1","7amada",100000,8000,9999);

    Message msg = temp[0];
    Message msg1 = temp[0];

	msg.unmarshal(msg.marshal(4,8000));

    cout<<"Testing Marshalling and unMarshalling"<<endl;

    if(msg != msg1){
        cout<<"marshall/unmarshall failed"<<endl;
        throw;
    }else{
        cout<<"pass!"<<endl;
    }

//    cout<<"Testing copy constructor"<<endl;
//
//    if(msg3 != msg2){
//        cout<<"copy constructor failed"<<endl;
//        throw;
//    }else{
//        cout<<"pass!"<<endl;
//    }
//
//    cout<<"Testing message partitioning and assimilation"<<endl;
//
//    string testData = "kkkkkkkkkkkkkkkkkkkkkkkkkkkkkkk";
//
//	vector<Message> &msgs = *(new std::vector<Message>);
//	msgs = Message::createMessages(3, "192.168.1.2", 7777, "Type2", testData,30);
//
//	Message bigMessage = Message::assembleBigMessage(msgs);
//
//    if(testData.length() != bigMessage.getData().length()){
//        cout<<"Mini-message creation and assimilation failed!"<<endl;
//        throw "";
//    }else{
//        cout<<"pass!"<<endl;
//    }
//
//    cout<<"testing message set insertion and deletion"<<endl;
//
//    testData = "";
//
//    for(int i = 0; i<200; i++){
//        testData += 'a' + rand()%20;
//    }
//
//    msgs = Message::createMessages(3, "192.168.1.2", 7777, "Type2", testData,30);
//
//    random_shuffle(msgs.begin(),msgs.end());
//
//    set<Message, Message::MessageCompare> messageSet;
//
//    for(auto it : msgs){
//        messageSet.insert(it);
//        if(rand()%2>0){
//            cout<<"Randomly inserted duplicate!"<<endl;
//            messageSet.insert(it);
//        }
//    }
//
//    msgs.clear();
//
//    bigMessage = Message::assembleBigMessage(messageSet);
//
//    if(testData == bigMessage.getData()){
//        cout<<"Mini-message creation and assimilation failed!"<<endl;
//        throw "";
//    }else{
//        cout<<"pass!"<<endl;
//    }
//

	return 0;
}
