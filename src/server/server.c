#include "../../include/server.h"

int main(void) {
    int server_fd, *new_socket;
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

    // Setup the server address
    // Set the adress family to AF_INET (IPv4)
    address.sin_family = AF_INET;

    // Accept connections on any available interface 
    address.sin_addr.s_addr = INADDR_ANY;

    // Set the port number in network byte order / PORT 8080
    address.sin_port = htons(PORT);

    // Bind the socket to the specified port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed!");
        return EXIT_FAILURE;
    }

    // Start listening to connections
    if (listen(server_fd, 10) < 0) {
        perror("listening filed");
        return EXIT_FAILURE;
    }   
    printf("Server listening on port %d\n", PORT);

    // Main loop: accept incoming connections
    while(1) {
        new_socket = malloc(sizeof(int)); // Allocate memory for the new socket
        
        if ((*new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0) {
            perror("Accept failed");
            free(new_socket);
            return EXIT_FAILURE;
        }

        //Create a new thread to handle the client
        pthread_t thread_id;
        if (pthread_create(&thread_id, NULL, handle_client, (void*)new_socket) != 0) {
            perror("Failed to create a thread");
            close(*new_socket);
            free(new_socket);
        }

        // Detatch the thread to allow it to clean up after finishing
        pthread_detach(thread_id);
    }

    // Close the socket
    close(server_fd);
    return EXIT_SUCCESS;
}

void *handle_client(void *arg) {
    int client_socket = *(int*)arg;
    free(arg); // Free the dynamically allocated memory
    char buffer[BUFFER_SIZE];
    size_t valread;

    printf("Client connected, thread ID: %p\n", (void*)pthread_self());

    // Read data from client in a loop
    while ((valread = read(client_socket, buffer, BUFFER_SIZE)) > 0) {
        printf("Client %p: %s",(void*)pthread_self(), buffer);
        memset(buffer, 0, sizeof(buffer));
    }

    printf("Client disconnectedm thread ID: %p\n", (void*)pthread_self());
    return NULL;
}