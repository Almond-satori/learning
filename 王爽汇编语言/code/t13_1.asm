assume cs:code

code segment
start:	
		;�����ƶ���7ch��
		mov ax,cs
		mov ds,ax
		mov si,offset circle		
		mov ax,0
		mov es,ax
		mov di,200h
		mov cx,offset c_end-offset circle
		cld
		rep movsb

		;�޸��ж��������ֵ
		mov ax,0
		mov es,ax		
		mov word ptr es:[7ch*4],200h
		mov word ptr es:[7ch*4+2],0
		mov ax,4c00h
		int 21h

circle:
		push bp;��ΪҪ�õ�ջ������ѡ��bpΪ����
		mov bp,sp;bpָ��ջ��
		mov ch,0
		mov cl,[si]
		jcxz ok
		add [bp+2],bx;bx��ת�Ƶ�ƫ����,bp+2ָ��ջ�е�IP		
ok:	pop bp
		iret
c_end:
		nop
code ends

end start