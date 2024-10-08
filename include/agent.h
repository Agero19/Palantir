#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <time.h>
#include <math.h>
#include <unistd.h>

#include <mach/mach.h>
#include <sys/types.h>
#include <sys/sysctl.h>
#include <sys/mount.h>
#include <signal.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <cjson/cJSON.h>

#define PATH "/"

// Round to n decimal point a given float
extern float round_to_places(float value, int places);

// Convertion function bytes to gigabytes
extern float bytes_to_gb(uint64_t bytes); 

// Call to system api to gather cpu usage in idle percents
extern float get_cpu_usage();

// Get a page size for current system to calculate memory in bytes
extern uint64_t get_page_size();

// Call to sytem api to get free memory remainder and total memory on system in bytes
extern void get_memory_usage(uint64_t *total_mem, uint64_t *free_mem);

// Call to system api to get free disk space in bytes and total disk space
extern void get_disk_usage(const char *path, uint64_t *total_disk, uint64_t *used_disk);

// Get network usage info by runing netstat utility to get bytes-in and bytes-out on current network
extern void get_network_usage (uint64_t *bytes_in, uint64_t *bytes_out);

// Forms a JSON formated structure to save gathered data about recources
// Passed data should be in bytes for valid calculations
// @param console_log boolean affects printing report to std-out on true
extern void create_resource_json(
    float cpu_usage,
    uint64_t total_mem,
    uint64_t free_mem, 
    uint64_t total_disk, 
    uint64_t used_disk, 
    uint64_t bytes_in, 
    uint64_t bytes_out,
    bool console_log
);

// Creates a background process, by forking and then setting separate session to this child process to be a leader of the session, set up redirect of stdfiles
extern void daemonize();

// Catches SIGNINT or SIGNTERM to gracefully shutdown a process
extern void handle_signal(int signal);

// Main runnning loop of agent module, takes no argument
// Creates a daemon to run a background process
// Sends data in defined intervals
//TODO: set up a link to config.json not to hardcode the interval. 
extern void run_agent();
