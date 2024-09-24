#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <stdarg.h>
#include <time.h>

#define PORT 8080
#define BUFFER_SIZE 1024

typedef enum { INFO, WARNING, ERROR } LogLevel;

extern void *handle_client(void *arg);

extern void log_message(LogLevel level, const char *format, ...);