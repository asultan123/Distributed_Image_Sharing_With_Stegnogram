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

	return true;
}

bool UDPSocket::initializeClient(char* _peerAddr, int _peerPort)
{
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

	return true;
}

int UDPSocket::writeToSocket (char * buffer, int maxBytes ){   
	int n;
	if((n = sendto(sock, buffer, maxBytes, 0, (struct sockaddr*)&peerAddr,\
		sizeof(struct sockaddr_in))) < 0)\
		perror("Send message failed\n");

	return n;
}

// int writeToSocketAndWait (char * buffer, int maxBytes,int microSec );
// int readFromSocketWithNoBlock (char * buffer, int maxBytes );
// int readFromSocketWithTimeout (char * buffer, int maxBytes, int timeoutSec,\
// 	int timeoutMilli);

int UDPSocket::readFromSocketWithBlock (char * buffer, int maxBytes )	{
	struct sockaddr_in aSocketAddress;
	int aLength, n;


	aLength = sizeof(aSocketAddress);
	aSocketAddress.sin_family = AF_INET; /* note that this is needed */
	if((n = recvfrom(sock, buffer,  maxBytes, 0, (struct sockaddr*)&aSocketAddress, (socklen_t*)&aLength))<0)
		perror("Receive 1") ;
	close(sock);

	peerAddr = aSocketAddress;

	return n;
}

UDPSocket::UDPSocket(){

}

UDPSocket::~UDPSocket(){

	
}

// int readSocketWithNoBlock (char * buffer, int maxBytes );
// int readSocketWithTimeout (char * buffer, int maxBytes, int timeoutSec, int timeoutMilli);
// int readSocketWithBlock (char * buffer, int maxBytes );
// int getMyPort ();
// int getPeerPort ();
// int getSocketHandler();


