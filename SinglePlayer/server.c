#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

int main()
{
    WSADATA wsa;
    SOCKET server_socket, client_socket;
    struct sockaddr_in server, client;
    int c;

    // Initialize Winsock
    printf("\nInitializing Winsock...");
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
        printf("Failed. Error Code : %d", WSAGetLastError());
        return 1;
    }
    printf("Initialized.\n");

    // Create a socket
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
    {
        printf("Could not create socket : %d", WSAGetLastError());
    }
    printf("Socket created.\n");

    // Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(8888);

    // Bind
    if (bind(server_socket, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR)
    {
        printf("Bind failed with error code : %d", WSAGetLastError());
    }
    puts("Bind done");

    // Listen to incoming connections
    listen(server_socket, 3);

    // Accept and incoming connection
    puts("Waiting for incoming connections...");

    c = sizeof(struct sockaddr_in);
    client_socket = accept(server_socket, (struct sockaddr *)&client, &c);
    if (client_socket == INVALID_SOCKET)
    {
        printf("accept failed with error code : %d", WSAGetLastError());
    }
    puts("Connection accepted");

    // Game loop
    int client_score = 0, server_score = 0;
    while (1)
    {
        char client_choice[10], server_choice[10];

        // Receive client's choice
        recv(client_socket, client_choice, 10, 0);

        // Generate random server choice
        int r = rand() % 3;
        if (r == 0)
            strcpy(server_choice, "rock");
        else if (r == 1)
            strcpy(server_choice, "paper");
        else
            strcpy(server_choice, "scissors");

        // Determine winner and update scores
        if (strcmp(client_choice, "rock") == 0)
        {
            if (strcmp(server_choice, "rock") == 0)
                send(client_socket, "Tie", strlen("Tie"), 0);
            else if (strcmp(server_choice, "paper") == 0)
            {
                send(client_socket, "Server", strlen("Server"), 0);
                server_score++;
            }
            else
            {
                send(client_socket, "Client", strlen("Client"), 0);
                client_score++;
            }
        }
        else if (strcmp(client_choice, "paper") == 0)
        {
            if (strcmp(server_choice, "rock") == 0)
            {
                send(client_socket, "Client", strlen("Client"), 0);
                client_score++;
            }
            else if (strcmp(server_choice, "paper") == 0)
                send(client_socket, "Tie", strlen("Tie"), 0);
            else
            {
                send(client_socket, "Server", strlen("Server"), 0);
                server_score++;
            }
        }
        else
        {
            if (strcmp(server_choice, "rock") == 0)
            {
                send(client_socket, "Server", strlen("Server"), 0);
                server_score++;
            }
            else if (strcmp(server_choice, "paper") == 0)
            {
                send(client_socket, "Client", strlen("Client"), 0);
                client_score++;
            }
            else
                send(client_socket, "Tie", strlen("Tie"), 0);
        }

        // Send current scorecard to client
        char score[50];
        sprintf(score, "Client: %d, Server: %d", client_score, server_score);
        send(client_socket, score, strlen(score), 0);

        // Check for winner
        if (client_score == 5)
        {
            send(client_socket, "Client wins!", strlen("Client wins!"), 0);
            break;
        }
        else if (server_score == 5)
        {
            send(client_socket, "Server wins!", strlen("Server wins!"), 0);
            break;
        }
    }

    // Close the socket and cleanup
    closesocket(client_socket);
    WSACleanup();

    return 0;
}
