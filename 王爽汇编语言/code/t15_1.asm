assume cs:code

stack segment
	db 128 dup (0)
stack ends

data segment;�ݴ�ԭint9�ж����̵�ַ
	dw 0,0
data ends

code segment
start:
	mov ax,stack
	mov ss,ax
	mov sp,128

	mov ax,data
	mov ds,ax;di:0 ָ��int9ԭ�ж�����IP
	
	mov ax,0
	mov es,ax

	push es:[9*4]
	pop ds:[0]
	push es:[9*4+2]
	pop ds:[2];��ԭ�ж����̵�ַ����data��

	;�޸��ж����̵ĵ�ַ
	cli
	mov word ptr es:[9*4],offset int9
	mov word ptr es:[9*4+2],cs
	sti

	mov ax,0b800h
	mov es,ax
	mov ah,'a'
s:
	mov es:[160*12+40*2],ah
	call delay		
	inc ah
	cmp ah,'z'
	jne s
ok:
	mov ax,0
	mov es,ax
	;������������ж����ָ̻�Ϊ����λ��
	cli
	push ds:[0]
	pop es:[9*4]
	push ds:[2]
	pop es:[9*4+2]
	sti

	mov ax,4c00h
	int 21h

;Ϊ�˿�����ĸ�仯��������Ҫ������ʾһ��ʱ��
delay:
	push ax
	push dx
	mov dx,10h;���ﲻ��д̫�����ҵĻ�����д1000h��ʱ����ĸ����
	mov ax,0
s1:
	sub ax,1
	sbb dx,0;��λ������ax������1,CF=1,dx-0-CF
	cmp ax,0
	jne s1
	cmp dx,0
	jne s1;��dx,ax����0,��ѭ������
	pop dx
	pop ax
	ret

;�±�д��int9�ж�����
;�����ǽ���int9ǰ�������Ѿ�������ж�ǰ��׼���׶Σ�
;1.ȡ��n����9
;2.psw��ջ����IF,TF=0		
;3.IP,CS��ջ
;4.CS:IP��ת���ж�����

int9:
	push ax
	push es

	in al,60h
;����������Ҫ����ԭ�����ж����̣�������Ҫд��һ��int n��׼���׶Σ�������4����
;����psw�ڰ��¼��̰���ʱ�Ѿ���IF,TF��0���ﲻ�ٲ���
	pushf
	call dword ptr ds:[0];����ԭ�����ж�����

	cmp al,1;ESC�ı�����1
	jne int9ret
	mov ax,0b800h
	mov es,ax
	inc byte ptr es:[160*12+40*2+1];�ı���ɫ

int9ret:
	pop es
	pop ax
	iret

code ends
end start