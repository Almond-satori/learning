#include <stdio.h>

int main(){
    int* data = malloc(100*sizeof(int));
    free(data);
    for (int i = 0; i < 100; i++)
    {
        printf("%d,",*(data + i));//100个0成功输出
    }
    printf("over\n");//没有报error，正常结束
    return 0;
}
/*
[root@CentAlmond chapter14]# valgrind --leak-check=yes ./test6
==13455== Memcheck, a memory error detector
==13455== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==13455== Using Valgrind-3.15.0 and LibVEX; rerun with -h for copyright info
==13455== Command: ./test6
==13455== 
==13455== Invalid read of size 4
==13455==    at 0x40062C: main (test6.c:9)
==13455==  Address 0x5204040 is 0 bytes inside a block of size 400 free'd
==13455==    at 0x4C2B06D: free (vg_replace_malloc.c:540)
==13455==    by 0x40060E: main (test6.c:5)
==13455==  Block was alloc'd at
==13455==    at 0x4C29F73: malloc (vg_replace_malloc.c:309)
==13455==    by 0x4005FE: main (test6.c:4)
==13455== 
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,over
==13455== 
==13455== HEAP SUMMARY:
==13455==     in use at exit: 0 bytes in 0 blocks
==13455==   total heap usage: 1 allocs, 1 frees, 400 bytes allocated
==13455== 
==13455== All heap blocks were freed -- no leaks are possible
==13455== 
==13455== For lists of detected and suppressed errors, rerun with: -s
==13455== ERROR SUMMARY: 100 errors from 1 contexts (suppressed: 0 from 0)
*/