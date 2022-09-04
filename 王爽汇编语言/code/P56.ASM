assume cs:code

a segment
      dw 1,2,3,4,5,6,7,8,9,0ah,0bh,0ch,0dh,0eh,0fh,0ffh
a ends

b segment
      dw 0,0,0,0,0,0,0,0
b ends

code segment
start:  mov ax,a
        mov ds,ax
        add ax,10h
        mov ss,ax
        mov sp,10h
        mov cx,8
        mov bx,0
        mov dx,b
        mov es,dx

s:      push [bx]
        pop ax
        mov es:[bx],al
        inc bx
        mov es:[bx],ah
        inc bx
        loop s

        mov ax,4c00h
        int 41h
code ends

end start

