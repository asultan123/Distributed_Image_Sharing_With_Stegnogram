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

using namespace std;

class Communcation
{
    public:
        Communcation();
        ~Communcation();

        void listen(int threadNumber);
        bool pushIntoAssembler(Message input);
        void startListeners(int numOfListeners, int listenerPort);
        void pushBigMessage(Message bigMessage);
        bool popBigMessage(Message& bigMessage);
        bool popFromAssembler(Message& bigMessage);

    protected:



        mutex assemblerLock;
        mutex bigMessageQueueLock;

        const int MAX_PACKET_LENGTH = 8000;
        const int NUMBER_OF_LISTENERS = 1;
        thread listener[1];

        map<string, pair<string, int> > usernameCache;
        vector<string> messageIdHistory;
        map<string, set<Message, Message::MessageCompare> > assemblerStructure;
        queue<Message> bigMessageQueue;

        bool messageComplete(Message input);


    private:

        UDPSocket client;
        UDPSocket server;
};

#endif // COMMUNCATION_H
