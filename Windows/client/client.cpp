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
	SetConsoleTitle(L"client");
	cout << "Please input the IP address of the server: ";
	string ipStr;
	getline(cin, ipStr);
	const char* serverIp = ipStr.c_str();
	cout << "Please input the port number of the server: ";
	string portStr;
	getline(cin, portStr);
	unsigned short portNum = atoi(portStr.c_str());
	printf("Connecting to %s:%d ...\n", serverIp, portNum);

	WORD sockVersion = MAKEWORD(2, 2);
	WSADATA data;
	if (WSAStartup(sockVersion, &data) != 0)
	{
		return 0;
	}

	while (true){
		ShowTheCursor();

		SOCKET sClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (sClient == INVALID_SOCKET)
		{
			printf("invalid socket!");
			return 0;
		}

		sockaddr_in serAddr;
		serAddr.sin_family = AF_INET;
		serAddr.sin_port = htons(portNum);
		serAddr.sin_addr.S_un.S_addr = inet_addr(serverIp);
		if (connect(sClient, (sockaddr *)&serAddr, sizeof(serAddr)) == SOCKET_ERROR)
		{  //连接失败   
			printf("Connect failed! 请先打开服务端并确保网络畅通\r\n");
			closesocket(sClient);
			return 0;
		}
		//system("cls");
		SetColor(14); printf("----- Connected with %s -----\r\n", serverIp);

		//发送数据
		PrintColorText(13, "sendData: ");
		string sendData;
		cin >> sendData;
		send(sClient, sendData.c_str(), strlen(sendData.c_str()), 0); 
		
		//接收数据
		PrintColorText(8, "Waiting for data...");
		HideTheCursor();
		
		char recvData[1024];
		int ret = recv(sClient, recvData, 1024, 0);
		if (ret>0){
			recvData[ret] = 0x00;
			ClearCurrentLine();
			SetColor(12); printf("recvData: %s\r\n\r\n", recvData);
		}
		closesocket(sClient);
		//Sleep(1000);
	}

	WSACleanup();
	return 0;
}