assume cs:code 

data segment  

    db '1975','1976','1977','1978','1979','1980','1981','1982','1983'  

    db '1984','1985','1986','1987','1988','1989','1990','1991','1992'  

    db '1993','1994','1995' 


    dd 16,22,382,1356,2390,8000,16000,24486,50065,97479,140417,197514  

    dd 345980,590827,803530,1183000,1843000,2759000,3753000,4649000,5937000  


    dw 3,7,9,13,28,38,130,220,476,778,1001,1442,2258,2793,4037,5635,8226  

    dw 11542,14430,15257,17800


data ends  

stack segment
db 256 dup(0)
stack ends
 
 reg segment
 dd 0
 reg ends

 code segment
 start:
		mov ax,data
		mov ds,ax
		mov ax,0b800h
		mov es,ax
		mov ax,stack
		mov ss,ax
		mov sp,256
		
		mov cx,21;循环次数
		mov bx,640;行地址,列数由idata直接定位
		mov di,0;列地址
		mov si,0;寻址到data中的每个年份以及收入,每轮自增4
		mov bp,168;寻址到data中的每个雇员数
		mov dl,2;颜色

s:		push cx
		mov cx,4
year:		
		mov al,ds:[si]
		inc si;si在循环中用于寻址年份中的每个数据
		mov es:[bx+di],al
		inc di
		mov es:[bx+di],dl
		inc di
		loop year
		sub si,4;将si恢复为寻址年份和收入
		call space
income:
		mov ax,reg
		mov ds,ax
		mov ds:[0],bx;将bx存入reg中	
		mov ax,data
		mov ds,ax;恢复ds
		mov ax,ds:[si+84];ax接收低16位
		mov dx,ds:[si+86];dx接收高16位
		mov di,10
		call dtoc		
emp:		
		call dtoc_emp
avg:
		mov ax,ds:[si+84]
		mov dx,ds:[si+86]
		mov cx,ds:[bp]		
		call divdw;ax,dx由divdw算出
		mov di,38;reg在计算income时已经存储这次循环的bx
		call dtoc
iter:
		add bx,0a0h;行地址自增
		mov di,0;每轮结束，列地址清零
		pop cx
		add si,4
		add bp,2
		loop s
		mov ax,4c00h
		int 21h

dtoc:
;ax被除数低16位
;dx被除数高16位
;bx行地址
;di表示想要写入的列地址
init:			
			push bp			
			mov bp,0;bp为计数器，记录数字长度					
dtoc_div:			
			mov cx,0ah;cx为除数10		
			call divdw			
			add cx,30h;数字转换为字符
			push cx
			mov cx,dx;判断商是否为0
			or cx,ax
			inc bp
			jcxz dtoc_ok
			jmp dtoc_div

dtoc_ok:
		mov cx,bp
		mov ax,reg
		mov ds,ax
		mov bx,ds:[0];将原来存储的bx值恢复
		mov ax,data
		mov ds,ax;将ds恢复
		mov dl,2

show_num:
		pop ax
		mov es:[bx+di],al
		inc di
		mov es:[bx+di],dl
		inc di
		loop show_num
		pop bp
		ret

dtoc_emp:
		push si		
		mov si,0;si用来计数
		mov ax,ds:[bp]
emp_div:		
		mov cx,0ah
		mov dx,0		
		div cx
		mov cx,dx
		add cx,30h
		push cx
		inc si
		mov cx,ax
		jcxz emp_w
		jmp emp_div
emp_w:
		mov cx,si
		mov di,26
ws:		
		mov dl,2
		pop ax
		mov es:[bx+di],al
		inc di 
		mov es:[bx+di],dl
		inc di
		loop ws

		pop si
		ret

divdw:
;ax低16位，dx高16位，cx除数
;dx结果的高16位，ax结果的低16位，cx余数
		push di
		mov di,ax
        mov ax,dx
        mov dx,0
        div cx
        mov bx,ax
		mov ax,di
        div cx
        mov cx,dx
        mov dx,bx
		pop di
        ret
space:
		mov ax,20h
		mov es:[di],ax
		add di,2
		ret
 code ends
 end start