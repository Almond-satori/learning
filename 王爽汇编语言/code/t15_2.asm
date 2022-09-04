assume cs:code

stack segment
	db 128 dup (0)
stack ends

code segment 
start:		
	mov ax,stack
	mov ss,ax
	mov sp,128

	;0:200h-0:203h存储原中断例程地址	
	mov ax,0
	mov es,ax
	push es:[9*4]
	pop es:[200h]
	push es:[9*4+2]
	pop es:[202h]
	
	;将新中断例程安装在0:204h
	mov ax,cs
	mov ds,ax
	mov si,offset int9
	mov di,204h
	mov cx,offset int9end-offset int9
	cld
	rep movsb

	;改变int 9中断向量表
	cli 
	mov word ptr es:[9*4],204h
	mov word ptr es:[9*4+2],0
	sti

	;注意我们是扩展int9的内容，这里不需要恢复原int 9的中断向量和中断例程
	
	mov ax,4c00h
	int 21h

int9:
	push ax
	push di
	push cx
	push es

	in al,60h

	pushf;psw入栈
	call dword ptr cs:[200h];双字调用原int 9

	cmp al,3bh;F1的扫描码是3bh
	jne int9ret

	mov ax,0b800h
	mov es,ax
	mov di,1
	mov cx,2000
s:	
	inc byte ptr es:[di];注意控制以字节为单位自增
	add di,2
	loop s
	
int9ret:
	pop es
	pop cx
	pop di
	pop ax
	iret

int9end:nop

code ends
end start