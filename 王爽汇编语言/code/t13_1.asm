assume cs:code

code segment
start:	
		;代码移动至7ch处
		mov ax,cs
		mov ds,ax
		mov si,offset circle		
		mov ax,0
		mov es,ax
		mov di,200h
		mov cx,offset c_end-offset circle
		cld
		rep movsb

		;修改中断向量表的值
		mov ax,0
		mov es,ax		
		mov word ptr es:[7ch*4],200h
		mov word ptr es:[7ch*4+2],0
		mov ax,4c00h
		int 21h

circle:
		push bp;因为要用到栈，所以选择bp为参数
		mov bp,sp;bp指向栈顶
		mov ch,0
		mov cl,[si]
		jcxz ok
		add [bp+2],bx;bx是转移的偏移量,bp+2指向栈中的IP		
ok:	pop bp
		iret
c_end:
		nop
code ends

end start