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

		struct timeval timeout;
		timeout.tv_sec = 5;  // 5 seconds
		timeout.tv_usec = 0;
		setsockopt(clientSocket, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));

		if (clientSocket < 0)
		{
			std::cout << "[ERR] Error accepting client connection." << std::endl;
			continue;
		}

		// Add the new client socket to the list safely
		{
			std::lock_guard<std::mutex> lock(muSocetList);
			clientSocketList.push_back(clientSocket);
			notProcesstList.push_back(clientSocket);
			condServer.notify_all();
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
		int nRecv = recv(clientSocket, buff, 50, 0);
		std::cout << "RECV: " << nRecv << std::endl;
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
					auto it = find(clientSocketList.begin(), clientSocketList.end(), clientSocket);
					// Checking if element is found or not
					if (it != clientSocketList.end())
					{
						notProcesstList.push_back(clientSocket);
						std::lock_guard<std::mutex> lock(muSocetList);
						clientSocketList.erase(it);
						close(clientSocket);
					}
				}
				break;
			}
		}
		else if(nRecv == 0)
		{
			// Remove the disconnected client socket to the list safely
			{
				auto it = find(clientSocketList.begin(), clientSocketList.end(), clientSocket);
				// Checking if element is found or not
				if (it != clientSocketList.end())
				{
					notProcesstList.push_back(clientSocket);
					std::lock_guard<std::mutex> lock(muSocetList);
					clientSocketList.erase(it);
					close(clientSocket);
				}
			}

			std::cout << "[ERR] client disconnected, fd = " << clientSocket << std::endl;

			// closing socket
			close(clientSocket);

			break;
		}
		else if((nRecv > 0) && (nRecv < 50))
		{
			break;
		}

		strClientMsg += buff;
	}

		std::cout << "client msg: " << strClientMsg << std::endl;

		// Broadcast the message to all the clients in the chatroom
		broadcastMsg(strClientMsg, clientSocket);

		auto it = find(clientSocketList.begin(), clientSocketList.end(), clientSocket);

		// Checking if element is found or not
		if (it != clientSocketList.end())
		{
			std::lock_guard<std::mutex> lock(muSocetList);
			notProcesstList.push_back(clientSocket);
			condServer.notify_all();
			std::cout << "[....] " << strClientMsg << std::endl;
		}
}

void Server::broadcastMsg(std::string strClientMsg, int currentclientSocket)
{
	int rSend;
	for (auto clientSocket : clientSocketList)
	{
		if (clientSocket == currentclientSocket)
		{
			continue;
		}

		rSend = send(clientSocket, strClientMsg.c_str(), strClientMsg.length(), 0);
		std::cout << "length: " << strClientMsg.length() << std::endl;

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
				std::cout << "[ERR] Can not broadcast message to , fd = " << clientSocket << std::endl;
				{
					auto it = find(clientSocketList.begin(), clientSocketList.end(), clientSocket);
					// Checking if element is found or not
					if (it != clientSocketList.end())
					{
						notProcesstList.push_back(clientSocket);
						std::lock_guard<std::mutex> lock(muSocetList);
						clientSocketList.erase(it);
						close(clientSocket);
					}
				}
				break;
			}
		}
	}

	std::cout << "[INFO] sent: " << strClientMsg << std::endl;
}

void Server::assignTask()
{
	std::function<void()> job;
	job = std::bind(&Server::checkConnection, this);
	threadPool.EnqueueJob(job);

	while(!shoudlTerminate)
	{
		std::unique_lock<std::mutex> lock(muSocetList);

		if(notProcesstList.empty())
		{
			condServer.wait(lock, [this] {return !notProcesstList.empty() || !shoudlTerminate;});
		}

		while (!notProcesstList.empty())
		{
			int clientSocket = notProcesstList.front();
			job = std::bind(&Server::handleClient, this, clientSocket);
			std::cout << "[INFO] assigned task for ... "  << clientSocket << std::endl;
			// notProcesstList.erase(std::find(notProcesstList.begin(), notProcesstList.end(), clientSocket));
			notProcesstList.erase(notProcesstList.begin());
			for(auto clientSocket1 : notProcesstList)
			{
				std::cout << "[TRC] left clinetsocket ID: "  << clientSocket1 << std::endl;
			}
			threadPool.EnqueueJob(job);
		}

		lock.unlock();
	}
}

