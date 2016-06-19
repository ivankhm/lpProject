#include<stdio.h>
#include<string.h>    //strlen
#include<stdlib.h>    //strlen
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //write
#include<pthread.h> //for threading , link with lpthread
#include <iostream> 
//the thread function
void *connection_handler(void *);
 
int main(int argc , char *argv[])
{
    int socket_desc , client_sock , c , *new_sock;
    struct sockaddr_in server , client;
     
    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        std::cout << "Could not create socket" << std::endl;;
    }
    std::cout << "Socket created" << std::endl;
     
    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( 8888 );
     
    //Bind
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        //print the error message
        perror("bind failed. Error");
        return 1;
    }
    std::cout << "Bind done\n";
     
    //Listen
    listen(socket_desc , 3);
     
    //Accept and incoming connection
    std::cout << "Waiting for incoming connections...\n";
    c = sizeof(struct sockaddr_in);
     
     
    //Accept and incoming connection
    //("Waiting for incoming connections...");
    //c = sizeof(struct sockaddr_in);
    while( (client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)) )
    {
        std::cout << "Connection accepted" << std::endl;
         
        pthread_t sniffer_thread;
        new_sock = new int;
        *new_sock = client_sock;
         
        if( pthread_create( &sniffer_thread , NULL ,  connection_handler , (void*) new_sock) < 0)
        {
            perror("could not create thread");
            return 1;
        }
         
        std::cout << "Handler assigned\n";
    }
     
    if (client_sock < 0)
    {
        perror("accept failed");
        return 1;
    }
     
    return 0;
}
 

void *connection_handler(void *socket_desc)
{
    //Get the socket descriptor
    int sock = *(int*)socket_desc;
    int read_size;
    char message[256] , client_message[2000];
     
    //Receive a message from client
    while( (read_size = recv(sock , client_message , 2000 , 0)) > 0 )
    {
        //Send message
        write(sock , client_message , strlen(client_message));
    }
     
    if(read_size == 0)
    {
        std::cout << "Client disconnected" << std::endl;;
        fflush(stdout);
    }
    else if(read_size == -1)
    {
        perror("recv failed");
    }
         
    //Free the socket pointer
    delete[] reinterpret_cast<int *>(socket_desc);
     
    return 0;
}
