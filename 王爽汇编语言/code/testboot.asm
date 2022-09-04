assume cs:code
code segment
start:
boot:   jmp short bootstart;短转移占用2字节
    option1: db '1)reset pc',0
    option2: db '2)start system',0
    option3: db '3)clock',0
    option4: db '4)set clock',0
bootstart:
showopt:
    ;在屏幕上显示出4个选项
    mov ax,0b800h
    mov es,ax
    mov bx,160*11;第12行地址
    mov di,35*2;第36列地址
    mov ax,cs
    mov ds,ax
    mov si,offset option1
    mov cx,4
showlines:
    push cx
showline:
    mov al,ds:[si]   
    mov es:[bx+di],al
    add di,2
    inc si  
    mov ch,0
    mov cl,ds:[si]
    jcxz Lfinished
    jmp showline
Lfinished:  
    mov di,35*2;重置列地址
    add bx,160
    inc si;si指向下一行开头
    pop cx
    loop showlines

    mov ax,4c00h
    int 21h
code ends
end start