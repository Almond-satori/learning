assume cs:code

stack segment
	db 128 dup (0)
stack ends

code segment 
start:		
	mov ax,stack
	mov ss,ax
	mov sp,128

	;0:200h-0:203h�洢ԭ�ж����̵�ַ	
	mov ax,0
	mov es,ax
	push es:[9*4]
	pop es:[200h]
	push es:[9*4+2]
	pop es:[202h]
	
	;�����ж����̰�װ��0:204h
	mov ax,cs
	mov ds,ax
	mov si,offset int9
	mov di,204h
	mov cx,offset int9end-offset int9
	cld
	rep movsb

	;�ı�int 9�ж�������
	cli 
	mov word ptr es:[9*4],204h
	mov word ptr es:[9*4+2],0
	sti

	;ע����������չint9�����ݣ����ﲻ��Ҫ�ָ�ԭint 9���ж��������ж�����
	
	mov ax,4c00h
	int 21h

int9:
	push ax
	push di
	push cx
	push es

	in al,60h

	pushf;psw��ջ
	call dword ptr cs:[200h];˫�ֵ���ԭint 9

	cmp al,3bh;F1��ɨ������3bh
	jne int9ret

	mov ax,0b800h
	mov es,ax
	mov di,1
	mov cx,2000
s:	
	inc byte ptr es:[di];ע��������ֽ�Ϊ��λ����
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