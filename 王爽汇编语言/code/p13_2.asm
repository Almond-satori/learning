assume cs:code

code segment
start:
		;将中断例程移入内存中(0:200)
		mov ax,cs
		mov ds,ax
		mov si,offset circle
		mov ax,0
		mov es,ax
		mov di,200h
		mov cx,offset c_end-offset circle
		cld
		rep movsb
		;配置中断向量表
		mov ax,0
		mov es,ax
		mov word ptr es:[7ch*4],200h
		mov word ptr es:[7ch*4+2],0

		mov ax,4c00h
		int 21h

		;写中断例程代码
circle:
		push bp
		mov bp,sp
		dec cx
		jcxz finish
		add [bp+2],bx
finish:
		pop bp
		iret
c_end:
		nop
code ends
end start