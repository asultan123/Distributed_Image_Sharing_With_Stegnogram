#include "UDPSocket.h"
using namespace std;
int main(int argc, char const *argv[])
{
	UDPSocket server;
	char* test = (char*) malloc(500*sizeof(char));
	server.initializeServer("10.7.57.252", 50000);
	server.readFromSocketWithBlock (test, 500 );
	printf("%s\n",test);
	server.readSocketWithTimeout(test, 500, 5, 500);
	printf("%s\n",test);
	server.readSocketWithNoBlock(test,500);
	printf("%s\n",test);
	return 0;
}
