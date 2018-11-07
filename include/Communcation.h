#ifndef COMMUNCATION_H
#define COMMUNCATION_H

#include <thread>
#include <pthread.h>
#include <UDPSocket.h>
#include <map>
#include <string>
#include <utility>
#include "Message.h"
#include <set>
#include <mutex>
#include <thread>
#include <queue>
#include <algorithm>
using namespace std;

class Communcation
{
    public:
        Communcation(int maxNumberOfListeners, int numberOfSenders, \
                    int maxPacketSize, int _maxSegmentCount, int listenerPort, int maxRetriesPerPacket);
        ~Communcation();

        struct Request{
            string username;
            string data;
            string type;
            string response;
            bool status;
            bool operator==(const Request& rhs) const;
        };

        struct UserInfo{
            string ip;
            int port;
            bool online;
        };

        void listen(int threadNumber);
        void send(int threadNumber);
        bool pushIntoAssembler(Message input);
        void startListeners();
        void startSenders();
        void pushBigMessage(Message bigMessage);
        bool popBigMessage(Message& bigMessage);
        bool popFromAssembler(Message& bigMessage);
        bool sendMessage(string username, string type, string& data,  string& response);
        Message assembleBigMessage(Message lastMiniMessage);
        bool messageComplete(Message input);
        bool updateUserInfoFromDirectory();
        bool popRequest(Request& req);
        bool resolveUserInfo(UserInfo& info, Request& req);
        bool pushRequest(Request req);
        bool popResponse(Request& req);
        bool pushResponse(Request fullResponse);
        bool checkForAck(Message msg,int waitTime);
        int getAndIncrementGlobalId();
        bool sendMiniMessages(vector<Message> miniMessages,UserInfo userInfo, Request req, int threadNumber);

        mutex assemblerLock;
        mutex bigMessageQueueLock;
        mutex RequestQueueLock;
        mutex responseVectorLock;
        mutex globalIdLock;

        int maxPacketLength;
        int numOfListeners;
        int numOfSenders;
        int listenerPort;
        int globalId;
        int maxRetriesPerPacket;
        int maxSegmentCount;

        vector<thread> listener;
        vector<thread> sender;

        map<string, UserInfo > usernameCache;
        vector<string> messageIdHistory;
        map<string, set<Message, Message::MessageCompare> > assemblerStructure;
        queue<Message> bigMessageQueue;
        queue<Request> RequestQueue;
        vector<Request> responseVector;

    private:

        vector<UDPSocket> client;
        UDPSocket server;
};

#endif // COMMUNCATION_H
