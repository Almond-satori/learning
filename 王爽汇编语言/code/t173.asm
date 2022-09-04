assume cs:code

data segment
db 128 dup (0)
data ends

code segment
start:
    mov ax,data
    mov ds,ax
    mov si,0
    mov dh,20
    mov dl,20
    call getstr

    mov ax,4c00h
    int 21h

;接收字符串的子程序
getstr: push ax

getstrs:    
    mov ah,0
    int 16h;使用16中断的功能0，返回值保存至ah,al分别是机器码和ascii
    cmp al,20h;ascii<20h的不是字符
    jb nochar
    mov ah,0
    call charstack
    mov ah,2
    call charstack
    jmp getstrs

nochar:
    cmp ah,0eh;退格键的扫描码
    je backspace
    cmp ah,1ch
    je enter
    jmp getstrs
backspace:
    mov ah,1
    call charstack
    mov ah,2
    call charstack
    jmp getstrs

enter:
    mov al,0
    mov ah,0
    call charstack;0入栈
    mov ah,2
    call charstack
    pop ax
    ret

;参数说明：
;ah功能号，
;ds:si指向字符栈空间
;功能0:al入栈字符，
;功能1:al返回字符
;功能2:dh，dl对应想显示的行号，列号
charstack:  jmp short charstart
table  dw charpush,charpop,charshow
top   dw 0;指向栈顶

charstart: 
    push bx 
    push es
    push dx
    push di
   

    cmp ah,2
    ja sret
    mov bl,ah
    mov bh,0
    add bx,bx
    jmp word ptr table[bx]

charpush:
    mov bx,top
    mov [si][bx],al
    inc top
    jmp sret

charpop:
    cmp top,0
    je sret
    dec top
    mov bx,top
    mov al,[si][bx]
    jmp sret
    
charshow:
    mov bx,0b800h
    mov es,bx
    mov al,160
    mov ah,0
    mul dh;ax保存行地址
    mov dh,0
    add dl,dl;dx保存列地址
    add dx,ax
    mov di,dx;di保存位置地址
    mov bx,0
charshows:
    cmp top,bx;注意迭代的参数是bx，top值在循环中没有变化，不能写cmp top,0
    jne noempty
    mov byte ptr es:[di],' '
    jmp sret
noempty:
    mov al,[si][bx]
    mov es:[di],al
    mov byte ptr es:[di+2],' ';如果是退格操作,需要对之前的字符清理
    inc bx
    add di,2
    jmp charshows

sret:
    pop di
    pop dx
    pop es
    pop bx
    ret

code ends

end start