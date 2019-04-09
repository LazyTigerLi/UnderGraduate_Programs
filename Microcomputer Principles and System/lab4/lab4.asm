; multi-segment executable file template.

data segment
    expr db  255,255 dup(0)             ;store the expresssion typed in
    opcode db 255 dup(0)                ;opcode stack
    oprand dw 255 dup(0)                ;oprand stack
    lastparen db 0                      ;indicate whether last character is ')'
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

    mov ah,0ah
    lea dx,expr
    int 21h                             ;input
    
    mov dx,0                            ;dx stores the number of opcodes
    mov ax,0                            ;ax stores the number of oprands      
    mov cx,1                            ;counter 
scan:
    inc cx
    mov di,cx
    mov bl,expr[di]                     ;bl stores the character
    cmp bl,13                           ;judge if it is the end of the expression
    je next

    cmp bl,0
    je num2str
    
    cmp bl,43
    je plus                             ;plus
    cmp bl,45                               
    je minus                            ;minus
    cmp bl,40
    je leftparen                        ;left paren
    cmp bl,41
    je rightparen                       ;right paren
    
    mov di,ax                           ;get the operand
    add di,di
    push dx
    push cx        
    push ax
    mov ax,oprand[di]
    mov cx,10
    mul cx 
    mov bh,0
    add ax,bx
    sub ax,30h
    mov oprand[di],ax
    pop ax
    pop cx
    pop dx
    
    jmp scan

next:
    mov bl,lastparen
    cmp bl,1
    je compute
    inc ax
    jmp compute
    
plus:
    mov lastparen,0
    inc ax
    cmp dx,0
    je opcode_push
    mov di,dx
    mov bh,opcode[di - 1]
    cmp bh,40                           ;the top of the opcode stack is '('
    je opcode_push
    jmp compute                         ;get two oprand and one opcode,push the result and the current opcode    

minus:
    mov lastparen,0 
    inc ax
    cmp dx,0
    je opcode_push
    mov di,dx
    mov bh,opcode[di - 1]
    cmp bh,40
    je opcode_push
    jmp compute

leftparen:  
    mov lastparen,0
    jmp opcode_push 

rightparen:
    mov bh,lastparen
    cmp bh,1
    je again3
    mov lastparen,1
    inc ax   
again3:
    mov di,dx
    mov bh,opcode[di - 1]
    cmp bh,40
    je done1    
    jmp compute_rightparen 
done1:    
    dec dx
    jmp scan
 
compute_rightparen:
    push ax
    push cx
    push dx
    mov di,ax
    sub di,1
    add di,di
    mov ax,oprand[di]
    mov cx,oprand[di - 2]
    push di                             ;or next instruction will cover di
    mov di,dx
    mov dl,opcode[di - 1]
    cmp dl,43
    je compute_plus_rightparen
    sub cx,ax
    pop di
    mov oprand[di - 2],cx
    mov oprand[di],0
    pop dx
    pop cx
    pop ax
    dec dx
    dec ax
    jmp again3    
compute_plus_rightparen:
    add cx,ax
    pop di
    mov oprand[di - 2],cx
    mov oprand[di],0
    pop dx
    pop cx
    pop ax
    dec dx
    dec ax
    jmp again3





    
opcode_push:
    mov di,dx
    mov opcode[di],bl
    inc dx
    jmp scan

compute:
    push ax
    push cx
    push dx
    mov di,ax
    sub di,1
    add di,di
    mov ax,oprand[di]
    mov cx,oprand[di - 2]
    push di                             ;or next instruction will cover di
    mov di,dx
    mov dl,opcode[di - 1]
    cmp dl,43
    je compute_plus
    sub cx,ax
    pop di
    mov oprand[di - 2],cx
    mov oprand[di],0
    pop dx
    pop cx
    pop ax
    dec dx
    dec ax
    jmp opcode_push    
compute_plus:
    add cx,ax
    pop di
    mov oprand[di - 2],cx
    mov oprand[di],0
    pop dx
    pop cx
    pop ax
    dec dx
    dec ax
    jmp opcode_push 
 
num2str:
    mov dl,61
    mov ah,2
    int 21h
    
    mov dx,0
    mov cx,10       
    mov bx,oprand[0]
    cmp bx,0
    jge again1
    mov ax,0
    sub ax,bx
    mov bx,ax
    mov dl,45
    mov ah,2
    int 21h
    mov dx,0  
again1:
    inc dx
    cmp bx, 10
    jl print
    mov ax,bx
    mov di,dx           ;save dx
    mov dx,0                 
    div cx
    mov bx,ax          ;bx = bx / 10
    push dx             ;push bx % 10 
    mov dx,di   
    jmp again1   
print:
    push bx
again2:
    mov di,dx     
    pop dx
    add dl, 30h
    mov ah, 2
    int 21h
    mov dx, di
    dec dx
    je done             ;the number has been printed
    jmp again2
    
done:   
    ; wait for any key....    
    mov ah, 1
    int 21h
    
    mov ax, 4c00h ; exit to operating system.
    int 21h    
ends

end start ; set entry point and stop the assembler.
