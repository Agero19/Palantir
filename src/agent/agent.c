#include "../../include/agent.h"

int main(void) {
    float cpu_usage = get_cpu_usage();
    uint64_t total_mem = 0, free_mem = 0;
    uint64_t total_disk = 0, used_disk = 0;
    uint64_t bytes_in = 0, bytes_out = 0;

    get_memory_usage(&total_mem, &free_mem);
    get_disk_usage("/", &total_disk, &used_disk);
    get_network_usage(&bytes_in, &bytes_out);  // Capture network usage

    // Create JSON and print it
    create_resource_json(cpu_usage, total_mem, free_mem, total_disk, used_disk, bytes_in, bytes_out);


    return EXIT_SUCCESS;
}

float bytes_to_gb(uint64_t bytes) {
    return (float)bytes / (1024.0 * 1024.0 * 1024.0);  // Convert bytes to GB
}

float round_to_places(float value, int places) {
    float scale = pow(10.0, places);
    return roundf(value * scale) / scale;
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

uint64_t get_page_size() {
    return (uint64_t)sysconf(_SC_PAGESIZE);  // Typically 4096 bytes on macOS
}

void get_memory_usage(uint64_t *total_mem, uint64_t *free_mem) {
    mach_port_t host = mach_host_self();
    vm_statistics64_data_t vm_stats;
    mach_msg_type_number_t count = HOST_VM_INFO64_COUNT;

    if (host_statistics64(host, HOST_VM_INFO64, (host_info64_t)&vm_stats, &count) != KERN_SUCCESS) {
        return;
    }

    // Get the page size (typically 4096 bytes)
    uint64_t page_size = get_page_size();

    // Calculate total memory (this is physical memory)
    *total_mem = (vm_stats.wire_count + vm_stats.active_count + vm_stats.inactive_count + vm_stats.free_count) * page_size;

    // Calculate free memory
    *free_mem = vm_stats.free_count * page_size;
}

void get_disk_usage(const char* path, uint64_t* total_disk, uint64_t* used_disk) {
    struct statfs stats;

    if (statfs(path, &stats) == 0) {
        *total_disk = stats.f_blocks * stats.f_bsize;
        *used_disk = (stats.f_blocks - stats.f_bfree) * stats.f_bsize;
    }
}

void get_network_usage(uint64_t *bytes_in, uint64_t *bytes_out) {
    FILE *fp;
    char buffer[128];
    char iface[16];
    uint64_t ibytes, obytes;

    // Open the process with popen, executing the netstat command
    fp = popen("netstat -ib | awk '/en0/ {print $7, $10}'", "r");
    if (fp == NULL) {
        perror("popen failed");
        return;
    }

    // Read the output of the netstat command (assuming it's formatted correctly)
    if (fgets(buffer, sizeof(buffer), fp) != NULL) {
        sscanf(buffer, "%llu %llu", &ibytes, &obytes);
        *bytes_in = ibytes;
        *bytes_out = obytes;
    }

    pclose(fp);
}

void create_resource_json(
    float cpu_usage, 
    uint64_t total_mem, 
    uint64_t free_mem, 
    uint64_t total_disk, 
    uint64_t used_disk, 
    uint64_t bytes_in, 
    uint64_t bytes_out
) {
    cJSON *root = cJSON_CreateObject();
    
    // Conversion ni more readble format
    float cpu_idle = 100.0 - cpu_usage;
    float network_in_mb = bytes_in / (1024.0 * 1024.0 * 1024.0);  // Bytes to MB
    float network_out_mb = bytes_out / (1024.0 * 1024.0 * 1024.0); // Bytes to MB

    // Buffers to hold formatted strings for floating-point values
    char buffer[64];

    // Add CPU usage with 3 decimal places
    sprintf(buffer, "%.3f", cpu_idle);
    cJSON_AddStringToObject(root, "cpu_usage_percent", buffer);

    sprintf(buffer, "%.3f", cpu_usage);
    cJSON_AddStringToObject(root, "cpu_idle_percent", buffer);

    // Add memory usage
    sprintf(buffer, "%.3f", bytes_to_gb(total_mem));
    cJSON_AddStringToObject(root, "total_memory_gb", buffer);

    sprintf(buffer, "%.3f", bytes_to_gb(free_mem));
    cJSON_AddStringToObject(root, "free_memory_gb", buffer);

    // Add disk usage
    sprintf(buffer, "%.3f", bytes_to_gb(total_disk));
    cJSON_AddStringToObject(root, "total_disk_gb", buffer);

    sprintf(buffer, "%.3f", bytes_to_gb(used_disk));
    cJSON_AddStringToObject(root, "used_disk_gb", buffer);

    // Add network usage
    sprintf(buffer, "%.3f", network_in_mb);
    cJSON_AddStringToObject(root, "network_mb_in", buffer);

    sprintf(buffer, "%.3f", network_out_mb);
    cJSON_AddStringToObject(root, "network_mb_out", buffer);

    // Convert JSON to string and print it
    char *json_data = cJSON_Print(root);
    printf("JSON Data: %s\n", json_data);  // You can send this to the server or save it

    // Free the memory
    cJSON_Delete(root);
    free(json_data);
}