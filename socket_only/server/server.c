// Server side
// (Accept connection, receive file, close connection)

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <libgen.h> // For basename function

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE];

    // Create socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind socket to localhost:PORT
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    // Accept incoming connection
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }

    // Receive file path from client
    ssize_t path_length = recv(new_socket, buffer, BUFFER_SIZE, 0);
    if (path_length <= 0) {
        perror("Failed to receive file path");
        exit(EXIT_FAILURE);
    }
    buffer[path_length] = '\0'; // Null terminate the file path

    // Extract filename from the path
    char *filename = basename(buffer);
    printf("Received : %s\n", filename);

    // Open file with the received file name for writing
    FILE *file = fopen(filename, "wb");
    if (!file) {
        perror("File creation failed");
        exit(EXIT_FAILURE);
    }

    // Receive and write file content
    ssize_t bytes_received;
    while ((bytes_received = recv(new_socket, buffer, BUFFER_SIZE, 0)) > 0) {
        fwrite(buffer, 1, bytes_received, file);
    }
    if (bytes_received < 0) {
        perror("Receive failed");
        exit(EXIT_FAILURE);
    }

    // Close file and connection
    fclose(file);
    close(new_socket);
    close(server_fd);
    return 0;
}
