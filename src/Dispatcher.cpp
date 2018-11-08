#include "Dispatcher.h"

Dispatcher::Dispatcher(Communcation* _comm,Image* _img)
{
    comm = _comm;
    img = _img;
}

Dispatcher::~Dispatcher()
{
    //dtor
}

void Dispatcher::start(){
    thr = new thread(&Dispatcher::listenForExternalRequests, this);
}

void Dispatcher::listenForExternalRequests(){

    while(true){
        Message msg;

        while(!comm->popBigMessage(msg)){
            this_thread::sleep_for(chrono::milliseconds(100));
        }

        if("recvImageRequest"){
            async(&Image::recvImageRequest, img, msg.getData());
        }
        else if("recvImageReply"){
            async(&Image::recvImageReply, img, msg.getData());
        }
        else if("recvListImages"){
            async(&Image::recvListImagesRequest, img, msg.getData());
        }
        else if("recvListImages"){
            async(&Image::recvListImagesReply, img, msg.getData());
        }
        else if("recvRequestIncrease"){
            async(&Image::recvRequestIncreaseCount, img, msg.getData());
        }
        else if("recvReplyIncrease"){
            async(&Image::recvReplyIncreaseCount, img, msg.getData());
        }
        else{
            cout<<"Dispatcher recieved a message of unknown type"<<endl;
        }


    }
}




