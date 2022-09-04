assume cs:code
code segment
start:  mov bl,100
        mov ax,1001
        div bl

        mov ax,4c00h
        int 21h
code ends
end start
