#ifndef SHM_MANAGER_H
#define SHM_MANAGER_H

#include <string>
#include <fcntl.h>      
#include <sys/mman.h>   
#include <sys/stat.h>   
#include <unistd.h>     
#include <stdexcept>    
#include <cstring>     
#include <cerrno>      
#include <cstddef>
#include <fcntl.h>
#include <cstdio>

class ShmManager {
    public:
        ShmManager(const char* name, size_t size, bool create_mem);
        ~ShmManager();

        ShmManager(const ShmManager&) = delete;
        ShmManager& operator=(const ShmManager&) = delete;

        ShmManager(ShmManager&&) = delete;          
        ShmManager& operator=(ShmManager&&) = delete;

        void* get_ptr() const { return shm_ptr_; };
        int get_fd() const{ return shm_fd_; };

    private:
        int shm_fd_;
        void* shm_ptr_;
        size_t size_;
        const char *name_;
        bool owns_mem;
};

#endif