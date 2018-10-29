#include "Message.h"

Message::Message(int _id, string ip, int port, string _type, string _data,\
 int _segmentOrder, int _segmentCount){
	id = _id;
	port = _port;
	type = _type;
	data = _data;
	segmentOrder = _segmentOrder;
	_segmentCount = _segmentCount;
}

Message::~Message();

string Message::getMessageId(){
	return string()
}

void Message::getData(string& _type,Data& data);

string Message::getIp();

int Message::getPort();

string Message::marshal();

void Message::unmarshal(string input);

Message Message::assembleBigMessage(std::vector<Message> minis);
std::vector<Message> Message::createMessages(int _id, string ip, int port,\
	int type, Data data);
