#ifndef Message
#define Message
#include "string"
#include <vector>
#include ""

using namespace std;

class Message
{
protected:
	int id;
	string ip;
	int port;
	string type;
	string data;
	int segmentOrder;
	int segmentCount;

public:

	Message(int _id, string ip, int port, string type, string data,\
	 int segmentOrder, int segmentCount);
	~Message();

	string getMessageId();

	void getData(string& _type,Data& data);

	string getIp();

	int getPort();

	string marshal();

	void unmarshal(string input);

	static Message assembleBigMessage(std::vector<Message> minis);
	static std::vector<Message> createMessages(int _id, string ip, int port,\
		int type, Data data);
	
};
#endif