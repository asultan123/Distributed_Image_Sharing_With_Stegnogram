#include<cstdlib>
#include<cstdio>
#include<iostream>
#include<string>
#include "Message.cpp"

using namespace std;

int main() {

	Message msg(1, "192.168.1.1", 8888,"Type","Data" ,2,3),msg2;

	cout << "id=" << msg.getMessageId()<<endl;
	cout << "ip=" << msg.getIp() << endl;
	cout << "port=" << msg.getPort() << endl;
	cout << "type=" << msg.getType() << endl;
	cout << "type=" << msg.getData() << endl;
	cout << "order=" << msg.getSegmentOrder() << endl;
	cout << "count=" << msg.getSegmentCount() << endl;

	cout << "marshalled=" << msg.marshal() << endl;


	msg2.unmarshal(msg.marshal());

	cout << "id=" << msg2.getMessageId() << endl;
	cout << "ip=" << msg2.getIp() << endl;
	cout << "port=" << msg2.getPort() << endl;
	cout << "type=" << msg2.getType() << endl;
	cout << "type=" << msg2.getData() << endl;
	cout << "order=" << msg2.getSegmentOrder() << endl;
	cout << "count=" << msg2.getSegmentCount() << endl;

	vector<Message> msgs;
	msgs = createMessages(3, "192.168.1.2", 7777, "Type2", "kkkkkkkkkkkkkkkkkkkkkkkkkkkkkkk");


	for (int i = 0; i < msgs.size(); i++) {
		cout << "id=" << msgs[i].getMessageId() << endl;
		cout << "ip=" << msgs[i].getIp() << endl;
		cout << "port=" << msgs[i].getPort() << endl;
		cout << "type=" << msgs[i].getType() << endl;
		cout << "type=" << msgs[i].getData() << endl;
		cout << "order=" << msgs[i].getSegmentOrder() << endl;
		cout << "count=" << msgs[i].getSegmentCount() << endl;
	}


	Message msg3;

	msg3 = assembleBigMessage(msgs);

	cout << "id=" << msg3.getMessageId() << endl;
	cout << "ip=" << msg3.getIp() << endl;
	cout << "port=" << msg3.getPort() << endl;
	cout << "type=" << msg3.getType() << endl;
	cout << "type=" << msg3.getData() << endl;
	cout << "order=" << msg3.getSegmentOrder() << endl;
	cout << "count=" << msg3.getSegmentCount() << endl;

	system("pause");


	return 0;
}