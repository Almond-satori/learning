assume cs:code
code segment
start:  mov dx,1h
        mov ax,86a1h
        mov bx,100
        div bx

        mov ax,4c00h
        int 21h
code ends
end start
