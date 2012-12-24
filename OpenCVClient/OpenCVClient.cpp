// OpenCVClient.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "winsock2.h"
#include "windows.h"
#include <vector>
#pragma comment(lib,"ws2_32.lib")

using namespace std;
using namespace cv;

char *sockdata;
char *imgdata;
int ImgOK,imgsize;
SOCKET sock;
IplImage* img;

void GetData(){
	while(1){
	int  i, j, k, bytes;
    for (i = 0; i < imgsize; i += bytes) {
        if ((bytes = recv(sock, sockdata + i, imgsize - i, 0)) == -1) {
            //quit("recv failed", 1);
        } 
    }


	ImgOK=1;
	Sleep(1000);
	}
}
int _tmain(int argc, _TCHAR* argv[])
{
	
	int width,height;
	width=640;
	height=480;
	img = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 1);
    cvZero(img);


	cvNamedWindow("stream_client", CV_WINDOW_AUTOSIZE);

	WSADATA wsa;
    //初始化套接字DLL
    if(WSAStartup(MAKEWORD(2,2),&wsa)!=0){
        printf("套接字初始化失败!");
        
    }
	imgsize = width*height;
    sockdata=(char*)malloc(imgsize);
	imgdata=(char*)malloc(imgsize);
    
	short port=atoi("8080");

	
    if((sock=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP))==INVALID_SOCKET){
        printf("创建套接字失败！");
        
    }

    struct sockaddr_in clientAddress;//用来和客户端通信的套接字地址
    memset(&clientAddress,0,sizeof(clientAddress));
    int addrlen = sizeof(clientAddress);
	clientAddress.sin_family=AF_INET;
    clientAddress.sin_addr.S_un.S_addr = inet_addr("192.168.1.80");
    clientAddress.sin_port = htons(port);
	if(connect(sock,(sockaddr*)&clientAddress,sizeof(clientAddress))==SOCKET_ERROR){
        printf("建立连接失败！");
        
    }


	HANDLE SenThread = CreateThread(NULL, 0, ( LPTHREAD_START_ROUTINE )GetData,(LPVOID)NULL, 0, NULL);
    /* start receiving images */
    while(1) 
    {
		int i,j,k;
        /* get raw data */
		if(ImgOK){
			ImgOK=0;
			memcpy(imgdata,sockdata,imgsize);
			for (i = 0, k = 0; i < img->height; i++) {
				for (j = 0; j < img->width; j++) {
					((uchar*)(img->imageData + i * img->widthStep))[j] = sockdata[k++];
				}
			}

		}
		       
		 printf("%d\n",1111);
		cvShowImage("stream_client", img);
		if(waitKey(30) >= 0) break;
	}
	return 0;
}

