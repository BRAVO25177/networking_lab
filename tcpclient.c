#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main(void) {
    int socket_desc;
    struct sockaddr_in server_addr;
    char client_msg[2000], server_msg[2000];

    // Create socket
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc < 0) {
        printf("Unable to create socket\n");
        return -1;
    }

    // Set port and IP
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htons(3000);

    // Connect to server
    if (connect(socket_desc, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        printf("Unable to connect\n");
        close(socket_desc);
        return -1;
    }

    while (1) {
        // Clean buffers
        memset(client_msg, '\0', sizeof(client_msg));
        memset(server_msg, '\0', sizeof(server_msg));

        // Get input from user
        printf("Enter message: ");
        fgets(client_msg, sizeof(client_msg), stdin);
        // Remove newline character from the input
        client_msg[strcspn(client_msg, "\n")] = '\0';

        // Send message to server
        if (send(socket_desc, client_msg, strlen(client_msg), 0) < 0) {
            printf("Unable to send message\n");
            break;
        }

        // Receive message from server
        if (recv(socket_desc, server_msg, sizeof(server_msg), 0) < 0) {
            printf("Couldn't receive message\n");
            break;
        }

        printf("Message from server: %s\n", server_msg);

        // Exit if client types "exit"
        if (strcmp(client_msg, "exit") == 0) {
            printf("Exiting...\n");
            break;
        }
    }

    // Close the socket
    close(socket_desc);
    return 0;
}

