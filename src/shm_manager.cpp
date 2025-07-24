#include "shm_manager.hpp"

ShmManager::ShmManager(const char *name, size_t size, bool create_mem)
    : name_(name), size_(size), shm_fd_(-1), shm_ptr_(nullptr), owns_mem(create_mem)  {

    //0666 -> public w/r
    //0600 -> private 
    //0644 -> r only for others

    int flags = O_RDWR;
    mode_t mode = 0;

    if (create_mem) {
        flags |= (O_CREAT | O_EXCL);
        mode = 0666;
    }

    int fd = shm_open(name_, flags, mode);
    if (fd == -1){
        printf("shm_open() failed with error %d: %s\n", errno, strerror(errno));
        if (mode == 0){
            throw std::runtime_error(std::string("Failed to open existing memory region"));
        }
        throw std::runtime_error(std::string("Failed to create new memory region"));
    }

    shm_fd_ = fd;

    if (create_mem){
        off_t offset = static_cast<off_t>(size_);
        if (ftruncate(fd, offset) == -1){
            printf("ftruncate() failed with error %d: %s\n", errno, strerror(errno));
            throw std::runtime_error(std::string("Failed to resize shared memory region"));
        }
    }
    else {
        struct stat sb;
        if (fstat(fd, &sb) == -1){
            printf("fstat() failed with error %d: %s\n", errno, strerror(errno));
            throw std::runtime_error(std::string("Unable to verify size of shared memory region"));
        }
        if (sb.st_size != size_){
            throw std::runtime_error(std::string("Specified size of memory region is different to actual memory region"));
        }
    }

    void* ptr = mmap(NULL, size_, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (ptr == MAP_FAILED) {
        printf("mmap() failed with error %d: %s\n", errno, strerror(errno));
        throw std::runtime_error(std::string("Unable to map shared memory into virtual memory space"));
    }

    shm_ptr_ = ptr;
}

ShmManager::~ShmManager() {

    if (shm_ptr_ != MAP_FAILED && shm_ptr_ != nullptr){
        munmap(shm_ptr_, size_);
    }

    if (shm_fd_ != -1){
        close(shm_fd_);
    }

    if (owns_mem){
        shm_unlink(name_);
    }

}
