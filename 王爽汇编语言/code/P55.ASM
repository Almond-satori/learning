assume cs:code

a segment
        db 1,2,3,4,5,6,7,8
a ends

b segment
        db 1,2,3,4,5,6,7,8
b ends

c segment
        db 0,0,0,0,0,0,0,0
c ends

code segment
start:  mov ax,a
        mov ds,ax
        mov bx,b
        mov es,bx
        mov cx,8
        mov bx,0

s:      mov ax,0

        mov dx,0
        mov dl,[bx]
        mov ax,dx
        mov dl,es:[bx]
        add ax,dx

        mov dx,c
        mov ds,dx

        mov [bx],al

        mov dx,a
        mov ds,dx

        inc bx
        loop s

        mov ax,4c00h
        int 21h

code ends
end start

