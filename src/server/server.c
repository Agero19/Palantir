#include "../../include/server.h"

pthread_mutex_t log_mutex;

int main(void) {
    int server_fd, *new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};

    log_message(INFO, "Server started");

    // Create a server file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        log_message(ERROR, "Socket failed");
        return EXIT_FAILURE;
    }
    log_message(INFO, "Socket created");

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
        log_message(ERROR, "Bind failed");
        return EXIT_FAILURE;
    }
    log_message(INFO, "Socket bound to port %d", PORT);

    // Start listening to connections
    if (listen(server_fd, 10) < 0) {
        log_message(ERROR, "Listen failed");
        return EXIT_FAILURE;
    }   

    log_message(INFO, "Listening for connections");
    printf("Server listening on port %d\n", PORT);

    // Main loop: accept incoming connections
    while(1) {
        new_socket = malloc(sizeof(int)); // Allocate memory for the new socket
        
        if ((*new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0) {
            log_message(ERROR, "Accept failed");
            free(new_socket);
            return EXIT_FAILURE;
        }

        log_message(INFO, "Accepted connection from client");

        //Create a new thread to handle the client
        pthread_t thread_id;
        if (pthread_create(&thread_id, NULL, handle_client, (void*)new_socket) != 0) {
            log_message(ERROR, "Failed to create thread");
            close(*new_socket);
            free(new_socket);
        } else {
            log_message(INFO, "Thread created to handle client");
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
    
    log_message(INFO, "Handling client %p connection in thread", (void*)pthread_self());
    printf("Client connected, thread ID: %p\n", (void*)pthread_self());

    // Read data from client in a loop
    while ((valread = read(client_socket, buffer, BUFFER_SIZE)) > 0) {
        // printf("Client %p: %s",(void*)pthread_self(), buffer);
        log_message(INFO, "Received from client %p: %s", (void*)pthread_self(), buffer);
        memset(buffer, 0, sizeof(buffer));
    }
    
    log_message(INFO, "Client disconnected");
    printf("Client disconnectedm thread ID: %p\n", (void*)pthread_self());

    log_message(INFO, "Closed connection with client");
    return NULL;
}

void log_message(LogLevel level, const char *format, ...) {
    pthread_mutex_lock(&log_mutex);
    
    FILE *log_file = fopen("/../../logs/server.log", "a");
    if (log_file == NULL) {
        pthread_mutex_unlock(&log_mutex);
        return;
    }

    time_t now = time(NULL);
    char *timestamp = ctime(&now);
    timestamp[strlen(timestamp) - 1] = '\0';

    switch (level) {
        case INFO:
            fprintf(log_file, "[%s] [INFO] ", timestamp);
            break;
        case WARNING:
            fprintf(log_file, "[%s] [WARNING] ", timestamp);
            break;
        case ERROR:
            fprintf(log_file, "[%s] [ERROR] ", timestamp);
            break;
    }

    va_list args;
    va_start(args, format);
    vfprintf(log_file, format, args);
    va_end(args);

    fprintf(log_file, "\n");
    fclose(log_file);

    pthread_mutex_unlock(&log_mutex);
}
