#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <thread>

#include "client.h"



#define MAX_LEN 256

void Client::InitClient()
{
    // creating socket
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    // specifying address
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    // sending connection request
    connect(clientSocket, (struct sockaddr*)&serverAddress,
            sizeof(serverAddress));
}

void Client::eraseText(int cnt)
{
	char back_space=8;
	for(int i=0; i<cnt; i++)
	{
		std::cout<<back_space;
	}
}

void Client::sendMessage(int clientSocket)
{
	while(1)
	{
		std::cout <<"You : ";
		char str[MAX_LEN];
		std::cin.getline(str,MAX_LEN);
		send(clientSocket,str,sizeof(str),0);
		// if(strcmp(str,"#exit")==0)
		// {
		// 	exit_flag=true;
		// 	t_recv.detach();
		// 	close(clientSocket);
		// 	return;
		// }
	}
}

// Receive message
void Client::recvMessage(int client_socket)
{
	while(1)
	{
		// if(exit_flag)
		// 	return;
		char name[MAX_LEN], str[MAX_LEN];
		// int bytes_received = recv(client_socket, name, sizeof(name), 0);
		// if(bytes_received <= 0)
		// 	continue;

		recv(client_socket, str, sizeof(str), 0);
		// eraseText(6);
		// if(strcmp(name,"#NULL") != 0)
		// std::cout << name <<" : " << str << std::endl;
		// else
			// std::cout<<color(color_code)<<str<<endl;

        std::cout << str << std::endl;
		// std::cout << "You : ";
		fflush(stdout);
	}
}
