#include <stdio.h>

int main(){
    int* data = malloc(100*sizeof(int));
    *(data+100) = 0;
    printf("return without fault\n");
    return 0;
}

/*
[root@CentAlmond chapter14]# valgrind --leak-check=yes ./test5
==17888== Memcheck, a memory error detector
==17888== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==17888== Using Valgrind-3.15.0 and LibVEX; rerun with -h for copyright info
==17888== Command: ./test5
==17888== 
==17888== Invalid write of size 4  (非法写入data[100])
==17888==    at 0x40057D: main (test5.c:5)
==17888==  Address 0x52041d0 is 0 bytes after a block of size 400 alloc'd
==17888==    at 0x4C29F73: malloc (vg_replace_malloc.c:309)
==17888==    by 0x40056E: main (test5.c:4)
==17888== 
return without fault
==17888== 
==17888== HEAP SUMMARY:
==17888==     in use at exit: 400 bytes in 1 blocks
==17888==   total heap usage: 1 allocs, 0 frees, 400 bytes allocated
==17888== 
==17888== 400 bytes in 1 blocks are definitely lost in loss record 1 of 1
==17888==    at 0x4C29F73: malloc (vg_replace_malloc.c:309)
==17888==    by 0x40056E: main (test5.c:4)
==17888== 
==17888== LEAK SUMMARY:
==17888==    definitely lost: 400 bytes in 1 blocks
==17888==    indirectly lost: 0 bytes in 0 blocks
==17888==      possibly lost: 0 bytes in 0 blocks
==17888==    still reachable: 0 bytes in 0 blocks
==17888==         suppressed: 0 bytes in 0 blocks
==17888== 
==17888== For lists of detected and suppressed errors, rerun with: -s
==17888== ERROR SUMMARY: 2 errors from 2 contexts (suppressed: 0 from 0)


*/