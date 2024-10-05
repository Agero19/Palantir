#include <assert.h>
#include <../include/agent.h>
#include <stdio.h>

#define EPSILON 0.0001

// Function prototypes
void test_get_cpu_usage();
void test_bytes_to_gb();
void test_round_to_places();
void test_get_memory_usage();

int main(void) {
    test_get_cpu_usage();
    test_bytes_to_gb();
    test_round_to_places();
    test_get_memory_usage();

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
