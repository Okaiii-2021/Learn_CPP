// C++ program to show the example of server application in
// socket programming
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

using namespace std;

int main()
{
    // creating socket
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    // specifying the address
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    // binding socket.
    bind(serverSocket, (struct sockaddr*)&serverAddress,
         sizeof(serverAddress));

    // listening to the assigned socket
    listen(serverSocket, 5);

    // accepting connection request
    int clientSocket
        = accept(serverSocket, nullptr, nullptr);
    std::string strClientMsg;

    // recieving data
    char buff[256] = { 0 };
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
				break;
			}
		}
		else if(nRecv == 0)
		{
			break;
		}

		strClientMsg = buff;
        std::cout << "client msg: " << strClientMsg;
		// strClientMsg = std::string(buff, nRecv);
	}

	

    // closing the socket.
    close(serverSocket);

    return 0;
}
