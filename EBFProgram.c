// Write a simple eBPF program using bpftrace to trace open/close syscalls and measure their frequency in a process.
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    const char *filename = "sample.txt";
    char buf[100];


    int fd = open(filename, O_CREAT | O_WRONLY, 0644);
    if (fd < 0) 
    { 
        perror("open"); 
        return 1; 
    }
    close(fd);


    for (int i = 0; i < 10; i++) 
    {
        fd = open(filename, O_RDONLY);
        if (fd < 0) 
        { 
            perror("open"); 
            return 1; 
        }
        
        int n = read(fd, buf, sizeof(buf));
        if (n < 0) 
        { 
            perror("read"); 
            return 1; 
        }

        printf("Iteration %d: File opened and read %d bytes successfully.\n", i + 1, n);
        close(fd);
        printf("Iteration %d: File closed successfully.\n", i + 1);
        usleep(500000);
    }

    return 0;
}



// root@DESKTOP-MNOTUNE:/mnt/c/Users/Admin/Desktop/IITM/Profiling/ebpf# sudo bpftrace -e 'tracepoint:syscalls:sys_enter_open,tracepoint:syscalls:sys_enter_openat /comm=="RunProg"/ { @open_count[pid] = count(); } tracepoint:syscalls:sys_exit_open,tracepoint:syscalls:sys_exit_openat /comm=="RunProg"/ { } tracepoint:syscalls:sys_enter_close /comm=="RunProg"/ { @close_count[pid] = count(); } tracepoint:syscalls:sys_exit_close /comm=="RunProg"/ { } tracepoint:syscalls:sys_enter_read /comm=="RunProg"/ { @read_count[pid] = count(); }'
// Attaching 7 probes...
// ^C

// @close_count[2462]: 13
// @open_count[2462]: 13 
// @read_count[2462]: 11 