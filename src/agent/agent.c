#include "../../include/agent.h"

int main(void) {
    float cpu_usage = get_cpu_usage();
    uint64_t total_mem = 0, free_mem = 0;
    uint64_t total_disk = 0, used_disk = 0;
    uint64_t bytes_in = 0, bytes_out = 0;

    printf("CPU percanage of idle time %.2f \n", cpu_usage);

    return EXIT_SUCCESS;
}

float get_cpu_usage() {
    host_cpu_load_info_data_t cpu_info;
    mach_msg_type_number_t count = HOST_CPU_LOAD_INFO_COUNT;
    
    kern_return_t kr = host_statistics(
        mach_host_self(), 
        HOST_CPU_LOAD_INFO, 
        (host_info_t)&cpu_info, &count
    );

    if (kr != KERN_SUCCESS) {
        return EXIT_FAILURE;
    }

    uint64_t total_ticks = 0;
    for (int i = 0; i < CPU_STATE_MAX; i++) {
        total_ticks += cpu_info.cpu_ticks[i];
    }

    return (float)(cpu_info.cpu_ticks[CPU_STATE_IDLE]) / total_ticks * 100.0;
}