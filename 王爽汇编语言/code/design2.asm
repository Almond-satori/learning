assume cs:code

data segment;一个扇区的初始状态
 db 512 dup (0)
data ends

code segment
start:
    ;将代码写入data中。确保写入512字节   
    mov ax,cs
    mov ds,ax
    mov ax,offset mainboot 
    mov si,ax
    mov ax,data
    mov es,ax
    mov di,0
    mov cx,offset mainbootend - offset mainboot
    cld
    rep movsb
    ;将data写入软盘0道0面1扇区处
    mov ax,data
    mov es,ax
    mov bx,0
    mov ah,3;写入
    mov al,1;写入1个扇区
    mov ch,0;0磁道
    mov cl,1;1扇区
    mov dh,0;0面
    mov dl,0;软驱A
    int 13h
    ;将子程序代码写入软盘0道0面2扇区-0道0面4扇区
    mov ax,cs
    mov es,ax
    mov bx,offset sysboot
    mov ah,3;写入
    mov al,3;写入3个扇区
    mov ch,0;0磁道
    mov cl,2;2扇区
    mov dh,0;0面
    mov dl,0;软驱A
    int 13h

    mov ax,4c00h
    int 21h
;--------主引导程序,写入0道0面1扇区,在0:7c00h处执行start--------
;主引导:读出后续扇区的程序到内存0:7e00并跳转至该内存执行       
mainboot:
    mov ax,0
    mov es,ax
    mov bx,7e00h
    mov ah,2;读出
    mov al,3;读出3个扇区
    mov ch,0;0磁道
    mov cl,2;2扇区
    mov dh,0;0面
    mov dl,0;软驱A
    int 13h
    ;跳转至0:7e00
    mov bx,offset adr - offset mainboot + 7c00h;
    jmp dword ptr cs:[bx]
    adr: dw 7e00h,0 
mainbootend: nop
;--------主引导程序,写入0道0面1扇区,在0:7c00h处执行end--------


;--------主程序,写入0道0面2,3,4扇区start,被主引导程序放置在0:7e00h--------
;主程序包括数据和各个功能函数的表单,核心循环代码,以及各个功能的子函数
sysboot:    jmp near ptr maincode
;---表单start---
menuadr1: dw offset reset - offset sysboot + 7e00h
menuadr2: dw offset startsys - offset sysboot + 7e00h
menuadr3: dw offset showclock - offset sysboot + 7e00h
menuadr4: dw offset setclock - offset sysboot + 7e00h
menuadr5: dw offset showmenu - offset sysboot + 7e00h

menudata1: db '------welcome------',0
menudata2: db '1)reset pc',0
menudata3: db '2)start system',0
menudata4: db '3)clock',0
menudata5: db '4)set clock',0
menudata6: db 'copyright @ 2022 KirisameAlmond,Inc.All rights reserved.',0

timeadr: db 9,8,7,4,2,0;CMOS中的年月日时分秒的存放地址
timedata: db 6 dup(1);存储CMOS拿出的BCD码值
timestr: db '00/00/00 00:00:00',0

;settime:setclock功能中的显示内容
setclock_detail: db 'please input data and time,(YY MM DD hh mm ss):',0
changetime: db 12 dup('*'),0;修改后键盘输入的ASCII码值
;---表单end---


;---核心循环代码start---
;主菜单负责显示选项并实现选择操作,从其他菜单返回时需要清屏
maincode:
    call clear
    call showmenu
    call chooseOption
    jmp maincode
    
;---核心循环代码end---

;---功能start---

;--------------------显示菜单--------------------
showmenu:
    push ax 
    push es
    push bx
    push di
    
    mov ax,0b800h
    mov es,ax
    mov bx,160*11
    mov di,24*2
    mov ax,cs
    mov ds,ax
    mov si,offset menudata1 - offset sysboot + 7e00h
    mov cx,6
showlines:
    call showline
    add bx,160;列地址不变,行地址增加160
    inc si;控制ds:si指向下一行开头
    loop showlines
    pop di
    pop bx
    pop es
    pop ax
    ret

;--------------------重新启动--------------------
;cs:ip=ffff:0即可  
reset:
    ;由于要重启了,参数没必要压栈
    
    mov ax,0FFFFh
    push ax
    mov ax,0
    push ax
    retf
       
    ret

;--------------------启动操作系统--------------------
;将硬盘c中的内容移入0:7c00,并将cs:ip指向0:7c00
startsys:
    ;由于要去执行操作系统了,参数没必要压栈

    mov ah,2;读取
    mov al,1;读取一个扇区
    mov ch,0;磁道号为0
    mov cl,1;扇区号为1
    mov dh,0;面号0
    mov dl,80h;从c盘读取

    mov bx,0
    mov es,bx
    mov bx,7c00h
    
    ;进行CS:IP跳转
    int 13h
    mov ax,0
    push ax
    mov ax,7c00h
    push ax
    retf

;--------------------展示时钟--------------------
showclock:
    call clear
    push ax;在clock_readkey中ax发生改变
clock_s:    
    call gettime
    call showtime
;clock_readkey:在显示clock的界面实现esc返回主菜单,f1修改颜色的功能 
;这里没有使用子程序调用的方式,因为这里需要写esc通过ret返回主菜单 
;关于缓冲区,当我们采用16h号中断1功能时,如果缓冲区有内容,可以读出已有的内容,且读出后该内容不会在缓冲区里删除
;如果缓冲区没有内容,置ZF位为1,直接返回（不会像16h中断0功能那样等待输入）    
clock_readkey: 
    mov ah,1h;非阻塞读取,当读取到之后,缓冲区不会清空
    int 16h
    ;功能码为1的16号中断,如果缓冲区为空则直接置ZF位为1并返回
    jz clock_s;如果缓冲区为空,直接返回
    ;如果缓冲区不为空,需要对缓冲区进行清空
    call clear_key_buffer
    cmp al,1bh;前面赋值ah=1,为防止冲突采用ascii比较
    je key_esc
    cmp ah,3bh
    je key_f1
f1_back:
    jmp clock_s
key_esc:
    pop ax   
    ret
;clock界面按f1后改变字体颜色
key_f1:
    push ax
    push es
    push di
    push cx

    mov ax,0b800h
    mov es,ax
    mov di,160*12+26*2+1
    mov cx,17
change_color:    
    add byte ptr es:[di],1
    and byte ptr es:[di],00001111b;不改变背景颜色和闪烁
    add di,2
    loop change_color

    pop cx
    pop di
    pop es
    pop ax
    jmp f1_back

;setclock:设置时钟YY MM DD hh mm ss,且输入时在屏幕上显示输入内容
setclock:   
    call clear   
    mov ax,0b800h
    mov es,ax
    mov ax,0
    mov ds,ax
    mov si,offset setclock_detail - offset sysboot + 7e00h
    mov bx,13*160
    mov di,26*2
    call clear_changetime;si在程序内设置
    call showline;显示提示信息
    ;显示修改内容12个*
    mov si,offset changetime - offset sysboot + 7e00h 
    mov di,25*26
    mov bx,160*15
    call showline
    call get_settime
    call setCMOSRAM
    ret

;---功能end---

;---各项功能的子程序start---

;showline功能:内容以0结尾,在屏幕上以一行显示该内容
;es:[bx+di]指向显存地址,bx行地址,di列地址
;ds:[si]指向源地址
;注意:栈中没有保存si
showline:
    push ax
    push cx
    push di   

line:
    mov al,ds:[si]
    mov es:[bx+di],al
    inc si
    add di,2
    mov ch,0
    mov cl,ds:[si]
    jcxz showend
    jmp short line
showend:   
    pop di
    pop cx
    pop ax
    ret

;chooseOption功能:在主菜单(5)状态下读输入的按键1-4,以便选择对应功能1-4
chooseOption:
    push ax
    push bx
    push es
    
    ;es:bx指向menuadr1
    mov bx,offset menuadr1 - offset sysboot +7e00h
    mov ax,0
    mov es,ax

    mov ah,0
    int 16h
    cmp al,31h
    jb notNum
    cmp al,34h
    ja notNum

    sub al,31h;转为对应功能号地址,menuadr1对应0
    mov ah,0
    add al,al
    add bx,ax
    call word ptr es:[bx]
 
notNum:
    pop es
    pop bx
    pop ax
    ret

;清屏功能      
clear:
    push ax 
    push es
    push di      
    push cx

    mov ax,0b800h
    mov es,ax
    mov di,0
    mov ax,0700h;清屏对应属性值
    mov cx,2000
cls:
    mov es:[di],ax
    add di,2
    loop cls
    
    pop cx
    pop di
    pop es
    pop ax   
    ret 

;gettime功能:showclock中从CMOS中获取时间的BCD码,写入timedata
gettime:
    push si
    push di
    push cx
    push ax

    mov si,offset timeadr - offset sysboot + 7e00h
    mov di,offset timedata - offset sysboot + 7e00h
    mov cx,6
gettime_s:
    mov al,cs:[si]
    out 70h,al
    in al,71h
    
    mov cs:[di],al
    inc si
    inc di
    loop gettime_s

    pop ax
    pop cx
    pop di
    pop si
    ret

;showtime功能:根据timedata的BCD码值在timestr中写入ASCII码表示的时间,并在屏幕上显示
showtime:
    push si
    push di
    push cx
    push ax
    push ds
    push es
    push bx
    
    mov si,offset timedata - offset sysboot + 7e00h
    mov di,offset timestr - offset sysboot + 7e00h   
    mov cx,6
showtime_s:    
    mov al,cs:[si]
    
    call bcdtochar

    mov cs:[di],ah
    mov cs:[di+1],al
    add di,3;不要忘记有"/","空格"或者":"
    inc si
    loop showtime_s

    mov ax,cs
    mov ds,ax
    mov si,offset timestr - offset sysboot + 7e00h
    mov ax,0b800h
    mov es,ax
    mov bx,160*12
    mov di,26*2   
    call showline

    pop bx
    pop es
    pop ds
    pop ax
    pop cx
    pop di
    pop si
    ret

;bcdtochar功能:返回后ah为十位的ascii,al为个位的ascii
bcdtochar:  
    push cx

    mov ah,al   
    mov cx,4
    shr ah,cl;ah中存储十位BCD   
    and al,00001111b;al中存储个位BCD

    add ah,30h
    add al,30h;转换为ASCII
    
    pop cx
    ret


;clear_key_buffer功能:清空缓冲区
clear_key_buffer:
    push ax   
clear_key_buffer_s:
    ;防止标志位zf改变,这里重新拿取一次
    mov ah,1h
    int 16h
    jz clear_key_buffer_end;缓存区为空,直接返回
    mov ah,0
    int 16h
    jmp clear_key_buffer_s
    
clear_key_buffer_end:
    pop ax
    ret

;clear_changetime功能:将changetime表中的值赋为*,确保多次调用修改时间时不会错误地显示出上一次修改的值
;ds=0,si=changetime表头地址
clear_changetime:  
    push si
    push cx

    mov si,offset changetime - offset sysboot + 7e00h
    mov cx,12

clear_changetime_s:
    mov byte ptr ds:[si],'*'
    inc si
    loop clear_changetime_s
       
    pop cx
    pop si
    ret

;get_settime功能:将键盘读入的数字放入changetime中,显示在屏幕上
get_settime:
    mov si,offset changetime - offset sysboot + 7e00h
    ;注意：每次showline都会改变si的值
    mov bx,0;bx控制写入位置
    mov di,25*26;di是显示的列地址
get_settime_s:    
    mov ah,0
    int 16h
    cmp al,30h
    jb get_settime_notnum
    cmp al,39h
    ja get_settime_notnum
    call pushnum
    push bx
    mov bx,160*15;bx是显示的行地址
    mov si,offset changetime - offset sysboot + 7e00h;栈中未保存si 
    call showline
    pop bx 
    jmp get_settime_s
    
get_settime_notnum:
    cmp ah,1ch
    je get_settime_enter
    cmp ah,0eh
    je get_settime_backspace
    jmp get_settime_s

get_settime_enter:
    ret;返回到setclock的主界面
get_settime_backspace:
    call popnum
    push bx
    mov bx,160*15;bx是显示的行地址
    mov si,offset changetime - offset sysboot + 7e00h;栈中未保存si
    call showline   
    pop bx
    jmp get_settime_s

;pushnum:将数字写入changetime表中,ds=0,si=changetime表头,bx=索引值
pushnum:
    cmp bx,11
    ja pushnum_end
    mov si,offset changetime - offset sysboot + 7e00h;栈中未保存si
    mov ds:[si+bx],al
    inc bx
pushnum_end:
    ret

;popnum:在changetime表中弹出一个数字
popnum:
    cmp bx,1
    jb popnum_end
    dec bx
    mov si,offset changetime - offset sysboot + 7e00h;栈中未保存si
    mov byte ptr ds:[si+bx],'*'
popnum_end:
    ret

;setCMOSRAM:已经向changetime中写入了12byte的时间的ASCII码,将这些ASCII转换并写入CMOS的RAM中
;进入程序时ds=0,
setCMOSRAM:
    mov bx,offset timeadr - offset sysboot + 7e00h;cmos中时间的地址
    mov si,offset changetime - offset sysboot + 7e00h
    mov cx,6
setCMOSRAM_s:
    mov dx,ds:[si];dh:个位ascii,dl:十位ascii
    sub dx,3030h;十位个位转换为BCD码
    push cx
    mov cl,4
    
    shl dl,cl;BCD码高四位为十位,低四位为个位
    ;and dh,00001111b
   
    pop cx
    or dl,dh;dl中保存BCD码

    mov al,ds:[bx]
    out 70h,al
    mov al,dl
    out 71h,al

    inc bx
    add si,2
    loop setCMOSRAM_s
    ret
;---子程序end---

;--------主程序,写入0道0面2,3,4扇区end--------
code ends
end start