#include <stdio.h>

int main(){
    int * p = malloc(sizeof(int));
    //malloc：incompatible implicit declaration of built-in function ‘malloc’ [enabled by default]gcc
    printf("return without free\n");
    return 0;
}

/*
[root@CentAlmond chapter14]# valgrind --leak-check=yes ./malloc
==17457== Memcheck, a memory error detector
==17457== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==17457== Using Valgrind-3.15.0 and LibVEX; rerun with -h for copyright info
==17457== Command: ./malloc
==17457== 
return without free
==17457== 
==17457== HEAP SUMMARY:
==17457==     in use at exit: 4 bytes in 1 blocks
==17457==   total heap usage: 1 allocs, 0 frees, 4 bytes allocated
==17457== 
==17457== 4 bytes in 1 blocks are definitely lost in loss record 1 of 1
==17457==    at 0x4C29F73: malloc (vg_replace_malloc.c:309)
==17457==    by 0x40056E: main (malloc.c:4)
==17457== 
==17457== LEAK SUMMARY:
==17457==    definitely lost: 4 bytes in 1 blocks
==17457==    indirectly lost: 0 bytes in 0 blocks
==17457==      possibly lost: 0 bytes in 0 blocks
==17457==    still reachable: 0 bytes in 0 blocks
==17457==         suppressed: 0 bytes in 0 blocks
==17457== 
==17457== For lists of detected and suppressed errors, rerun with: -s
==17457== ERROR SUMMARY: 1 errors from 1 contexts (suppressed: 0 from 0)

*/