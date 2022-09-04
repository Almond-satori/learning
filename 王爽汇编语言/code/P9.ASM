assume cs:code

data segment
      db 'Kirisame Marisa!'
      db 9ah,0a4h,0f9h
data ends

stack segment
      db 16 dup(0)  
stack ends

code segment
start:  mov ax,data
        mov ds,ax
        mov ax,0b800h
        mov es,ax
        mov ax,stack
        mov ss,ax
        mov sp,10h
        mov bx,06e0h
        mov cx,3
        mov si,10h
sl:     mov di,40h
        mov bp,0
        mov ah,ds:[bp+si]
        push cx
        mov cx,16        
sc:     mov al,ds:[bp]
        mov es:[bx+di],al
        mov es:[bx+di+1],ah
        add di,2
        inc bp
        loop sc
        pop cx
        inc si
        add bx,0a0h
        loop sl

        mov ax,4c00h
        int 21h
code ends
end start
