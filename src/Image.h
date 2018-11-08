#ifndef IMAGE_H
#define IMAGE_H
#include <string>
#include <vector>
#include<map>
#include<cstdlib>
#include<cstdio>
#include<iostream>
#include<jpeglib.h>
#include<fstream>
#include<unistd.h>
#include<sys/types.h>
#include<pwd.h>
#include<Communcation.h>

using namespace std;


class Image
	{

	public:


	string data_temp;

        struct ImageInfo{
						int NumberOfViews;//remaining
						int numOfTimesViewed=0;
						string ImageOwner;
						//sting ExtractedImage;
						};
		struct ImageList_struct{
						string Owner;
						vector<string> List;
					};

   //     struct statistics_struct{
     //                           int viewsSent;
       //                         int viewsRemaing;
         //                       int totalViews=viewsSent-viewsRemaing;
           //                     }
		Image(Communcation* _comm, string _OwnName); //check
		~Image();
		void loadOwnImages(vector<string> _OwnedImages); //check
		//send
		void sendImageRequest(string ImageName,string UserName);//check
		void sendImageReply(string ImageName,string UserName); //check without image to string
		void sendListImagesRequest(string UserName);//check
		void sendListImagesReply(string UserName);//check
		void sendRequestIncreaseCount(string ImageName,string UserName);
		void sendReplyIncreaseCount(string ImageName,string UserName);

		//receive
		void recvImageRequest(string input);//check
		void recvImageReply(string input);
		void recvListImagesRequest(string input);//check
		void recvListImagesReply(string input);//check
		void recvRequestIncreaseCount(string input);
		void recvReplyIncreaseCount(string input);


		void updateDir(string ImageName);
		void updateAllDir();
        void cout_LoadedImages();
        void loadFromDir(vector<string> images);

		void decreaseViewsByOne(string ImageName);
		void deleteImage(string ImageName);
		string previewImage(string ImageName);
		vector<Image::ImageList_struct> receivedLists();//check
        vector<string> userList(string UserName);//check
        map<string,ImageInfo> returnReceivedImages();
        Communcation* comm;


        //robot
        //for now keep string for testing
        string sendRequestIquiry();
        string sendReplyInquiry(string input);
        void recvReplyInquiry(string input);
        void initializeStatistics();
        void  coutStatistics();



 static string ImageToString(string ImagePath)
{
    streampos begin,end;
    ifstream myfile (ImagePath, ios::binary);
    begin = myfile.tellg();
    myfile.seekg (0, ios::end);
    end = myfile.tellg();
    cout << "size is: " << (end-begin) << " bytes noob.\n";
    streamsize n=end-begin;
    vector<char> data((size_t)n);
    myfile.seekg(0,ios::beg);
    myfile.read(&data[0],n);
    cout<<"Size of Data = " << data.size() <<endl;
    string imagestring(data.begin(),data.end());
    return imagestring;
}
 static void StringToImage(string ImageString,string ImageName){

    vector<char> data;
    std::copy( ImageString.begin(), ImageString.end(), std::back_inserter(data));
    //ofstream outfile (homedir_string+ImageName+".jpeg", ios::binary);
    ofstream outfile (ImageName, ios::binary);
    //ofstream outfile ("output.jpg", ios::binary);

    if (outfile.is_open())
    {
    for (unsigned int i = 0 ; i < data.size(); i++) outfile << data[i];
    outfile.close();
    }
    else cout << "Unable to open file";

}

		protected:


		vector<ImageList_struct> ImageList_vector;
		map<string,ImageInfo> LoadedImages;
		vector<string> OwnedImages;
		string OwnName;
		//map<string,statistics_struct>Statistics;
        map<string,int>Statistics;

	};
#endif












