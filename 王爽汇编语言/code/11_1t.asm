assume cs:code
code segment
start: 
sub al,al
mov al,1
push ax
pop bx
add al,bl
add al,10
mul al
code ends
end start
