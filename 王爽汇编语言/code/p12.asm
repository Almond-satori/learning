assume cs:code
code segment
;将do0中代码送入0:200
start:
		mov ax,cs
		mov ds,ax
		mov si,offset do0
		mov ax,0
		mov es,ax
		mov di,200h
		mov cx,offset do0end-offset do0
		cld
		rep movsb
		;设置中断向量表
		mov ax,0
		mov ds,ax
		mov word ptr ds:[0],200h
		mov word ptr ds:[2],0
		mov ax,4c00h
		int 21h
;do0中断操作的程序以及数据
do0:
		jmp short do0start
db	"overflow!"

do0start:
		mov ax,cs
		mov ds,ax
		mov si,202h

		mov ax,0b800h
		mov es,ax
		mov di,12*160+68;显示在屏幕中央

		mov cx,9
s:		mov al,ds:[si]
		inc si
		mov es:[di],al
		inc di
		mov byte ptr es:[di],2
		inc di
		loop s

		mov ax,4c00h
		int 21h
do0end: nop	
code ends
end start