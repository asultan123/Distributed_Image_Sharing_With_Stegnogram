#include "Image.h"
//#include"Message.h"
#include<fstream>
#include<cstdlib>
#include<cstdio>
#include<string>
struct ImageList_struct{
						string Owner;
						vector<string> List;
					};




using namespace std;

int main(){

Image img_obj_1("karim");

Image img_obj_2("asser");
Image img_obj_3("ali");
vector<string> u1_list,u3_list;
u1_list.push_back("img1");
u1_list.push_back("img2");
u1_list.push_back("img3");
u1_list.push_back("secret");
u3_list.push_back("img4");
u3_list.push_back("img5");
u3_list.push_back("img6");
//ImageList_struct s1,s2;
img_obj_1.loadOwnImages(u1_list);
img_obj_3.loadOwnImages(u3_list);
img_obj_1.sendListImagesReply("asser");


img_obj_2.recvListImagesReply("3||karim||img1||img2||img3||");
img_obj_2.recvListImagesReply("3||ali||img4||img5||img6||");
vector<Image::ImageList_struct> vec1=img_obj_2.receivedLists();

//cout<<vec1.size()<<endl;
//cout<<vec1[0].Owner<<endl;
//cout<<vec1[0].List[0]<<endl;
//cout<<vec1[0].List[1]<<endl;
//cout<<vec1[0].List[2]<<endl;
//cout<<vec1[1].Owner<<endl;
//cout<<vec1[1].List[0]<<endl;
//cout<<vec1[1].List[1]<<endl;
//cout<<vec1[1].List[2]<<endl;


img_obj_2.sendImageRequest("secret","karim");
img_obj_1.recvImageRequest("secret||asser||");
string s=img_obj_1.data_temp;
img_obj_2.recvImageReply(s);
vector<string> images;
images.push_back("secret");
img_obj_2.loadFromDir(images);
img_obj_2.cout_LoadedImages();
//img_obj_2.decreaseViewsByOne("secret");
//img_obj_2.cout_LoadedImages();

string s1=img_obj_2.previewImage("secret");
img_obj_2.cout_LoadedImages();
string s2=img_obj_2.previewImage("secret");
img_obj_2.cout_LoadedImages();
string s3=img_obj_2.previewImage("secret");
img_obj_2.cout_LoadedImages();
string s4=img_obj_2.previewImage("secret");
img_obj_2.cout_LoadedImages();
//string s5=img_obj_2.previewImage("secret");
//img_obj_2.cout_LoadedImages();

string string1=img_obj_1.sendRequestIquiry();
string string2=img_obj_2.sendReplyInquiry(string1);
img_obj_1.initializeStatistics();
img_obj_1.recvReplyInquiry(string2);

img_obj_1.coutStatistics();



vector<string> vec2=img_obj_2.userList("ali");

//cout<<vec2.size()<<endl;
//cout<<vec2[0]<<endl;
//cout<<vec2[1]<<endl;
//cout<<vec2[2]<<endl;


//img_obj_1.sendListImagesRequest("asser");
//img_obj_1.recvListImagesRequest("asser");



//string s=Image::ImageToString("/home/osboxes/Documents/Own_Images/secret.jpeg");
//Image::StringToImage(s1,"1.jpeg");
//Image::StringToImage(s2,"2.jpeg");
//Image::StringToImage(s3,"3.jpeg");
//Image::StringToImage(s4,"4.jpeg");
//Image::StringToImage(s5,"5.jpeg");
return 0;
}
