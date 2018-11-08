#include<thread>
#include<Message.h>
#include<Communcation.h>
#include<Image.h>
#include<chrono>
#include<future>
#include<Image.h>

#ifndef DISPATCHER_H
#define DISPATCHER_H

using namespace std;

class Dispatcher
{
    public:

        Dispatcher(Communcation* _comm,Image* img);
        ~Dispatcher();

        thread* thr;
        Communcation* comm;
        Image* img;

        void listenForExternalRequests();
        void start();

    protected:
    private:
};

#endif // DISPATCHER_H
