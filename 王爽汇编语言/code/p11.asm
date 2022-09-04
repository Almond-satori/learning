assume cs:code 

data segment
db "Beginner's All-purpose Symbolic Instruction Code.",0	
data ends

code segment
begin:
	mov ax,data
	mov ds,ax
	mov si,0
	call letterc

	mov ax,4c00h
	int 21h

letterc:
	mov si,0
	mov ah,0
	mov ch,0
s:	mov al,ds:[si]
	cmp al,65
	inc si
	jb x	
	dec si
	and al,11011111b
	mov ds:[si],al
	inc si
x:	
	mov cl,ds:[si]
	jcxz finish	
	jmp s
finish:
	ret

code ends
end begin