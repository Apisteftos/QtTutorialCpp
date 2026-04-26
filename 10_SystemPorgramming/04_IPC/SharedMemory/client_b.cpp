#include <iostream>
#include <fcntl.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <unistd.h>
#include "shared_data.h"

int main() {
    std::cout << "=== Client B (Reader) ===\n";

    // 1. Open existing shared memory (created by A)
    int fd = shm_open(SHM_NAME, O_RDONLY, 0666);

    // 2. Map it
    SharedData* data = (SharedData*)mmap(
        0, sizeof(SharedData),
        PROT_READ,
        MAP_SHARED, fd, 0
    );

    // 3. Open existing semaphore
    sem_t* sem = sem_open(SEM_NAME, 0);

    // 4. Read in a loop
    int last_counter = -1;
    for (int i = 0; i < 10; i++) {
        sem_wait(sem);  // lock

        if (data->data_ready && data->counter != last_counter) {
            std::cout << "B read:  " << data->message
                      << " (counter=" << data->counter << ")\n";
            last_counter = data->counter;
        }

        sem_post(sem);  // unlock
        sleep(1);
    }

    // 5. Cleanup
    munmap(data, sizeof(SharedData));
    close(fd);
    sem_close(sem);

    std::cout << "B finished.\n";
    return 0;
}