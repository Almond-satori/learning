assume cs:code

stack segment
	db 128 dup (0)
stack ends

code segment
start:
	mov ax,stack
	mov ss,ax
	mov sp,128
	;��ԭint9�ж������洢��0:200h	
	mov ax,0
	mov es,ax
	push es:[9*4]
	pop es:[200h]
	push es:[9*4+2]
	pop es:[202h]
	;�����ж����̴洢��0:204h
	mov ax,cs
	mov ds,ax
	mov si,offset int9
	mov di,204h
	mov cx,offset int9end-offset int9
	cld
	rep movsb
	;�޸��ж�������
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
	call dword ptr cs:[200h];����int9��ʱ��csһ����0,
	;���ﲻ��дes,��Ϊ�κ�ʱ�򶼿��ܵ����ж�,���ܱ�֤es��0

	cmp al,1eh+80h;�ж��Ƿ���a�Ķ���
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