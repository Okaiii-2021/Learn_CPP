#include <thread>
#include <sys/socket.h>


class Client
{
    private:
        int clientSocket;
        std::thread t_send, t_recv;

    public:
        ~Client()
        {
            close(clientSocket);
        }

        Client()
        {
            InitClient();

            t_send = std::thread(&Client::sendMessage, this, clientSocket);
            t_recv = std::thread(&Client::recvMessage, this, clientSocket);

            if(t_send.joinable())
                t_send.join();
            if(t_recv.joinable())
                t_recv.join();
        }

        void InitClient();
        void eraseText(int num);
        void sendMessage(int client_socket);
        void recvMessage(int client_socket);
};