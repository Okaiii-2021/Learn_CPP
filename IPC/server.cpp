#include "server.h"
#include <algorithm> // use in here to avoid conflict


bool Server::InitServerSocket()
{
	// Creating socket file descriptor
	if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == 0)
	{
	std::cout << "Socket failed" << std::endl;
	return -1;
	}

	std::cout << "[INFO] Initing server socket ... \n";

	// Specify the address
	sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(8080);
	serverAddress.sin_addr.s_addr = INADDR_ANY;

	// binding socket.
	bind(serverSocket, (struct sockaddr*)&serverAddress,
			sizeof(serverAddress));

	// listening to the assigned socket
	listen(serverSocket, 5);

	std::cout << "[INFO] Init server socket sucessfully!!!" << " Socket Server: " <<  serverSocket << std::endl;

	return true;
}

void Server::checkConnection()
{
	while((!shoudlTerminate))
	{
		struct sockaddr_in clientAddr;
		socklen_t clientLen = sizeof(clientAddr);
		int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientLen);

		if (clientSocket < 0)
		{
			std::cout << "[ERR] Error accepting client connection." << std::endl;
			continue;
		}

		// Add the new client socket to the list safely
		{
			std::lock_guard<std::mutex> lock(muSocetList);
			clientSocketList.push_back(clientSocket);
			std::cout << "New client connected. Socket: " << clientSocket << std::endl;
		}
	}
}

void Server::handleClient(int clientSocket)
{
	std::string strClientMsg;
	char buff[256];

	while(1)
	{
		std::memset(buff, 0, sizeof(buff));
		int nRecv = recv(clientSocket, buff, 256, 0);
		if(nRecv == -1)
		{
			if(errno == EWOULDBLOCK)
			{
				break;
			}
			else
			{
				std::cout << "[ERR] recv error, client disconnected, fd = " << clientSocket << std::endl;

				// Remove the disconnected client socket to the list safely
				{
					std::lock_guard<std::mutex> lock(muSocetList);
					// auto first = clientSocketList.begin();
    				// auto last = clientSocketList.end();
					clientSocketList.erase(std::find(clientSocketList.begin(), clientSocketList.end(), clientSocket));

					// closing socket
    				close(clientSocket);
				}
				break;
			}
		}
		else if(nRecv == 0)
		{
			// Remove the disconnected client socket to the list safely
			{
				std::lock_guard<std::mutex> lock(muSocetList);
				auto first = clientSocketList.begin();
				auto last = clientSocketList.end();
				clientSocketList.erase(std::find(first, last, clientSocket));
			}

			std::cout << "[ERR] client disconnected, fd = " << clientSocket << std::endl;

			// closing socket
			close(clientSocket);

			break;
		}

		strClientMsg = buff;

		std::cout << "client msg: " << strClientMsg;

		// Broadcast the message to all the clients in the chatroom
		broadcastMsg(strClientMsg);
	}


}

void Server::broadcastMsg(std::string strClientMsg)
{
	int rSend;
	for (auto clientSocket : clientSocketList)
	{
		rSend = send(clientSocket, strClientMsg.c_str(), strClientMsg.length(), 0);

		if(rSend == -1)
		{
			if(errno == EWOULDBLOCK)
			{
				// wait for socket to clean for more data to be sent
				sleep(10);
				continue;
			}
			else
			{
				std::cout << "send error, fd = " << clientSocket << std::endl;
				{
					std::lock_guard<std::mutex> lock(muSocetList);
					auto first = clientSocketList.begin();
					auto last = clientSocketList.end();
					clientSocketList.erase(std::find(first, last, clientSocket));
				}
				break;
			}
		}
	}

	std::cout << "[INFO] send: " << strClientMsg << std::endl;
}

void Server::assignTask()
{
	std::function<void()> job;
	job = std::bind(&Server::checkConnection, this);
	threadPool.EnqueueJob(job);

	while(1)
	{
		std::unique_lock<std::mutex> lock(muSocetList);

		if(clientSocketList.empty())
		{
			condServer.wait(lock, [this] {return !clientSocketList.empty() || shoudlTerminate;});
		}

		for (auto clientSocket : clientSocketList)
		{
			// std::cout << "assignTask...." << std::endl;
			job = std::bind(&Server::handleClient, this, clientSocket);
			threadPool.EnqueueJob(job);
		}

		lock.unlock();
	}
}

