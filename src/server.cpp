#include "UDPSocket.h"
using namespace std;
int main(int argc, char const *argv[])
{
	UDPSocket server;
	char* test;
	server.initializeServer("", 0);
	server.readFromSocketWithBlock (test, 500 );
	return 0;
}