assume cs:code 

stack segment
	db 128 dup (0)
stack ends

code segment
start: 
	;将程序安装在0:200h处
	mov ax,stack
	mov ss,ax
	mov sp,128

	mov ax,cs
	mov ds,ax
	mov si,offset setscreen
	mov ax,0
	mov es,ax
	mov di,200h
	mov cx,offset setscreenend-offset setscreen
	cld
	rep movsb
	;重设中断向量表
	mov ax,0
	mov ds,ax

	cli
	mov word ptr ds:[7ch*4],200h
	mov word ptr ds:[7ch*4+2],0
	sti

	mov ah,4
	mov al,00000100b
	int 7ch
	
	mov ax,4c00h
	int 21h

;ah:table中的功能段,0->sub1
;al:sub2，sub3中的颜色GBA
setscreen: jmp short set

;寻址到sub1234
table dw offset sub1-offset setscreen+200h
	  dw offset sub2-offset setscreen+200h
	  dw offset sub3-offset setscreen+200h
	  dw offset sub4-offset setscreen+200h
		 ;注意table数据直接与代码setscreen一同存储在0:200
		 ;不能直接写sub1，因为sub1等地址在搬运至0:200时是相对于搬运的代码的IP的偏移，
		 ;在我们使用中断时，还使用的是这个偏移，这样就会出错	
set:
	push bx

	cmp ah,3
	ja setret

	mov bl,ah
	mov bh,0
	add bx,bx
	call word ptr cs:[202h+bx];这里不能使用table,中断时不能拿原来程序的table寻址,202h因为jmp short set占用两个字节

setret:
	pop bx
	iret
	

sub1:
	push es
	push bx
	push cx

	mov bx,0b800h
	mov es,bx
	mov bx,0
	mov cx,2000
sub1s:
	mov byte ptr es:[bx],' '
	add bx,2
	loop sub1s	
	pop cx
	pop bx
	pop es
	ret
sub2:
	push es
	push bx
	push cx

	mov bx,0b800h
	mov es,bx
	mov bx,1
	mov cx,2000
sub2s:
	and byte ptr es:[bx],11111000b
	or es:[bx],al
	add bx,2
	loop sub2s

	pop cx
	pop bx
	pop es
	ret
sub3:
	push es
	push bx
	push cx

	;将al与属性值中的背景色位数对齐
	mov cl,4
	shl al,cl;shr移动位数大于1，该位数必须放在cl中
	mov bx,0b800h
	mov es,bx
	mov bx,1
	mov cx,2000
sub3s:
	and byte ptr es:[bx],10001111b
	or es:[bx],al
	add bx,2
	loop sub3s

	pop cx
	pop bx
	pop es
	ret
sub4:	
	push es
	push ds
	push cx
	push si
	push di

	mov si,0b800h
	mov es,si
	mov ds,si
	mov cx,24
	mov si,160
	mov di,0
	cld 
sub4s:
	push cx
	mov cx,160	
	rep movsb;不可以对si,di迭代,这两个参数在movsb中的每次移动过程中已经自增
	pop cx
	loop sub4s
;清空最后一行
	mov cx,80
	mov si,0
sub4last:
	mov byte ptr ds:[160*24+si],' '
	add si,2
	loop sub4last

	pop di
	pop si
	pop cx
	pop ds
	pop es
	ret
setscreenend: nop

code ends

end start