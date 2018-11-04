#include "Message.h"
#include<cstdlib>
#include<cmath>
#include<cstdio>
#include <string>
#include "Message.h"

#define SEPARATOR "!!"
using namespace std;

Message::Message(int _id, string _ip, int _port, string _type, string _data,\
 int _segmentOrder, int _segmentCount){
	 id = _id;
	ip=_ip;
	port = _port;
	type = _type;
	data = _data;
	segmentOrder = _segmentOrder;
	segmentCount = _segmentCount;
}


Message::Message()
{
}


Message::~Message()
{
}

string Message::randomLowerCaseString(int length){
    string temp = "";
    for(int i = 0; i<length; i++){
        temp+= char(rand()%26 + 'a');
    }
    return temp;
}

vector<Message> Message::randomMessagesWith(int _id, int _port, string _ip,string _type, int msgCount){
    const int randStringLength = 10;
    vector<Message> temp;

    for(int i = 0; i<msgCount; i++){
        Message tmpMessage(_id, _ip, _port, _type\
        , randomLowerCaseString(randStringLength),i, msgCount);
        temp.push_back(tmpMessage);
    }

    return temp;
}

bool Message::MessageCompare::operator()(const Message& lhs, const Message& rhs){
    return lhs.getSegmentOrder() < rhs.getSegmentOrder();
}

int Message::getMessageId() const{
	return id;
}

string Message::getData() const{
	return data;

}
string Message::getType() const{
	return type;
}

int Message::getSegmentOrder() const{
	return segmentOrder;
}

int Message::getSegmentCount() const{
	return segmentCount;
}

string Message::getIp() const{
	return ip;
}

int Message::getPort() const{
	return port;
}

bool Message::operator==(const Message& lhs){
    bool temp;
    temp = id == lhs.id;
    temp = temp && ip == lhs.ip;
    temp = temp && port == lhs.port;
    temp = temp && type == lhs.type;
    temp = temp && data == lhs.data;
    temp = temp && segmentOrder == lhs.segmentOrder;
    temp = temp && segmentCount == lhs.segmentCount;
    return temp;
}

bool Message::operator!=(const Message& lhs){
    return !(*this==lhs);
}

//string Message::to_string(int input){
//	string temp = "";
//
//	while(input>0){
//		temp = (char)((int)'0' + input%10) + temp;
//		input = input / 10;
//	}
//
//	return temp;
//}

Message::Message(const Message& rhs){
    this->id = rhs.id;
	this->ip= rhs.ip;
	this->port = rhs.port;
	this->type = rhs.type;
	this->data = rhs.data;
	this->segmentOrder = rhs.segmentOrder;
	this->segmentCount = rhs.segmentCount;
}

string Message::marshal(){

string temp = to_string(id)+SEPARATOR+ip+SEPARATOR+to_string(port)+SEPARATOR+type+SEPARATOR+data+SEPARATOR\
	+to_string(segmentOrder)+SEPARATOR+to_string(segmentCount)+SEPARATOR;

return base64_encode((unsigned char*)temp.c_str(), temp.length());

}

string Message::getMessageUID(){
    return to_string(getMessageId())+ getIp() + to_string(getPort()) + type;
}

void Message::unmarshal(string input)
	{

	input = base64_decode(input);

	int pos=0;
	string temp="";
	int found=0;
	for(int i=0;i<=6;i++)
		{
		found=input.find(SEPARATOR,pos);
		temp=input.substr(pos,found-pos);
		pos=found+2;
		switch(i)
			{
			case 0:id=atoi(temp.c_str());
			break;
			case 1:ip=temp;
			break;
			case 2:port=atoi(temp.c_str());
			break;
			case 3:type=temp;
			break;
			case 4:data=temp;
			break;
			case 5:segmentOrder=atoi(temp.c_str());
			break;
			case 6:segmentCount=atoi(temp.c_str());
			break;
			}
		}
	}

void Message::printMessage(){
	cout << "id=" << getMessageId() << endl;
	cout << "ip=" << getIp() << endl;
	cout << "port=" << getPort() << endl;
	cout << "type=" << getType() << endl;
	cout << "data=" << getData() << endl;
	cout << "order=" << getSegmentOrder() << endl;
	cout << "count=" << getSegmentCount() << endl;
}

Message Message::assembleBigMessage(std::vector<Message> minis)
{
    string temp = "";
    for(auto& it : minis){
        temp += it.getData();
    }
    Message msg(minis[0].getMessageId(),minis[0].getIp(),minis[0].getPort(),minis[0].getType(),temp,0,1);
    return msg;
}

Message Message::assembleBigMessage(std::set<Message,MessageCompare> minis)
{
    vector<Message> temp(minis.begin(),minis.end());
    return assembleBigMessage(temp);
}


std::vector<Message> Message::createMessages(int _id, string _ip, int _port,\
string _type, string _data, int maxPacketSize)
{
	string temp;
	int headerSize = to_string(_id).length() + _ip.length() + to_string(_port).length() + _type.length();

    try{
    	if(maxPacketSize <= headerSize){
            cout<<"insufficient packet size defined for message " + to_string(_id) \
            + _ip + to_string(_port);
            throw;
        }
    }
	catch(...){
        cout<<"default max packet size set to 8kb"<<endl;
        maxPacketSize = 8000;
	}

	float remainingLength = maxPacketSize - headerSize;

	int count=ceil(_data.length()/remainingLength);

	vector<Message> msgs;
	for(int i=0;i<count;i++)
    {

        temp=_data.substr(i*remainingLength,remainingLength);
        Message msg(_id,_ip,_port,_type,temp,i,count);
        msgs.push_back(msg);


    }
	return msgs;
}



