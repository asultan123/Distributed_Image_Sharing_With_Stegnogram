#include "UDPSocket.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <iostream>
#include <cstdio>
#include <cstring>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>


void UDPSocket::makeLocalSA(struct sockaddr_in *sa)
{
	sa->sin_family  =  AF_INET;
	sa->sin_port = htons(0);
	sa-> sin_addr.s_addr = htonl(INADDR_ANY);
}

void UDPSocket::makeDestSA(struct sockaddr_in* sa, char* hostip, int port)
{
	sa->sin_family  =  AF_INET;
	if(inet_pton(AF_INET,hostip,&sa->sin_addr)<1){
		perror("invalid destination ip\n");
	}
	sa->sin_port = htons(port);
}

void UDPSocket::makeReceiverSA(struct sockaddr_in *sa, int port)
{
	sa->sin_family  =  AF_INET;
	sa->sin_port = htons(port);
	sa-> sin_addr.s_addr = htonl(INADDR_ANY);
}

bool UDPSocket::initializeServer (char * _myAddr, int _myPort)
{
    mtx.lock();

	if((sock = socket(AF_INET, SOCK_DGRAM, 0))<0) {
		perror("socket  failed");
		return false;
	}

	makeReceiverSA(&myAddr, _myPort);
	if(bind(sock, (struct sockaddr*)&myAddr, sizeof(struct sockaddr_in))!= 0){
		perror("Bind  failed\n");
		close(sock);
		return false;
	}

	this->myAddress = _myAddr;
	this->myPort = _myPort;

	mtx.unlock();

	return true;
}

bool UDPSocket::initializeClient(char* _peerAddr, int _peerPort)
{

    mtx.lock();

	if((sock = socket(AF_INET, SOCK_DGRAM, 0))<0) {
		perror("socket  failed");
		return false;
	}

	makeLocalSA(&myAddr);

	if(bind(sock, (struct sockaddr*)&myAddr, sizeof(struct sockaddr_in))!= 0){
		perror("Bind  failed\n");
		close(sock);
		return false;
	}

	makeDestSA(&peerAddr, _peerAddr, _peerPort);

	this->peerAddress = _peerAddr;
	this->peerPort = _peerPort;

    mtx.unlock();

	return true;
}

int UDPSocket::writeToSocket (char * buffer, int maxBytes ){

    mtx.lock();

	int n;
	if((n = sendto(sock, buffer, maxBytes, 0, (struct sockaddr*)&peerAddr,\
		sizeof(struct sockaddr_in))) < 0)\
		perror("Send message failed\n");

    mtx.unlock();

	return n;
}

int UDPSocket::readFromSocketWithBlock (char * buffer, int maxBytes )	{

    mtx.lock();

	struct sockaddr_in aSocketAddress;
	int aLength, n;

	aLength = sizeof(struct sockaddr_in);
	aSocketAddress.sin_family = AF_INET; /* note that this is needed */
	if((n = recvfrom(sock, buffer,  maxBytes, 0, (struct sockaddr*)&aSocketAddress, (socklen_t*)&aLength))<0){
		perror("Receive 1") ;
		close(sock);
	}

	peerAddr = aSocketAddress;

	mtx.unlock();

	return n;
}

UDPSocket::UDPSocket(){

}

UDPSocket::~UDPSocket(){

}

int UDPSocket::getMyPort (){
	return this->myPort;
}
int UDPSocket::getPeerPort (){
	return this->peerPort;
}
int UDPSocket::getSocketHandler(){
	return this->sock;
}

int UDPSocket::readSocketWithNoBlock (char * buffer, int maxBytes ){

    mtx.lock();

	struct timeval read_timeout;
	read_timeout.tv_sec = 0;
	read_timeout.tv_usec = 10;

	setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &read_timeout, sizeof read_timeout);

	struct sockaddr_in aSocketAddress;
	int aLength, n;

	aLength = sizeof(struct sockaddr_in);
	aSocketAddress.sin_family = AF_INET; /* note that this is needed */
	if((n = recvfrom(sock, buffer,  maxBytes, 0, (struct sockaddr*)&aSocketAddress, (socklen_t*)&aLength))<0){
		perror("Receive 1") ;
		close(sock);
	}

	peerAddr = aSocketAddress;

	setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, 0, sizeof read_timeout);

	mtx.unlock();

	return n;
}

int UDPSocket::readSocketWithTimeout (char * buffer, int maxBytes, int timeoutSec, \
	int timeoutMilli){

	mtx.lock();

	struct timeval read_timeout;
	read_timeout.tv_sec = timeoutSec;
	read_timeout.tv_usec = timeoutMilli;

	setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &read_timeout, sizeof read_timeout);

	struct sockaddr_in aSocketAddress;
	int aLength, n;

	aLength = sizeof(struct sockaddr_in);
	aSocketAddress.sin_family = AF_INET; /* note that this is needed */
	if((n = recvfrom(sock, buffer,  maxBytes, 0, (struct sockaddr*)&aSocketAddress, (socklen_t*)&aLength))<0){
		perror("Receive 1") ;
		close(sock);
	}

	peerAddr = aSocketAddress;

	setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, 0, sizeof read_timeout);

	mtx.unlock();

	return n;
}


