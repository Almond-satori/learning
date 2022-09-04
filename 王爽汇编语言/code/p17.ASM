assume cs:code

stack segment
db 128 dup (0)
stack ends

data segment
db 30 dup ("kirisameMarisadaza!!")
data ends

code segment
start:  
    mov ax,stack
    mov ss,ax
    mov sp,128

    ;在0:200h处安装新中断例程
    mov ax,0
    mov es,ax   
    mov di,200h
    mov ax,cs
    mov ds,ax
    mov si,offset int7ch
    mov cx,offset int7chend - offset int7ch
    cld
    rep movsb
    ;重设中断向量表
    cli
    mov word ptr es:[7ch*4],200h
    mov word ptr es:[7ch*4+2],0
    sti
    
    mov ax,data
    mov es,ax
    mov bx,0
    ;测试写入
    mov ah,1
    mov al,2
    mov dx,17
    int 7ch
    ;测试读出
    mov ah,0
    mov al,1
    mov dx,17
    int 7ch

    mov ax,4c00h
    int 21h

;ah:确定功能0读取1写入
;dx:要操作扇区的逻辑扇区编号
;es:bx读：指向存储读出数据的内存区,
;写：指向写入数据的内存区
int7ch:    
    push ax
    push bx
   
    mov ax,dx
    mov dx,0
    mov bx,1440
    div bx
    ;ax为面数,dx是余数后续使用
    push ax;将面数入栈
    mov ax,dx
    mov dx,0
    mov bx,18
    div bx
    push ax;磁道号,dx是余数
    inc dx
    push dx;扇区号
    
    pop cx;cl扇区号
    pop ax;ch磁道号
    mov ch,al

    pop dx;dh磁头号（面数）
    mov dh,dl
    mov dl,0;驱动器号
    pop bx  
    pop ax  
    int 13h

    iret
int7chend:  nop

code ends
end start