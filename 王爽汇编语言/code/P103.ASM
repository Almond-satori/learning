assume cs:code
data segment
  dw 123,12666,1,8,3,38
data ends

res segment
   db 64 dup(0)
res ends

code segment

start:	
	mov ax,res
	mov es,ax
	mov bp,0;bp指向res中的每个数据
	
	

	mov cx,6
    mov bx,0; bx指向data中的每个数字
	mov ax,data
	mov ds,ax
s:	
	 mov di,cx
	call dtoc;
	add bx,2
	mov cx,di
	loop s
	mov bx,bp;bx控制写入显存的循环次数
	
w:	mov dh,8
	mov dl,3
	mov ch,0
    mov cl,2; 8行3列,cl为颜色
	call show_str
	
	mov ax,4c00h
	int 21h

dtoc:	
	   mov ax,ds:[bx]
       mov si,10
	
circle:	
	 mov dx,0
	div si
	 mov cx,ax
	add dx,30h
	mov es:[bp],dx
	add bp,2
	 jcxz ok;
	jmp circle 
ok:	
		ret

show_str:	
	
init:   	mov ax,0b800h
        	mov es,ax
        	mov di,0
			mov ax,res
			mov ds,ax

getl:   	sub dh,1
        	mov ah,0
        	mov al,0a0h
        	mul dh
       	mov bp,ax;bp为行地址

getc:   	sub dl,1
        	mov dh,0
        	add dx,dx;列地址是 （列号-1）*2
       	add bp,dx;行列地址相加作为总地址

        	mov dh,0
        	mov dl,cl
			mov cx,bx
			sub bx,2;为了索引到最后一个元素，将bx-2
			

s1:     	
			mov ax,ds:[bx]
            mov es:[di+bp],ax
			sub bx,2
			sub cx,2
        	inc di
		 	mov es:[di+bp],dl		
        	inc di 
            jcxz e
        	jmp short s1      

e:     ret

code ends
end start
