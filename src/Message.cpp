#include "Message.h"
#include<cstdlib>
#include<cmath>
#include<cstdio>
#include <string>
#include "Message.h"

#define SEPARATOR "!!"
using namespace std;

Message::Message(int _id, string _ip, int _port, string _type, string _data,int _segmentOrder, int _segmentCount){
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

vector<Message> Message::randomMessageBrokenDownToMiniMessages(int _id, int _port,string _ip,string _type,int dataLength, int maxPacketSize,int maxSegmentCount)
{
    string temp = randomLowerCaseString(dataLength);
    return createMessages(_id,_ip,_port,_type,temp,maxPacketSize,maxSegmentCount);
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

Message::Message(const Message& rhs){
    this->id = rhs.id;
	this->ip= rhs.ip;
	this->port = rhs.port;
	this->type = rhs.type;
	this->data = rhs.data;
	this->segmentOrder = rhs.segmentOrder;
	this->segmentCount = rhs.segmentCount;
}

string Message::padNumberWithLeadingZeros(int number, int numberWidth){
    string res = to_string(number);
    int currentNumberWidth = res.size();
    if(currentNumberWidth>numberWidth){
        cout<<"Invalid padding request made, number is bigger than desired width"<<endl;
        throw "";
    }
    int numberOfZeroesToAdd = numberWidth - currentNumberWidth;
    for(int i = 0; i<numberOfZeroesToAdd; i++){
        res = '0' + res;
    }
    return res;
}

string Message::marshal(int maxSegmentCount, int maxPacketSize){

    string temp = to_string(id);
    temp+=SEPARATOR;
    temp+=ip;
    temp+=SEPARATOR;
    temp+=to_string(port);
    temp+=SEPARATOR;
    temp+=type;
    temp+=SEPARATOR;
    temp+=data;
    temp+=SEPARATOR;
    temp+=to_string(segmentOrder);
    temp+=SEPARATOR;
    temp+=padNumberWithLeadingZeros(segmentCount, maxSegmentCount);
    temp+=SEPARATOR;

	//base64_encode((unsigned char*)temp.c_str(), maxPacketSize);

return temp;

}

string Message::getMessageUID(){
    return to_string(getMessageId())+ getIp() + to_string(getPort()) + type;
}

int Message::messageSize(int maxSegmentCount, int maxPacketSize){
    return marshal(to_string(maxSegmentCount).size(), maxPacketSize).size();
}

void Message::unmarshal(string input)
	{

	//input = base64_decode(input);

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
			case 6:
                for(int segCountStart = 0; segCountStart<temp.size(); segCountStart++){
                    if(temp[segCountStart]=='0'){
                        continue;
                    }
                    else{
                        segmentCount = atoi((char*)temp.substr(segCountStart,4).c_str());
                        break;
                    }
                }
			break;
			}
		}
	}

string Message::printMessage(){
    stringstream ss;

	ss << "id=" << getMessageId() << endl;
	ss << "ip=" << getIp() << endl;
	ss << "port=" << getPort() << endl;
	ss << "type=" << getType() << endl;
	ss << "data=" << getData() << endl;
	ss << "order=" << getSegmentOrder() << endl;
	ss << "count=" << getSegmentCount() << endl;

	return ss.str();
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


std::vector<Message> Message::createMessages(int _id, string _ip, int _port,string _type, string& _data, int maxPacketSize, int maxSegmentCount)
{
    int headerSize = to_string(_id).size()+_ip.size()+to_string(_port).size()+_type.size();
    int footerSize = to_string(maxSegmentCount).size() + to_string(maxSegmentCount).size();
    int seperatorOverhead = 7*string(SEPARATOR).size();
    int dataSizePerPacket = maxPacketSize - headerSize - footerSize - seperatorOverhead;
    bool dataAvailable = true;
    int _segmentCount = 0;

    vector<string> dataSegments;

    while(dataAvailable){
        string dataSegment = _data.substr(_segmentCount*dataSizePerPacket, dataSizePerPacket);
        dataSegments.push_back(dataSegment);
        if(dataSegment.size()<dataSizePerPacket){
            dataAvailable = false;
        }
        _segmentCount++;
    }

    vector<Message> msgs;

    for(int _segmentOrder = 0; _segmentOrder<_segmentCount; _segmentOrder++){
        Message msg(_id, _ip, _port, _type, dataSegments[_segmentOrder], _segmentOrder, _segmentCount);
        msg.marshal(to_string(maxSegmentCount).size(), maxPacketSize);
        msgs.push_back(msg);
    }

	return msgs;
}



