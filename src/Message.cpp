#include "Message.h"
#include<cstdlib>
#include<cmath>
#include<cstdio>
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

int Message::getMessageId(){
	return id;
}

string Message::getData(){
	return data;
	
}
string Message::getType(){
	return type;	
}

int Message::getSegmentOrder(){
	return segmentOrder;	
}

int Message::getSegmentCount(){
	return segmentCount;	
}

string Message::getIp(){
	return ip;
}

int Message::getPort(){
	return port;	
}

string Message::marshal(){
return to_string(id)+SEPARATOR+ip+SEPARATOR+to_string(port)+SEPARATOR+type+SEPARATOR+data+SEPARATOR\
	+to_string(segmentOrder)+SEPARATOR+to_string(segmentCount)+SEPARATOR;
	 
	
}

void Message::unmarshal(string input)
	{
	
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

Message assembleBigMessage(std::vector<Message> minis)
	{
	
	string temp="";
	for(int i=0;i< minis.size();i++)
		temp+=minis[i].getData();
	Message msg(minis[0].getMessageId(),minis[0].getIp(),minis[0].getPort(),minis[0].getType(),temp,1,1);
	return msg;
	
	}



std::vector<Message> createMessages(int _id, string _ip, int _port,\
string _type, string _data)
	{
	string temp;
	int x=_data.length();
		
		
	int count=ceil(x/10.0);
	//data=_data.makestring();
	vector<Message> msgs;
	for(int i=0;i<count;i++)
		{
		
		temp=_data.substr(i*10,10);
		Message msg(_id,_ip,_port,_type,temp,i+1,count);
		msgs.push_back(msg);
		
		
		}
	return msgs;
	}



