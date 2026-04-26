#pragma once

// This header is included by BOTH client_a and client_b
// It defines the data layout in shared memory

struct SharedData {
    int   counter;
    char  message[256];
    bool  data_ready;
};



const char* SHM_NAME  = "/ipc_shared_data";
const char* SEM_NAME  = "/ipc_semaphore";
const int   SHM_SIZE  = sizeof(SharedData);

