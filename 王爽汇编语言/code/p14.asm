assume cs:code
code segment
start:
		mov si,9;si����Ѱַ��д�ָ��������ҿ���ѭ������	
		mov bx,0b800h
		mov es,bx
		mov di,70
s:		
		;��ȡʱ������
		mov ax,si
		out 70h,al
		in al,71h
		;����BCD����
		mov ah,al		
		mov cl,4
		shr ah,cl
		and al,00001111b
		;תΪascii����ʽ
		add al,30h
		add ah,30h
		;�ƶ����Դ�		
		mov es:[160*12+di],ah
		mov es:[160*12+di+2],al
		;���Ϸָ���
		cmp si,8
		jnb year_or_month;si>=8
		cmp si,7
		je day;si=7
		cmp si,0
		jne hour_or_minute;si<8 && si!=7 && si!=0
		;else˵������second��ֱ����ת
second:
		jmp short iterate
year_or_month:
		mov byte ptr es:[160*12+di+4],2fh;2fh�ǡ�/����ascii��ֵ
		jmp short iterate
day:
		mov byte ptr es:[160*12+di+4],20h;
		jmp short iterate
hour_or_minute:
		mov byte ptr es:[160*12+di+4],3ah;
		jmp short iterate

iterate:		
		add di,6
		;si����,iterate����8,9�ĵ���

		cmp si,7
		je iterate3;si=7

		cmp si,4
		jna iterate2;si<=4	

		sub si,1
		jmp short s
iterate3:
		sub si,3
		jmp short s
iterate2:
		cmp si,0
		je finish
		sub si,2
		jmp short s
finish:
		mov ax,4c00h
		int 21h
code ends
end start