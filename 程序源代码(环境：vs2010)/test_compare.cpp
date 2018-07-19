/*	#include <opencv2/opencv.hpp>
#include "main.h"
using namespace std;
string ImageHashValue(IplImage* src);  //����ͼƬ��ָ����Ϣ
double ImageSimilarity(string &str1,string &str2);  //����ָ����Ϣ��������ͼ������ƶ�

int main()
{
IplImage* image1 = cvLoadImage("example1.jpg",1);
IplImage* image2 = cvLoadImage("example3.jpg",1);
cvShowImage("image1",image1);
cvShowImage("image2",image2);
string imgPrint1 = ImageHashValue(image1);
string imgPrint2 = ImageHashValue(image2);
double similarity = ImageSimilarity(imgPrint1,imgPrint2);
cout<<"The similarity of two images is "<<similarity*100<<"%"<<endl;
if(similarity>=0.9)
cout<<"The two images are extremely similar."<<endl;
else if(similarity>=0.8&&similarity<0.9)
cout<<"The two images are pretty similar."<<endl;
else if(similarity>=0.7&&similarity<0.8)
cout<<"The two images are a little similar."<<endl;
else if(similarity<0.7)
cout<<"The two image are not similar."<<endl;
cout<<endl;
cvWaitKey(0);
}

//����ͼƬ��ָ����Ϣ
string ImageHashValue(IplImage* src)
{
string resStr(64,'\0');
IplImage* image =  cvCreateImage(cvGetSize(src),src->depth,1);
//step one : �ҶȻ�
if(src->nChannels == 3)  cvCvtColor(src,image,CV_BGR2GRAY);
else  cvCopy(src,image);
//step two : ��С�ߴ� 8*8
IplImage* temp = cvCreateImage(cvSize(8,8),image->depth,1);
cvResize(image,temp);
//step three : ��ɫ��
uchar* pData;
for(int i=0; i<temp->height; i++)
{
pData =(uchar* )(temp->imageData+i*temp->widthStep);
for(int j=0; j<temp->width;j++)
pData[j]= pData[j]/4;
}
//step four : ����ƽ���Ҷ�ֵ
int average = cvAvg(temp).val[0];
//step five : �����ϣֵ
int index = 0;
for(int i=0; i<temp->height; i++)
{
pData =(uchar* )(temp->imageData+i*temp->widthStep);
for(int j=0; j<temp->width;j++)
{
if(pData[j]>=average)
resStr[index++]='1';
else 
resStr[index++]='0';
}
}
return resStr;
}

//����ָ����Ϣ��������ͼ������ƶ�
double ImageSimilarity(string &str1,string &str2)
{
double similarity = 1.0;
for(int i=0;i<64;i++)
{
char c1 = str1[i];
char c2 = str2[i];
if(c1!=c2)
similarity = similarity -1.0/64;
}
return similarity;
}*/