#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

int main()
{
    WSADATA wsa;
    SOCKET socket_;
    struct sockaddr_in server;
    char *message;
    char server_reply[200];

    // Initialize Winsock
    printf("\nInitializing Winsock...");
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
        printf("Failed. Error Code : %d", WSAGetLastError());
        return 1;
    }
    printf("Initialized.\n");

    // Create a socket
    if ((socket_ = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
    {
        printf("Could not create socket : %d", WSAGetLastError());
    }
    printf("Socket created.\n");

    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons(8888);

    // Connect to remote server

    if (connect(socket_, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        puts("connect error");
        return 1;
    }
    puts("Connected");

    // Game loop
    while (1)
    {
        printf("Enter your choice (rock, paper, scissors): ");
        char choice[10];
        scanf("%s", choice);
        if(strcmp(choice, "rock") != 0 && strcmp(choice, "scissors") != 0 && strcmp(choice, "paper") != 0){
            printf("\nInvalid Input! Enter rock, paper or scissors!\n");
            continue;
        }

        // Send choice to server
        send(socket_, choice, strlen(choice), 0);

        // Receive result from server
        int recv_size = recv(socket_, server_reply, sizeof(server_reply), 0);
        server_reply[recv_size] = '\0';
        printf("\nResult: %s\n", server_reply);

        // Receive and display current scorecard
        //recv_size = recv(socket_, server_reply, 2000, 0);
        recv(socket_, server_reply, sizeof(server_reply), 0);
        //server_reply[recv_size] = '\0';
        //printf("Current Score: %s\n", server_reply);

        // Check for game over
        if (strcmp(server_reply, "Client has won the game") == 0 ){
            printf("\nClient has won the game by getting to 5 points first!");
            break;
        }

        if(strcmp(server_reply, "Server has won the game") == 0){
            printf("\nServer has won the game by getting to 5 points first!");
            break;
        }

        printf("Current Score: %s\n", server_reply);
   
    }

    // Close the socket and cleanup
    printf("\n\n***CLIENT CLOSING***\n");
    closesocket(socket_);
    WSACleanup();

    return 0;
}
