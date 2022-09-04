assume cs:code

stack segment
	db 128 dup (0)
stack ends

data segment;暂存原int9中断例程地址
	dw 0,0
data ends

code segment
start:
	mov ax,stack
	mov ss,ax
	mov sp,128

	mov ax,data
	mov ds,ax;di:0 指向int9原中断例程IP
	
	mov ax,0
	mov es,ax

	push es:[9*4]
	pop ds:[0]
	push es:[9*4+2]
	pop ds:[2];将原中断例程地址移入data中

	;修改中断例程的地址
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
	;程序结束，将中断例程恢复为正常位置
	cli
	push ds:[0]
	pop es:[9*4]
	push ds:[2]
	pop es:[9*4+2]
	sti

	mov ax,4c00h
	int 21h

;为了看出字母变化，我们需要让其显示一段时间
delay:
	push ax
	push dx
	mov dx,10h;这里不能写太大，在我的机器上写1000h长时间字母不变
	mov ax,0
s1:
	sub ax,1
	sbb dx,0;借位减，当ax不够减1,CF=1,dx-0-CF
	cmp ax,0
	jne s1
	cmp dx,0
	jne s1;当dx,ax都是0,则循环结束
	pop dx
	pop ax
	ret

;新编写的int9中断例程
;当我们进入int9前，我们已经完成了中断前的准备阶段：
;1.取出n，即9
;2.psw入栈，置IF,TF=0		
;3.IP,CS入栈
;4.CS:IP跳转至中断例程

int9:
	push ax
	push es

	in al,60h
;我们现在想要调用原来的中断例程，现在需要写出一个int n的准备阶段（即上述4步）
;由于psw在按下键盘按键时已经将IF,TF置0这里不再操作
	pushf
	call dword ptr ds:[0];调用原来的中断例程

	cmp al,1;ESC的编码是1
	jne int9ret
	mov ax,0b800h
	mov es,ax
	inc byte ptr es:[160*12+40*2+1];改变颜色

int9ret:
	pop es
	pop ax
	iret

code ends
end start