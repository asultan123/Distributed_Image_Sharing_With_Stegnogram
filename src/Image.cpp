#include "Image.h"
//#include"Message.cpp"
#include<fstream>
#include<cstdlib>
#include <unistd.h>
#include<cstdio>
//#include"Communication.cpp"
#include<cstring>
#define IMAGE_DATA_SEPARATOR "||"
using namespace std;


Image::Image(Communcation* _comm, string _OwnName){
    comm = _comm;
	OwnName=_OwnName;
	}

Image::~Image(){}

void Image::loadOwnImages(vector<string> _OwnedImages){
	OwnedImages=_OwnedImages;
	}


//send
void Image::sendImageRequest(string ImageName,string UserName){
    string response = "DEFAULT TEXT";
    string dataToSend = ImageName+IMAGE_DATA_SEPARATOR+OwnName+IMAGE_DATA_SEPARATOR;
	if(!comm->sendMessage(UserName,"sendImageRequest", dataToSend, response))
	{
	        cout<<response<<endl;
	}
}

void Image::sendImageReply(string ImageName,string UserName){

	system("cd ~/Documents/Own_Images && touch cred.txt");
	system(("cd ~/Documents/Own_Images && echo "+ImageName+">>cred.txt").c_str());
	system("cd ~/Documents/Own_Images && echo 15>>cred.txt");
    system(("cd ~/Documents/Own_Images && echo "+OwnName+">>cred.txt").c_str());
	system(("cd ~/Documents/Own_Images && mv "+ImageName+".jpeg temp.jpeg").c_str());
//	system(("cd ~/Documents/Own_Images && steghide embed -cf temp.jpeg -ef cred.txt -sf "+ImageName+"_hidden.jpeg -p passphrase").c_str());
    system("cd ~/Documents/Own_Images && steghide embed -cf temp.jpeg -ef cred.txt -sf hidden.jpeg -p passphrase");
    system(("cd ~/Documents/Own_Images && steghide embed -cf default.jpeg -ef hidden.jpeg -sf "+ImageName+".jpeg -p passphrase").c_str());
	//image to string here:from ImageName to string
	//ImageToString(ImageName);
    struct passwd *pw = getpwuid(getuid());
	const char *homedir = pw->pw_dir;
    string homedir_string(homedir);
	string data=ImageToString(homedir_string+"/Documents/Own_Images/"+ImageName+".jpeg");
	//cout<<data;
	data_temp=data;
	//data=ImageName+IMAGE_DATA_SEPARATOR+data+IMAGE_DATA_SEPARATOR;
	//string data="this is the image sent";
	system(("cd ~/Documents/Own_Images && rm "+ImageName+".jpeg").c_str());
    system("cd ~/Documents/Own_Images && rm hidden.jpeg");

	system("cd ~/Documents/Own_Images && rm cred.txt");
	system(("cd ~/Documents/Own_Images && mv temp.jpeg "+ImageName+".jpeg").c_str());
	string response = "DEFAULT TEXT";
	if(!comm->sendMessage(UserName,"sendImageReply",data,response))
		cout<<response<<endl;
	}


void Image::sendListImagesRequest(string UserName){
    string response = "";
	if(!comm->sendMessage(UserName,"sendListImagesRequest",OwnName,response)){
		cout<<response<<endl;
	}
}



void Image::sendListImagesReply(string UserName){
	string data=to_string(OwnedImages.size())+IMAGE_DATA_SEPARATOR+OwnName+IMAGE_DATA_SEPARATOR;
	for(unsigned int i=0;i<OwnedImages.size();i++)
		{data+=OwnedImages[i];
		data+=IMAGE_DATA_SEPARATOR;}

    string response = "DEFAULT TEXT";
	if(!comm->sendMessage(UserName,"sendListImagesReply",data,response))
		cout<<response<<endl;

}


void Image::sendRequestIncreaseCount(string ImageName,string UserName){
string response = "DEFAULT TEXT";
string dataToSend = ImageName+IMAGE_DATA_SEPARATOR+OwnName+IMAGE_DATA_SEPARATOR;
	if(!comm->sendMessage(UserName,"sendRequestIncreaseCount",data_temp, response))
		cout<<response<<endl;
}


void Image::sendReplyIncreaseCount(string ImageName,string UserName){
	string response = "DEFAULT TEXT";
	if(!comm->sendMessage(UserName,"sendRequestIncreaseCount",ImageName,response))
		cout<<response<<endl;
}


//receive

void Image::recvImageRequest(string input){
    int pos=0;
	string ImageName,UserName;
	int found=0;

	found=input.find(IMAGE_DATA_SEPARATOR,pos);
	ImageName=input.substr(pos,found-pos);
	pos=found+2;

	found=input.find(IMAGE_DATA_SEPARATOR,pos);
	UserName=input.substr(pos,found-pos);

	//will be removed
    this->sendImageReply(ImageName,UserName);
    }

void Image::recvImageReply(string input){
	//from string to image using input(data received)
	 int pos=0;
	string ImageName,ImageData;
	int found=0;

	found=input.find(IMAGE_DATA_SEPARATOR,pos);
	ImageName=input.substr(pos,found-pos);
	pos=found+2;

	found=input.find(IMAGE_DATA_SEPARATOR,pos);
	ImageData=input.substr(pos,found-pos);
   // Image::StringToImage(ImageData,"/Documents/Received_Images/"+ImageName);
    struct passwd *pw = getpwuid(getuid());
    const char *homedir = pw->pw_dir;
    string homedir_string(homedir);
    Image::StringToImage(input,homedir_string+"/Documents/Received_Images/output.jpeg");

	system("cd ~/Documents/Received_Images && steghide extract -sf output.jpeg -p passphrase");
    system("cd ~/Documents/Received_Images && steghide extract -sf hidden.jpeg -p passphrase");
    ifstream file(homedir_string+"/Documents/Received_Images/cred.txt");
    string imagename,owner,views;
    getline(file,imagename);
    getline(file,views);
    getline(file,owner);
    system(("cd ~/Documents/Received_Images && mv output.jpeg "+imagename+".jpeg").c_str());
    system("cd ~/Documents/Received_Images && rm hidden.jpeg");
    system("cd ~/Documents/Received_Images && rm cred.txt");
    LoadedImages[imagename].ImageOwner=owner;
    LoadedImages[imagename].NumberOfViews=atoi(views.c_str());
	//this->updateAllDir();

	}


void Image::recvListImagesRequest(string input){
    //will be removed
    this->sendListImagesReply(input);
    }


void Image::recvListImagesReply(string input){
	int pos=0;
	string temp;
	int found=0;

	found=input.find(IMAGE_DATA_SEPARATOR,pos);
	temp=input.substr(pos,found-pos);
	pos=found+2;

	int max_index=atoi(temp.c_str());
	found=input.find(IMAGE_DATA_SEPARATOR,pos);
	string name=input.substr(pos,found-pos);
	pos=found+2;

	ImageList_struct list;
	list.Owner=name;

	for(int i=0;i<max_index;i++)
		{
		found=input.find(IMAGE_DATA_SEPARATOR,pos);
		temp=input.substr(pos,found-pos);
		pos=found+2;
		list.List.push_back(temp);
		}
	ImageList_vector.push_back(list);

	}

void Image::recvRequestIncreaseCount(string input){
    int pos=0;
	string ImageName,UserName;
	int found=0;

	found=input.find(IMAGE_DATA_SEPARATOR,pos);
	ImageName=input.substr(pos,found-pos);
	pos=found+2;

	found=input.find(IMAGE_DATA_SEPARATOR,pos);
	UserName=input.substr(pos,found-pos);
    //will be removed
    this->sendReplyIncreaseCount(ImageName,UserName);
    }

void Image::recvReplyIncreaseCount(string input){
	LoadedImages[input].NumberOfViews+=10;
	this->updateDir(input);
	}


void Image::updateDir(string ImageName){//to update text file hidden inside the image after viewing or receiving an image

	system("cd ~/Documents/Received_Images && touch cred.txt");

	system(("cd ~/Documents/Received_Images && echo "+ImageName+">>cred.txt").c_str());
	system(("cd ~/Documents/Received_Images && echo 00"+to_string(LoadedImages[ImageName].NumberOfViews)+">>cred.txt").c_str());
    system(("cd ~/Documents/Received_Images && echo "+LoadedImages[ImageName].ImageOwner+">>cred.txt").c_str());
	//file<<ImageName<<endl;
	//file<<LoadedImages[ImageName].NumberOfViews<<endl;
	//file<<LoadedImages[ImageName].ImageOwner<<endl;
	//file.close();

	system(("cd ~/Documents/Received_Images && steghide extract -sf "+ImageName+".jpeg -p passphrase").c_str());
	system(("cd ~/Documents/Received_Images && rm "+ImageName+".jpeg").c_str());
	system("cd ~/Documents/Received_Images && steghide embed -cf hidden.jpeg -ef cred.txt -p passphrase");
	system(("cd ~/Documents/Received_Images && steghide embed -cf default.jpeg -ef hidden.jpeg -sf "+ImageName+".jpeg -p passphrase").c_str());
	system("cd ~/Documents/Received_Images && rm hidden.jpeg");
	system("cd ~/Documents/Received_Images && rm cred.txt");
	}

void Image::updateAllDir(){
	string name;
	std::map<string,ImageInfo>::iterator it;
	for (it=LoadedImages.begin(); it!=LoadedImages.end(); ++it){
		name=it->first;
		this->updateDir(name);
		}
    }

void Image::loadFromDir(vector<string> images){
    struct passwd *pw = getpwuid(getuid());
    const char *homedir = pw->pw_dir;
    string homedir_string(homedir);


    for(unsigned i=0;i<images.size();i++){

        system(("cd ~/Documents/Received_Images && steghide extract -sf "+images[i]+".jpeg -p passphrase").c_str());
        system("cd ~/Documents/Received_Images && steghide extract -sf hidden.jpeg -p passphrase");
        ifstream file(homedir_string+"/Documents/Received_Images/cred.txt");
        string imagename,owner,views;
        getline(file,imagename);
        getline(file,views);
        getline(file,owner);
        //system(("cd ~/Documents/Received_Images && mv "+images[i]+".jpeg "+imagename+".jpeg").c_str());
        system("cd ~/Documents/Received_Images && rm hidden.jpeg");
        system("cd ~/Documents/Received_Images && rm cred.txt");
        LoadedImages[imagename].ImageOwner=owner;
        LoadedImages[imagename].NumberOfViews=atoi(views.c_str());
        }

    }


void Image::decreaseViewsByOne(string ImageName){

		LoadedImages[ImageName].NumberOfViews-=1;
		this->updateDir(ImageName);

	//else this->deleteImage(ImageName);
	}

void Image::deleteImage(string ImageName){
	LoadedImages.erase(ImageName);
	system(("cd ~/Documents/Received_Images && rm "+ImageName+".jpeg").c_str());
	}

string Image::previewImage(string ImageName){
//extract and convert to string then delete extracted image
    struct passwd *pw = getpwuid(getuid());
    const char *homedir = pw->pw_dir;
    string homedir_string(homedir);
    if(LoadedImages[ImageName].NumberOfViews!=0){
        LoadedImages[ImageName].numOfTimesViewed++;
        system(("cd ~/Documents/Received_Images && steghide extract -sf "+ImageName+".jpeg -p passphrase").c_str());
        string s=ImageToString(homedir_string+"/Documents/Received_Images/hidden.jpeg");
        system("cd ~/Documents/Received_Images && rm hidden.jpeg");
        this->decreaseViewsByOne(ImageName);
        return s;}
    else return ImageToString(homedir_string+"/Documents/Received_Images/"+ImageName+".jpeg");

	}


vector<Image::ImageList_struct> Image::receivedLists(){
	return ImageList_vector;
	}

vector<string> Image::userList(string UserName){
    for(unsigned int i=0;i<ImageList_vector.size();i++ )
        if(ImageList_vector[i].Owner==UserName)
            return ImageList_vector[i].List;


    return vector<string>();
}

    void Image::cout_LoadedImages(){
    string name;
	std::map<string,ImageInfo>::iterator it;
	for (it=LoadedImages.begin(); it!=LoadedImages.end(); ++it){
		name=it->first;
		cout<<"imagename="<<name<<endl;
		cout<<"owner="<<LoadedImages[name].ImageOwner<<endl;
		cout<<"views="<<LoadedImages[name].NumberOfViews<<endl;

		}


    }


    void  Image::coutStatistics(){
        string name;
        std::map<string,int>::iterator it;
        for (it=Statistics.begin(); it!=Statistics.end(); ++it){
            name=it->first;
            cout<<"imagename="<<name<<endl;
            cout<<"views="<<Statistics[name]<<endl;

        }
    }


//need to implement all keyword understood by the coms module
string Image::sendRequestIquiry(){
    string response = "";
    if(!comm->sendMessage("ALL","sendRequestInquiry",OwnName, response))
		cout<<"sendImageRequest failed";
		return OwnName;
}


string Image::sendReplyInquiry(string input){
    string data;
    //data=OwnName+IMAGE_DATA_SEPARATOR;
    int counter=0;
	std::map<string,ImageInfo>::iterator it;
	for (it=LoadedImages.begin(); it!=LoadedImages.end(); ++it){
		if(LoadedImages[it->first].ImageOwner==input){
            counter++;
            }
        }
        data=to_string(counter)+IMAGE_DATA_SEPARATOR;
        for (it=LoadedImages.begin(); it!=LoadedImages.end(); ++it){
		if(LoadedImages[it->first].ImageOwner==input){
            data+=it->first+IMAGE_DATA_SEPARATOR+to_string(LoadedImages[it->first].numOfTimesViewed)+IMAGE_DATA_SEPARATOR;
            }
        }
        string response = "DEFAULT TEXT";
       if(!comm->sendMessage(input,"sendReplyInquiry",data, response))
            cout<<response<<endl;
        return data;
}


void Image::recvReplyInquiry(string input){

    int pos=0;
	string numberOfImages,tempName,tempViews;
	//string UserName;
	int found=0;

	//found=input.find(IMAGE_DATA_SEPARATOR,pos);
	//UserName=input.substr(pos,found-pos);
	//pos=found+2;

	found=input.find(IMAGE_DATA_SEPARATOR,pos);
	numberOfImages=input.substr(pos,found-pos);
    pos=found+2;

    for(int i=0;i<atoi(numberOfImages.c_str());i++){
        found=input.find(IMAGE_DATA_SEPARATOR,pos);
        tempName=input.substr(pos,found-pos);
        pos=found+2;

        found=input.find(IMAGE_DATA_SEPARATOR,pos);
        tempViews=input.substr(pos,found-pos);
        pos=found+2;
       //will be changed if needed to implement robot in different way

        Statistics[tempName]=atoi(tempViews.c_str());
        }


}


void Image::initializeStatistics(){
    for(unsigned int i=0;i<OwnedImages.size();i++){
        //Statistics[OwnedImages[i]].viewsRemaing=0;
       // Statistics[OwnedImages[i]].viewsSent=0;
        Statistics[OwnedImages[i]]=0;
        }
    }

    map<string,Image::ImageInfo> Image::returnReceivedImages(){
        return LoadedImages;
        }

