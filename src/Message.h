#ifndef MESSAGE_H
#define MESSAGE_H
//#include "string.h"
#include <vector>
//#include<cstdlib>
//#include<cstdio>
//#include ""

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
	Message();
	Message(int _id, string _ip, int _port, string _type, string _data,int _segmentOrder, int _segmentCount);
	
	~Message();

	int getMessageId();

	string getType();
	
	string getData();

	string getIp();

	int getPort();
	
	int getSegmentOrder();

	int getSegmentCount();

	string marshal();

	void unmarshal(string input);

	static Message assembleBigMessage(std::vector<Message> minis);
	static std::vector<Message> createMessages(int _id, string _ip, int _port,string _type, string _data);
	
};
#endif
//#include"Message.cpp"