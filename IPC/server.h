#ifndef SERVER_H
#define SERVER_H

#include <mutex>
#include <condition_variable>
#include <functional>
#include <queue>
#include <thread>
#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <algorithm>  // Add this to the includes
#include <fcntl.h>

#include "thread_pool.cpp"
// Currently support 1 severver and multiple clients

// [?] why we need to use this in job = std::bind(checkConnection, this);

class Server
{
    private:
        int serverSocket;
        std::vector<int> clientSocketList;
        std::vector<int> notProcesstList;
        std::mutex muSocetList;
        std::condition_variable condServer;
        bool shoudlTerminate;
        std::vector<std::thread> threads;

        ThreadPool threadPool;

    public:
        Server()
        {
            // Start threadpool
            threadPool.Start(5);

            // Init server
            InitServerSocket();

            // 1 thread for assign task
            threads.push_back(std::thread(&Server::assignTask, this));
        }

        ~Server()
        {
            for(auto i = 0; i < threads.size(); i++)
            {
                threads[i].join();
            }
            threads.clear();

            // closing the socket.
            close(serverSocket);
        }

        bool InitServerSocket();
        bool acceptConnection();
        void checkConnection();
        void handleClient(int clientSocket);
        void assignTask();
        void broadcastMsg(std::string strClientMsg, int currentclientSocket);
};

#endif // SERVER_H