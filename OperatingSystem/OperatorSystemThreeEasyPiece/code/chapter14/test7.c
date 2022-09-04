#include <stdio.h>

int main(){
    int* data = malloc(100*sizeof(int));
    data += 50;
    free(data);//error
    int i;
    for (i = 0; i < 100; i++)
    {
        printf("%d,",*(data + i));//100个0成功输出
    }
    printf("over\n");//没有报error，正常结束
    return 0;
}