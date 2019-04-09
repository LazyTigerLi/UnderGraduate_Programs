; multi-segment executable file template.

data segment
    array db 36 dup(0)  ;emu8086 does not support dup(?)
ends

code segment  
    assume ds:data,cs:code
start:
    
    mov cx,36           ;counter
    lea bx,array        ;bx stores base address
    mov di,0
    mov al,1            ;al stores the number to be stored  
assign:
    mov [bx][di],al
    add di,1       
    add al,1       
    sub cx,1
    je  assign_finished
    jmp assign
assign_finished:
                  
    mov cx,7            ;counter1      
    mov bx,-6
print1:
    add bx,6
    dec cx
    je print_finished
    mov ax,7            ;counter2
    mov si,0
print2:
    mov dl,array[bx][si]
    cmp dl,30       
    jae gt30
    cmp dl,20
    jae gt20
    cmp dl,10
    jae gt10
    jmp gt0  
done:
    dec ax
    inc si
    cmp ax,cx
    je newline    
    jmp print2
 
gt0:
    add dl,30h
    push ax
    mov ah,2
    int 21h
    pop ax
    jmp space
gt30:
    push ax
    push dx
    mov dl,33h 
    mov ah,2
    int 21h
    pop dx
    add dl,18           ;-30+30h  
    int 21h
    pop ax 
    jmp space 
gt20:
    push ax
    push dx
    mov dl,32h 
    mov ah,2
    int 21h
    pop dx
    add dl,28           ;-20+30h  
    int 21h
    pop ax 
    jmp space 
gt10:
    push ax
    push dx
    mov dl,31h 
    mov ah,2
    int 21h
    pop dx
    add dl,38           ;-10+30h  
    int 21h
    pop ax 
    jmp space       
newline:                ;start a newline:"\r\n"   
    push ax
    mov dl,10
    mov ah,2
    int 21h 
    mov dl,13
    int 21h
    pop ax  
    jmp print1
space:
    mov dl,9
    push ax
    mov ah,2
    int 21h
    pop ax
    jmp done
       
print_finished: 
    ; wait for any key....    
    mov ah, 1
    int 21h
    
    mov ax, 4c00h ; exit to operating system.
    int 21h    
ends

end start ; set entry point and stop the assembler.
