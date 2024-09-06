// using strace for syst_call.c for n = 1, The top 5 system calls are:
// 
// calls     syscall
// ---------  ----------------
// 8      mmap
// 4      mprotect
// 4      pread64
// 4      newfstatat
// 3      write
// 
// for n = 1000
// 
// calls     syscall
// ---------  ----------------
// 2001      write
// 8      mmap
// 4      mprotect
// 4      pread64
// 4      newfstatat
// 
// For n = 100000,
// 
// calls     syscall
// ---------  ----------------
// 200001      write
// 8      mmap
// 4      mprotect
// 4      pread64
// 4      newfstatat
// 
// Library function :
// 
// For n = 1, using ltrace
// 
// calls     lib call
// ---------  ----------------
// fflush      3
// printf()    2
// getpid()    1
// fopen       1
// fclose      1
// 
// for n = 1000,
// 
// calls     lib call
// ---------  ----------------
// fflush      2001
// printf()    1001
// fprintf     1000
// getpid()    1
// fopen       1
// 
// for n = 100000,
// 
// calls     lib call
// ---------  ----------------
// fflush      200001
// printf()    100001
// fprintf     100000
// getpid()    1
// fopen       1
// 
// As expected, with increasing n, printf, flush calls would increase
// 