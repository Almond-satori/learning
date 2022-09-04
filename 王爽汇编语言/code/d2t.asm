assume cs:code

code segment
sysboot:    jmp near ptr maincode
menudata1: db '------welcome------',0
menudata2: db '1)reset pc',0
menudata3: db '2)start system',0
menudata4: db '3)clock',0
menudata5: db '4)set clock',0
menudata6: db 'copyright @ 2022 KirisameAlmond,Inc.All rights reserved.',0 
;---表单end---

;---核心循环代码start---
maincode:
    call showmenu
    mov ax,4c00h
    int 21h
;---核心循环代码end---

;---功能start---
showmenu:
    push ax 
    push es
    push bx
    push di
    
    mov ax,0b800h
    mov es,ax
    mov bx,10*160
    mov di,20*2
    mov ax,cs
    mov ds,ax
    mov si,offset menudata1  
    mov cx,6
showlines:
    call showline
    add bx,160;列地址不变,行地址增加160
    inc si;控制ds:si指向下一行开头
    loop showlines
    pop di
    pop bx
    pop es
    pop ax

  
nextline:

    ret  
;---功能end---

;---功能的子程序start---

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
end sysboot