#include "UDPSocket.h"
using namespace std;
int main(int argc, char const *argv[])
{
	UDPSocket client;
	char* test = "this is a test\n";
	client.initializeClient("", 0);
	client.writeToSocket(test, sizeof(test)/sizeof(char));
	return 0;
}
