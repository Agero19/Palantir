#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main(void) {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};

    // Create a server file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        return EXIT_FAILURE;
    }

    // Set a socket options
    
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt SO_REUSEADDR failed");
        return EXIT_FAILURE;
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt)) < 0) {
        perror("setsockopt SO_REUSEPORT failed");
        return EXIT_FAILURE;
    }

    // Setup the server adress
    // Set the adress family to AF_INET (IPv4)
    address.sin_family = AF_INET;

    // Accept connections on any available interface 
    address.sin_addr.s_addr = INADDR_ANY;

    // Set the port number in network byte order / PORT 8080
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed!");
        return EXIT_FAILURE;
    }

    // Start listening to connections
    if (listen(server_fd, 3) < 0) {
        perror("listening filed");
        return EXIT_FAILURE;
    }   
    printf("Server listening on port %d\n", PORT);

    // Accepting incomming connections
    if ((new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0) {
        perror("Accept failed");
        return EXIT_FAILURE;
    }
    printf("Connection established\n");

    //Read the data from client and print it out
    // Declare a signed size type variable for the number of bytes read
    size_t valread;
    while ((valread = read(new_socket, buffer, BUFFER_SIZE)) > 0) {
        printf("Client: %s", buffer);
        memset(buffer, 0, sizeof(buffer));
    }

    // Close the socket
    close(server_fd);
    return EXIT_SUCCESS;
}