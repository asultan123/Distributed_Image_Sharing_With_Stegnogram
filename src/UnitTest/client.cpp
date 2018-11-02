#include "UDPSocket.h"
using namespace std;
int main(int argc, char const *argv[])
{
	UDPSocket client;
	char* test = "this is a test\n";
	client.initializeClient("172.16.115.196", 50000);
	client.writeToSocket(test, 500);
	return 0;
}
