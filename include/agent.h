#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <unistd.h>
#include <mach/mach.h>
#include <sys/types.h>
#include <sys/sysctl.h>
#include <sys/mount.h>
#include <cjson/cJSON.h>

#define PATH "/"

extern float round_to_places(float value, int places);

extern float bytes_to_gb(uint64_t bytes); 

extern float get_cpu_usage();

extern uint64_t get_page_size();

extern void get_memory_usage(uint64_t *total_mem, uint64_t *free_mem);

extern void get_disk_usage(const char *path, uint64_t *total_disk, uint64_t *used_disk);

extern void get_network_usage (uint64_t *bytes_in, uint64_t *bytes_out);

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

