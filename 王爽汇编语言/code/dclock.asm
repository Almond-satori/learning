assume cs:code
code segment
start:
    jmp short astart
    timeadr: db 9,8,7,4,2,0;CMOS中的年月日时分秒的存放地址
    timedata: db 6 dup(1);存储BCD码值
    timestr: db '00/00/00 00:00:00',0
astart:
    call gettime
    call showtime

    mov ax,4c00h
    int 21h
gettime:
    push si
    push di
    push cx
    push ax

    mov si,offset timeadr
    mov di,offset timedata
    mov cx,6

gettime_s:
    mov al,cs:[si]
    out 70h,al
    in al,71h
    
    mov cs:[di],al
    inc si
    inc di
    loop gettime_s

    pop ax
    pop cx
    pop di
    pop si
    ret


showtime:
    push si
    push di
    push cx
    push ax
    push ds
    push es
    push bx
    
    mov si,offset timedata
    mov di,offset timestr
    mov cx,6
showtime_s:    
    mov al,cs:[si]
    
    call bcdtochar

    mov cs:[di],ah
    mov cs:[di+1],al
    add di,3;不要忘记有"/","空格"或者":"
    inc si
    loop showtime_s

    mov ax,cs
    mov ds,ax
    mov si,offset timestr
    mov ax,0b800h
    mov es,ax
    mov bx,160*12
    mov di,26*2   
    call showline

    pop bx
    pop es
    pop ds
    pop ax
    pop cx
    pop di
    pop si
    ret
;返回后ah为十位的ascii,al为个位的ascii
bcdtochar:  
    push cx

    mov ah,al   
    mov cx,4
    shr ah,cl;ah中存储十位BCD
    
    and al,00001111b;al中存储个位BCD

    add ah,30h
    add al,30h;转换为ASCII
    
    pop cx
    ret

;功能:内容以0结尾,在屏幕上以一行显示该内容
;es:[bx+di]指向显存地址,bx行地址,di列地址
;ds:[si]指向源地址
;注意:栈中没有保存si
showline:
    push ax
    push cx
    push di   

line:
    mov al,ds:[si]
    mov es:[bx+di],al
    inc si
    add di,2
    mov ch,0
    mov cl,ds:[si]
    jcxz showend
    jmp short line
showend:   
    pop di
    pop cx
    pop ax
    ret
;---子程序end---
code ends
end start