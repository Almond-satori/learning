assume cs:code
code segment
start:
		mov ax,1000h
		mov cl,1
		div cl

		mov ax,4c00h
		int 21h
code ends
end start