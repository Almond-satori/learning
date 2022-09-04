assume cs:code

code segment
start:
    ;将代码写入软盘0道0面1扇区处
    mov ax,cs
    mov es,ax
    mov bx,offset boot

    mov ah,3
    mov al,1;需要确定写入几个扇区unf,目前是1
    mov ch,0
    mov cl,1
    mov dh,0
    mov dl,0
    int 13h

    mov ax,4c00h
    int 21h

boot:   jmp short bootstart;短转移占用2字节
    option1: db '1)reset pc',0
    option2: db '2)start system',0
    option3: db '3)clock',0
    option4: db '4)set clock',0
    table:  
        dw offset reset -offset boot + 7c00h 
        dw offset start_system -offset boot + 7c00h   
        dw offset clock -offset boot + 7c00h 
        dw offset set_clock -offset boot + 7c00h
bootstart:

;在屏幕上显示出4个选项   
showopt:  
    mov ax,0b800h
    mov es,ax
    mov bx,160*11;第12行地址
    mov di,34*2;第35列地址
    mov ax,cs;安装完成后cs:ip指向0:7c00h   
    mov ds,ax;ds->0
    mov si,offset option1 - offset boot;option1是安装程序到软盘时option1处数据相对于cs偏移值
    mov cx,4
showlines:
    push cx
showline:
    mov al,ds:[si+7c00h]
    mov es:[bx+di],al
    add di,2
    inc si
    mov ch,0
    mov cl,ds:[si+7c00h]
    jcxz Lfinished
    jmp showline
Lfinished:  
    mov di,34*2;重置列地址
    add bx,160
    inc si;si指向下一行开头
    pop cx
    loop showlines

;实现具体功能
;读取功能
readkey:    
    mov ah,0
    int 16h
    cmp al,31h;1的ascii
    jb readkey
    cmp al,34h;4的ascii
    ja readkey
    sub al,30h
    mov bl,al
    mov bh,0
    add bx,bx    
    call word ptr cs:[bx+7c00h+offset table-offset boot]

reset:
;借用0:200处的内存单元进行跳转
    mov ax,200h
    mov ds,ax
    mov ax,0
    mov ds:[0],ax
    mov ax,0ffffh
    mov ds:[1],ax
    jmp word ptr ds:[0]
start_system:
    
clock:

set_clock:
  

db 512 dup (0);程序不够512字节（一个扇区的大小），补充512byte，多的不传入软盘
code ends
end start