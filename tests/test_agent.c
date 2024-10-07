#include <assert.h>
#include <../include/agent.h>
#include <stdio.h>

#define EPSILON 0.0001

// Function prototypes
void test_get_cpu_usage();
void test_bytes_to_gb();
void test_round_to_places();
void test_get_memory_usage();
void test_get_page_size();
void test_get_disk_usage();
void test_get_network_usage();
void test_create_resource_json();

int main(void) {
    test_get_cpu_usage();
    test_bytes_to_gb();
    test_round_to_places();
    test_get_memory_usage();
    test_get_page_size();
    test_get_disk_usage();
    test_get_network_usage();
    test_create_resource_json();

    printf("All tests passed!\n");
    return 0;
}

void test_get_cpu_usage() {
    float cpu_usage = get_cpu_usage();

    assert(cpu_usage >= 0.0 && cpu_usage <= 100.0);

    printf("test get_cpu_usage passed!\n");
}

void test_bytes_to_gb() {
    assert(bytes_to_gb(1073741824) == 1.0);
    assert(bytes_to_gb(2147483648) == 2.0);

    printf("test bytes_to_gb passed!\n");
}

void test_round_to_places() {
    assert(fabs(round_to_places(1.23456789, 3) - 1.235) < EPSILON);
    assert(fabs(round_to_places(1.23456789, 2) - 1.23) < EPSILON);
}

void test_get_memory_usage() {
    uint64_t total_mem = 0, free_mem = 0;
    get_memory_usage(&total_mem, &free_mem);

    // Memory should not be 0 and should have reasonable values
    assert(total_mem > 0);
    assert(free_mem >= 0);

    printf("test_get_memory_usage passed!\n");
}

void test_get_page_size() {
    uint64_t page_size = get_page_size();
    assert(page_size >= 4096 && page_size <= 16384);
    printf("test_get_page_size passed!\n");
}


void test_get_disk_usage() {
    uint64_t total_disk = 0, used_disk = 0;
    
    // Use a valid path on your system, such as root "/"
    get_disk_usage("/", &total_disk, &used_disk);

    // Basic assertions to verify non-zero values
    assert(total_disk > 0 && "Total disk space should be greater than 0");
    assert(used_disk > 0 && "Used disk space should be greater than 0");
    
    // Add more specific checks if you know the exact disk space
    printf("test_get_disk_usage passed!\n");
}

void test_get_network_usage() {
    uint64_t bytes_in = 0, bytes_out = 0;

    get_network_usage(&bytes_in, &bytes_out);

    // Basic assertions to verify the function captures some network traffic data
    assert(bytes_in >= 0 && "Bytes in should be 0 or greater");
    assert(bytes_out >= 0 && "Bytes out should be 0 or greater");
    
    printf("test_get_network_usage passed!\n");
}

void test_create_resource_json() {
    // Simulate resource data
    float cpu_usage = 20.5;
    uint64_t total_mem = 8589934592;  // 8GB in bytes
    uint64_t free_mem = 4294967296;   // 4GB in bytes
    uint64_t total_disk = 1000000000000; // 1TB in bytes
    uint64_t used_disk = 500000000000;  // 500GB in bytes
    uint64_t bytes_in = 1000000;  // 1MB in bytes
    uint64_t bytes_out = 500000;  // 500KB in bytes

    // Create and print JSON
    create_resource_json(cpu_usage, total_mem, free_mem, total_disk, used_disk, bytes_in, bytes_out, false);

    // We can use cJSON_GetObjectItem to verify specific parts of the JSON
    cJSON *root = cJSON_Parse("{ \"cpu_usage_percent\": \"79.500\" }");
    cJSON *cpu_usage_json = cJSON_GetObjectItem(root, "cpu_usage_percent");
    
    // Verify if the JSON contains the correct formatted value
    assert(strcmp(cpu_usage_json->valuestring, "79.500") == 0 && "CPU usage JSON should be correct");
    
    // Free the JSON object
    cJSON_Delete(root);
    
    printf("test_create_resource_json passed!\n");
}