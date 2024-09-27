#include <stdio.h>
#include <stdlib.h>
#include <mach/mach.h>
#include <sys/types.h>
#include <sys/sysctl.h>
#include <sys/mount.h>
#include <cjson/cJSON.h>

#define PATH "/"


extern float get_cpu_usage();

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
    uint64_t bytes_out
);

