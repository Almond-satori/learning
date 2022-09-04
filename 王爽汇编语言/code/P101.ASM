assume cs:code
data segment
db      'Kirisema Marisa!',0
data ends

code segment
start:  mov dh,8
        mov dl,3
        mov cl,2
        mov ax,data
        mov ds,ax
        mov si,0
        call show_str

        mov ax,4c00h
        int 21h

show_str:   
init:   mov ax,0b800h
        mov es,ax
        mov di,0

getl:   sub dh,1
        mov ah,0
        mov al,0a0h
        mul dh
        mov bp,ax

getc:   sub dl,1
        mov dh,0
        add dx,dx
        add bp,dx

        mov dh,0
        mov dl,cl

s1:     mov al,ds:[si]
        mov es:[di+bp],al
        inc si
        inc di
        mov es:[di+bp],dl
        inc di
        mov cx,ds:[si]
        jcxz ok
        jmp short s1      

ok:     ret
code ends
end start
