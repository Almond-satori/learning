assume cs:code

stack segment
	db 128 dup (0)
stack ends

code segment
start:
	mov ax,stack
	mov ss,ax
	mov sp,128
	;将原int9中断向量存储在0:200h	
	mov ax,0
	mov es,ax
	push es:[9*4]
	pop es:[200h]
	push es:[9*4+2]
	pop es:[202h]
	;将新中断例程存储在0:204h
	mov ax,cs
	mov ds,ax
	mov si,offset int9
	mov di,204h
	mov cx,offset int9end-offset int9
	cld
	rep movsb
	;修改中断向量表
	cli
	mov word ptr es:[9*4],204h
	mov word ptr es:[9*4+2],0
	sti

	mov ax,4c00h
	int 21h

int9:
	push ax
	push es
	push cx
	push di

	in al,60h

	pushf
	call dword ptr cs:[200h];调用int9的时候cs一定是0,
	;这里不能写es,因为任何时候都可能调用中断,不能保证es是0

	cmp al,1eh+80h;判断是否是a的断码
	jne int9ret

	mov ax,0b800h
	mov es,ax
	mov cx,2000
	mov di,0
	mov al,65
s:
	mov es:[di],al
	add di,2
	loop s

int9ret:
	pop di
	pop cx
	pop es
	pop ax
	iret

int9end: nop

code ends
end start