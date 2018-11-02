#ifndef MESSAGE_H
#define MESSAGE_H
//#include "string.h"
#include <vector>
#include <string>
#include "base64.h"
#include <iostream>
#include <set>

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
    Message(const Message& rhs);

	~Message();

    bool operator==(const Message& lhs);

    bool operator!=(const Message& lhs);

	int getMessageId() const;

	void printMessage();

	string getType() const;

	string getData() const;

	string getIp() const;

	int getPort() const;

	int getSegmentOrder() const;

	int getSegmentCount() const;

	string marshal();

	void unmarshal(string input);

	string getMessageUID();

	static string to_string(int input);

    struct MessageCompare{
        bool operator()(const Message& lhs, const Message& rhs);
    };

	static Message assembleBigMessage(std::vector<Message> minis);
	static Message assembleBigMessage(std::set<Message,MessageCompare> minis);

	static std::vector<Message> createMessages(int _id, string _ip, int _port,string _type, string _data, int max_packet_size);

};
#endif
//#include"Message.cpp"
