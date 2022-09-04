assume cs:code

code segment 
start:
		;将中断例程移入0:200h
		mov ax,cs
		mov ds,ax
		mov si,offset print
		mov ax,0
		mov es,ax
		mov di,200h
		mov cx,offset p_end-offset print
		cld
		rep movsb
		;设置中断向量表
		mov ax,0
		mov es,ax
		mov word ptr es:[7ch*4],200h
		mov word ptr es:[7ch*4+2],0

		mov ax,4c00h
		int 21h

print:
		push di
		push dx
		mov ax,0b800h
		mov es,ax
		;根据行列计算目的地址,由di存储
		mov ah,0
		mov al,160
		mul dh
		mov di,ax
		mov ah,0
		mov al,2
		mul dl
		add di,ax
		mov dl,cl;dl暂存颜色代码
		mov ch,0;使用cx进行jcxz跳转

s:		mov al,[si]
		inc si
		mov es:[di],al
		inc di
		mov es:[di],dl
		inc di
		mov cl,ds:[si]
		jcxz p_finish
		jmp short s
p_finish:
		pop dx
		pop di
		iret
p_end:
		nop
code ends

end start