#include<string.h>    //strlen
#include<sys/socket.h>    //socket
#include<arpa/inet.h> //inet_addr
#include <unistd.h>
#include <stdio.h>
#include <iostream>
 
int main(int argc , char *argv[])
{
    int sock;
    struct sockaddr_in server;
    char message[1000] , server_reply[2000];
     
    //Create socket
    sock = socket(AF_INET , SOCK_STREAM , 0);
    if (sock == -1)
    {
        std::cout << "Could not create socket" << std::endl;
    }
    std::cout << "Socket created" << std::endl;
     
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons( 8888 );
 
    //Connect to remote server
    if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        perror("connect failed. Error");
        return 1;
    }
     
    std::cout << "Connected\n";
     
    //keep communicating with server
    while(1)
    {
        std::cout << "Enter message : ";
        std::cin >> message;
         
        //Send some data
        if( send(sock , message , strlen(message) , 0) < 0)
        {
            std::cout << "Send failed\n";
            return 1;
        }
         
        //Receive a reply from the server
        if( recv(sock , server_reply , 2000 , 0) < 0)
        {
            std::cout << "recv failed\n";
            break;
        }
         
        std::cout << "Server reply :" << server_reply << std::endl;
	memset(server_reply,0,1999);
    }
     
    close(sock);
    return 0;
}
