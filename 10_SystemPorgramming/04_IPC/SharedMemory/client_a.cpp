#include <iostream>
#include <fcntl.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <unistd.h>
#include <cstring>
#include "shared_data.h"

int main() {
    std::cout << "=== Client A (Writer) ===\n";

    // 1. Create shared memory
    int fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    ftruncate(fd, sizeof(SharedData));

    // 2. Map it into this process
    SharedData* data = (SharedData*)mmap(
        0, sizeof(SharedData),
        PROT_READ | PROT_WRITE,
        MAP_SHARED, fd, 0
    );

    // 3. Create semaphore for synchronization
    sem_t* sem = sem_open(SEM_NAME, O_CREAT, 0666, 1);

    // 4. Write data in a loop
    int counter = 0;
    while (true) {
        sem_wait(sem);  // lock

        data->counter    = counter;
        data->data_ready = true;
        snprintf(data->message, sizeof(data->message),
                 "Message #%d from Client A", counter);

        std::cout << "A wrote: " << data->message << "\n";

        sem_post(sem);  // unlock

        counter++;
        sleep(2);  // write every 2 seconds

        if (counter >= 10) break;
    }

    // 5. Cleanup
    munmap(data, sizeof(SharedData));
    close(fd);
    shm_unlink(SHM_NAME);
    sem_close(sem);
    sem_unlink(SEM_NAME);

    std::cout << "A finished and cleaned up.\n";
    return 0;
}