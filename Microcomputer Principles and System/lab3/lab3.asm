; multi-segment executable file template.

data segment
    result db 10 dup(0ffh)                      ;store the result.Every byte stores two digits.  
    num db 4 (3,0,0,0)                          ;store the input string 
ends

stack segment
    dw   128  dup(0)
ends

code segment
start:
; set segment registers:
    mov ax, data
    mov ds, ax
    mov es, ax
    
    call input
    
    inc ax
    mov byte ptr result,1    
    mov cx,0
loop2:
    inc cx
    cmp cx,ax
    je done2 
    push ax
    mov ax,cx
    call multiply
    pop ax
    jmp loop2
    
done2:
    call print    
    ; wait for any key....    
    mov ah, 1
    int 21h
    mov ax, 4c00h ; exit to operating system.
    int 21h  
    
             
proc input
    push dx
    push bx
    mov ah,0ah 
    mov bx,0
    lea dx,num
    int 21h
    mov ax,0
    mov al,num[2]
    sub al,30h
    
    mov bl,num[3]
    cmp bl,0dh
    je done4
    sub bl,30h
    mov dl,10
    mul dl
    add al,bl
done4:
    push bx
    push dx
    ret
endp             
                      
proc multiply                                   ;multiplication for big inetgers
    push dx
    push cx
    push bx                                     ;al stores one of the operands,1 <= al <= 20
    push ax
    mov dl,100                                        
    mov bx,0                                    ;bl stores the carry
    mov cx,-1                                   ;counter
loop1:
    inc cx
    cmp cx,10
    je done1
    mov di,cx                                                                                               
    mov ah,byte ptr result[di]
    cmp ah,0ffh                                 ;invalid digit
    je done1                                    ;0ffh is a sentinal
    
    push ax                                     ;store al because the result of multiplication will cover ax
    mul byte ptr result[di]                     
    add ax,bx
    div dl
    mov bl,al                                   ;update the carry
    mov byte ptr result[di],ah                  ;store the result
    pop ax
    cmp bl,0
    je loop1
    push ax
    mov al,byte ptr result[di + 1]
    cmp al,0ffh
    je set
    pop ax
    jmp loop1
set:
    mov byte ptr result[di + 1],0
    pop ax
    jmp loop1    
done1:
    pop ax
    pop bx
    pop cx
    pop dx
    ret     
endp
    
proc print
    push dx
    push cx
    push bx
    push ax
    mov ah,2
    mov dl,13
    int 21h
    mov dl,10
    int 21h
    
    mov ax,0
    mov bl,10
    mov cx,10                                   ;counter
loop3:
    dec cx
    jl done3
    mov di,cx
    mov al,byte ptr result[di] 
    cmp al,0ffh
    je loop3
    div bl
    
    mov dl,al
    add dl,30h
    push ax
    mov ah,2
    int 21h
    pop ax
    mov dl,ah
    add dl,30h
    mov ah,2
    int 21h
    mov ax,0
    jmp loop3
done3:    
    pop ax
    pop bx
    pop cx
    pop dx
    ret
endp
    
      
ends
end start ; set entry point and stop the assembler.
