#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <winsock2.h>  
#include <stdio.h>      
#include <iostream>  
#include <cstring>  
#include <string>
#include "m_console.h"

#pragma comment(lib, "ws2_32.lib")    

using namespace std;

int main(int argc, char* argv[])
{
	SetConsoleTitle(L"server");
	cout << "Please input the port number of the server: ";
	string portStr;
	getline(cin, portStr);
	unsigned short portNum = atoi(portStr.c_str());

	//初始化WSA    
	WORD sockVersion = MAKEWORD(2, 2);
	WSADATA wsaData;
	if (WSAStartup(sockVersion, &wsaData) != 0)
	{
		return 0;
	}

	//创建套接字    
	SOCKET sListen = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sListen == INVALID_SOCKET)
	{
		printf("socket error !\r\n");
		return 0;
	}

	//绑定IP和端口    
	sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(portNum);
	sin.sin_addr.S_un.S_addr = INADDR_ANY;
	if (bind(sListen, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR)
	{
		printf("bind error !\r\n");
	}

	//开始监听    
	if (listen(sListen, 5) == SOCKET_ERROR)
	{
		printf("listen error !\r\n");
		return 0;
	}

	//循环接收数据    
	SOCKET sClient;
	sockaddr_in remoteAddr;
	int nAddrlen = sizeof(remoteAddr);
	
	while (true)
	{
		//system("cls");
		PrintColorText(8, "Waiting for connection...");
		sClient = accept(sListen, (SOCKADDR *)&remoteAddr, &nAddrlen);
		if (sClient == INVALID_SOCKET)
		{
			printf("accept error !\r\n");
			continue;
		}
		ClearCurrentLine();
		SetColor(14); printf("----- Connected with %s -----\r\n", inet_ntoa(remoteAddr.sin_addr));
		
		//接收数据   
		PrintColorText(8, "Waiting for data...");
		HideTheCursor();
		char recvData[1024];
		int ret = recv(sClient, recvData, 1024, 0);
		if (ret > 0)
		{
			recvData[ret] = 0x00;
			ClearCurrentLine();
			SetColor(12); printf("recvData: %s\r\n", recvData);
		}

		//发送数据
		PrintColorText(13, "sendData: ");
		ShowTheCursor();
		string sendData;
		cin >> sendData;
		send(sClient, sendData.c_str(), strlen(sendData.c_str()), 0);
		closesocket(sClient);
		printf("\r\n");
		//Sleep(1000);
	}
	
	closesocket(sListen);
	WSACleanup();

	return 0;
}