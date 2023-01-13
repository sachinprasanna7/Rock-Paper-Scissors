#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

int main(){
    WSADATA wsa;
    SOCKET server_socket, client_socket1, client_socket2;
    struct sockaddr_in server, client1, client2;

    printf("Initializing Winsock...\n");
    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) {
        printf("Failed. Error Code : %d", WSAGetLastError());
        return 1;
    }
    printf("Initialized.\n");

    // Create a socket
    if((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("Could not create socket : %d", WSAGetLastError());
    }
    printf("Socket created.\n");

    // Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(8888);

    // Bind the socket
    if(bind(server_socket, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR) {
        printf("Bind failed with error code : %d", WSAGetLastError());
        exit(EXIT_FAILURE);
    }
    printf("Bind done.\n");

    // Listen to incoming connections
    listen(server_socket, 3);

    // Accept and incoming connection
    printf("Waiting for incoming connections...\n");
    int c = sizeof(struct sockaddr_in);
    client_socket1 = accept(server_socket, (struct sockaddr *)&client1, &c);
    printf("Connection accepted from client 1.\n");
    client_socket2 = accept(server_socket, (struct sockaddr *)&client2, &c);
    printf("Connection accepted from client 2.\n");

    int player1_points = 0, player2_points = 0;
    char player1_move[10], player2_move[10];

    while (1) {
        recv(client_socket1, player1_move, 10, 0);
        recv(client_socket2, player2_move, 10, 0);
        printf("Player 1 chose: %s\nPlayer 2 chose: %s\n", player1_move, player2_move);

        if (strcmp(player1_move, "rock") == 0) {
            if (strcmp(player2_move, "scissors") == 0) {
                printf("Player 1 wins!\n");
                player1_points++;
            } else if (strcmp(player2_move, "paper") == 0) {
                printf("Player 2 wins!\n");
                player2_points++;
            } else {
                printf("Tie!\n");
            }
        } else if (strcmp(player1_move, "scissors") == 0) {
if (strcmp(player2_move, "paper") == 0) {
printf("Player 1 wins!\n");
player1_points++;
} else if (strcmp(player2_move, "rock") == 0) {
printf("Player 2 wins!\n");
player2_points++;
} else {
printf("Tie!\n");
}
} else if (strcmp(player1_move, "paper") == 0) {
if (strcmp(player2_move, "rock") == 0) {
printf("Player 1 wins!\n");
player1_points++;
} else if (strcmp(player2_move, "scissors") == 0) {
printf("Player 2 wins!\n");
player2_points++;
} else {
printf("Tie!\n");
}
}

if (player1_points == 5) {
        printf("Player 1 wins the game!\n");
        break;
    } else if (player2_points == 5) {
        printf("Player 2 wins the game!\n");
        break;
    }
}

closesocket(client_socket1);
closesocket(client_socket2);
WSACleanup();
return 0;
}
