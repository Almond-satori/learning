assume cs:code

code segment 
start:
		;���ж���������0:200h
		mov ax,cs
		mov ds,ax
		mov si,offset print
		mov ax,0
		mov es,ax
		mov di,200h
		mov cx,offset p_end-offset print
		cld
		rep movsb
		;�����ж�������
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
		;�������м���Ŀ�ĵ�ַ,��di�洢
		mov ah,0
		mov al,160
		mul dh
		mov di,ax
		mov ah,0
		mov al,2
		mul dl
		add di,ax
		mov dl,cl;dl�ݴ���ɫ����
		mov ch,0;ʹ��cx����jcxz��ת

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