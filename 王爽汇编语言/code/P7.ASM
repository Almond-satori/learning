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

table segment  

    db 21 dup ('year summ ne ?? ')  

table ends

code segment

start:  mov ax,table
        mov es,ax
        mov ax,data
        mov ds,ax

        mov cx,21
        mov bx,0
        mov ax,0
        mov di,0
        mov si,0

sl:     mov ax,[di]
        mov es:[bx],ax
        mov ax,[di+2]
        mov es:[bx+2],ax
        
        mov ax,[di+84]
        mov es:[bx+5],ax
        mov ax,[di+86]
        mov es:[bx+7],ax

        mov ax,[si+168]
        mov es:[bx+0ah],ax

        mov dx,[di+86]
        mov ax,[di+84]
        mov bp,[si+168]
        div bp
        mov es:[bx+0dh],ax

        add di,4
        add si,2
        add bx,10h

        loop sl

        mov ax,4c00H
        int 21H

code ends

end start
