; multi-segment executable file template.

data segment
    filename db "test.txt"      ;the file must be placed in "D:\emu8086\MyBuild" even I give the absolute address     
    filehandle dw ?
    string db 6144 dup(0)       ;store the original data(a string)
    num dw 1024 dup(0)          ;store the numbers to be sorted
ends

stack segment
    dw   128  dup(0)
ends

code segment
start:                             
    mov ax, data                ;set segment registers
    mov ds, ax
    mov es, ax                  ;these codes can't be deleted,or the segment regs will be wrong even I give the 'assume'.
               
    mov ah, 3dh
    lea dx, filename            ;it will be wrong if I replace "filename" with "name"!!!
    mov al, 2
    int 21h                     ;open the file, the file handle is stored in ax
    
    lea dx, string
    mov cx, 1024
    mov filehandle, ax
    mov bx, filehandle
    mov ah, 3fh
    int 21h                     ;read the file
    
    call string2nums
    
    call insertion_sort         
         
    call nums2string    
    
    mov ah, 3eh
    mov bx, filehandle
    int 21h                     ;close the file
                   
    mov ah, 1                   ; wait for any key....
    int 21h
    
    mov ax, 4c00h ; exit to operating system.
    int 21h
    


    
string2nums proc
    push dx                       ;counter for num
    push cx
    push bx                       ;store the num translated from string
    push ax
    mov dx, 0
    mov cx, -1                    ;counter for string   
    mov bx, 0
    mov ax, 0
scan1:
    inc cx  
    mov di,cx
    mov al, string [di]
    cmp al, 0
    je case1
    cmp al, 32
    je case2
    
    sub al,30h
    push ax
    push dx
    mov ax, 10
    mul bx
    mov bx, ax
    pop dx
    pop ax                     
    add bx, ax                  ;bx = bx * 10 + ax
    
    jmp scan1
case1:                          ;has arrived at the end of the string 
    mov di, dx
    mov num[di], bx
    add dx, 2
    jmp done1
case2:                          ;has read a number     
    mov di, dx
    mov num[di], bx
    add dx, 2 
    mov bx, 0
    jmp scan1
done1:        
    mov di, dx
    mov num[di], -1             ;add a sentinal in the end
    pop ax 
    pop bx
    pop cx 
    pop dx
    ret            
endp
     

proc insertion_sort
    push dx
    push cx
    push bx
    push ax
    
    mov cx, 0               ;counter1
loop1:
    inc cx
    mov di, cx
    add di, di
    mov bx, num[di]         ;bx = num[cx] = key = A[j]
    cmp bx, -1    
    je done2
    
    mov dx, cx
    dec dx                  ;counter2
loop2:    
    and dx,dx
    jl finished            
    mov di, dx
    add di, di
    mov ax, num[di]         ;ax = num[dx] = A[i]
    cmp ax, bx 
    jle finished
    mov num[di + 2], ax
    dec dx
    jmp loop2
finished:
    mov di, dx
    add di, 1
    add di, di
    mov num[di], bx    
    jmp loop1
done2:
    pop ax
    pop bx
    pop cx
    pop dx    
    ret
endp
 
proc nums2string
    push dx
    push cx
    push bx
    push ax
    
    mov cx, -2          ;counter1,index of the num array
scan2:
    add cx, 2
    mov di, cx
    mov bx, num[di]     ;bx stores the number
    cmp bx, -1
    je done3
    
    mov dx, 0           ;counter,the number of digits
loop3:    
    inc dx
    cmp bx, 10
    jl print            ;has finished a number
    mov ax, bx
    mov si, cx          ;save cx
    mov di, dx          ;save dx
    mov cx, 10                  
    mov dx, 0
    div cx
    mov bx, ax          ;bx = bx / 10
    push dx             ;push bx % 10
    mov cx, si
    mov dx, di
    jmp loop3
    
print:
    push bx
loop4:     
    mov di, dx          ;save dx
    pop dx
    add dl, 30h
    mov ah, 2
    int 21h
    mov dx, di
    dec dx
    je space            ;the number has been printed, now next number
    jmp loop4
space:
    push dx
    mov dl, 32
    mov ah, 2
    int 21h
    pop dx     
    jmp scan2
done3:
    pop ax
    pop bx
    pop cx
    pop dx
    ret
endp

ends
end start ; set entry point and stop the assembler.
    
