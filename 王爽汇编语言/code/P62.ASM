assume cs:code,ss:stacksg,ds:datasg

stacksg segment
      dw 0,0,0,0,0,0,0,0
stacksg ends

datasg segment
        db '1. display      '
        db '2. brows        '
        db '3. replace      '
        db '4. modify       '
datasg ends

code segment
start:  mov ax,datasg
        mov ds,ax
        mov ax,stacksg
        mov ss,ax
        mov sp,4h
        mov cx,4
        mov bx,0

s0:     push cx
        mov cx,4
        mov si,3
s1:     
        mov al,[bx+si]
        and al,11011111b
        mov [bx+si],al
        inc si
        loop s1

        pop cx
        add bx,10h
        loop s0

        mov ax,4c00h
        int 21h
code ends

end start
