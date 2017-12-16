#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/syscall.h>

#define INJECT_SYSCALL_NUM 337

int main (int argc, char** argv) {
    if (argc != 3) {
        printf("Usage: injector [MEMSIZE GB] [ERROR IN EVERY N PAGES]\n");
    }

    size_t nPages = (atoi(argv[1]) * 1024 * 1024) / 4;
    size_t inverseProb = atoi(argv[2]);

    // initialize random seed
    srand(time(NULL));

    FILE* fp = fopen("faults.txt", "w");

    for (size_t i = 0; i < nPages; i++) {

    	if (i % inverseProb != 0) continue;

        uint64_t pageBase = i << 12;

        uint64_t pageOffset = rand() % 4096;

        uint64_t faultAddr = pageBase + pageOffset;

        syscall(INJECT_SYSCALL_NUM, faultAddr);

        fprintf(fp, "%lx\n", faultAddr);
    }	
}