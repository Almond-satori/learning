#include <stdio.h>

int main(){
    int* p = NULL;
    free(p);//incompatible implicit declaration of built-in function ‘free’ [enabled by default]
    printf("run succeefully\n");//可以运行
}

/*
编译：
[root@CentAlmond chapter14]# gcc -g null.c -o null
null.c: In function ‘main’:
null.c:5:5: warning: incompatible implicit declaration of built-in function ‘free’ [enabled by default]
     free(p);//incompatible implicit declaration of built-in function ‘free’ [enabled by default]
     ^

valgrind检查：
[root@CentAlmond chapter14]# valgrind --leak-check=yes ./null
==17149== Memcheck, a memory error detector
==17149== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==17149== Using Valgrind-3.15.0 and LibVEX; rerun with -h for copyright info
==17149== Command: ./null
==17149== 
run succeefully
==17149== 
==17149== HEAP SUMMARY:
==17149==     in use at exit: 0 bytes in 0 blocks
==17149==   total heap usage: 0 allocs, 0 frees, 0 bytes allocated
==17149== 
==17149== All heap blocks were freed -- no leaks are possible
==17149== 
==17149== For lists of detected and suppressed errors, rerun with: -s
==17149== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
*/