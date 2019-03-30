%define N 3
%define STACK_SIZE r12
%define CURR_CHAR r14
%macro case 2
    cmp bl, %1
    je %2
%endmacro
%macro case_range 3
    cmp bl, %1
    jl %%fail
    cmp bl, %2
    jg %%fail
    jmp %3
    %%fail:
%endmacro
%macro case_handler 1
    jmp read_prog_condition
    %1:
%endmacro
%macro stack_push 1
    push %1
    inc STACK_SIZE
%endmacro
%macro stack_pop 1
    pop %1
    dec STACK_SIZE
%endmacro
%macro save_call 1
    push rbx
    push rdi
    push STACK_SIZE
    push CURR_CHAR

    call %1

    pop CURR_CHAR
    pop STACK_SIZE
    pop rdi
    pop rbx
%endmacro


global euron

extern get_value, put_value


section .text


; uint64_t euron(uint64_t n, char const *prog);
euron:
    push rdi                                ; Restore registers
    push CURR_CHAR
    push rbx

    mov CURR_CHAR, rsi
    mov STACK_SIZE, 0
    jmp read_prog_condition
    read_prog:

    case '+', plus
    case '-', minus
    case '*', mult
    case_range '0', '9', digit
    case 'n', ident
    case 'B', branch
    case 'C', clean
    case 'D', duplicate
    case 'E', exchange
    case 'G', get
    case 'P', popstack
    case 'S', synch

    ;***********************************;
    case_handler plus
    stack_pop rax
    stack_pop rbx
    add rax, rbx
    stack_push rax
    
    ;***********************************;
    case_handler minus
    stack_pop rax
    imul rax, -1
    stack_push rax
    
    ;***********************************;
    case_handler mult
    stack_pop rax
    stack_pop rbx
    imul rax, rbx
    stack_push rax
    
    ;***********************************;
    case_handler digit
    sub bl, '0'
    movzx rax, bl
    stack_push rax
    
    ;***********************************;
    case_handler ident
    stack_push rdi
    
    ;***********************************;
    case_handler branch
    stack_pop rbx
    stack_pop rax
    cmp rax, 0
    je no_jump
    add CURR_CHAR, rbx
    no_jump:
    stack_push rax
    
    ;***********************************;
    case_handler clean
    stack_pop rax
    
    ;***********************************;
    case_handler duplicate
    stack_pop rax
    stack_push rax
    stack_push rax
    
    ;***********************************;
    case_handler exchange
    stack_pop rax
    stack_pop rbx
    stack_push rax
    stack_push rbx
    
    ;***********************************;
    case_handler get
    save_call get_value
    stack_push rax
    
    ;***********************************;
    case_handler popstack
    stack_pop rsi
    save_call put_value

    ;***********************************;
    case_handler synch

    nop
    read_prog_condition:
    mov bl, [CURR_CHAR]
    inc CURR_CHAR
    cmp bl, 0
    jne read_prog

    stack_pop rax

    jmp clean_stack_condition
    clean_stack:
    stack_pop rbx
    clean_stack_condition:
    cmp STACK_SIZE, 0
    jg clean_stack

    pop rbx
    pop CURR_CHAR
    pop rdi
    ret
