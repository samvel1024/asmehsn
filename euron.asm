%define EURON_ID r13
%define STACK_SIZE r12
%define CURR_CHAR r14
%macro case 2                           ; Switch case declaration macro
    cmp bl, %1
    je %2
%endmacro
%macro case_range 3                     ; Switch case for range of values like 0..9
    cmp bl, %1
    jl %%fail
    cmp bl, %2
    jg %%fail
    jmp %3
    %%fail:
%endmacro
%macro case_handler 1                   ; Case body declaration
    jmp read_prog_condition
    %1:
%endmacro
%macro stack_push 1                     ; Push/Pop to euron stack with counting the size
    push %1
    inc STACK_SIZE
%endmacro
%macro stack_pop 1
    pop %1
    dec STACK_SIZE
%endmacro
%macro index 3                          ; Get the relative address of [%2][%3] index in the matrix
    xor %1, %1
    mov %1, N
    imul %1, %2
    add %1, %3
%endmacro
%macro spinlock 0                       ;  while (meet[fr][to] > meet[to][fr]) {}

     %%spin:
    index rax, rdi, rsi
    index rbx, rsi, rdi
    mov rax, [meet + rax * 8]
    mov rbx, [meet + rbx * 8]
    cmp rax, rbx
    ja %%spin
%endmacro
%macro save 0                           ; Save and load all the used registers
    push r12
    push r13
    push r14
    push rbx
    push rdi
    push rsi
    push rdx
%endmacro
%macro load 0
    pop rdx
    pop rsi
    pop rdi
    pop rbx
    pop r14
    pop r13
    pop r12
%endmacro
%macro save_call 1                      ; Call a function with saving all used registers
    save
    call %1
    load
%endmacro
global euron, thread_meet
extern get_value, put_value

section .bss
    meet resq N*N
    buff resq N

section .text
thread_meet:
    push rbx
    spinlock                            ; while (meet[fr][to] > meet[to][fr]) {}

    mov qword [buff + rdi * 8], rdx     ; buff[fr] = msg;
    index rax, rdi, rsi
    inc qword [meet + rax * 8]          ; meet[fr][to]++;
    spinlock                            ; while (meet[fr][to] > meet[to][fr]) {}

    mov rax, qword [buff + rsi * 8]     ; int64_t ans = buff[to];
    index rbx, rdi, rsi
    inc qword [meet + rbx * 8]          ; meet[fr][to]++;
    pop rbx
    ret                                 ; return ans

euron:
    save

    mov EURON_ID, rdi
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
    stack_push EURON_ID
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
    mov rdi, EURON_ID
    stack_pop rsi
    save_call put_value

    ;***********************************;
    case_handler synch
    mov rdi, EURON_ID               ; fr = rdi
    stack_pop rsi                   ; to = rsi
    stack_pop rdx
    call thread_meet
    stack_push rax


    nop
    read_prog_condition:
    mov bl, [CURR_CHAR]
    inc CURR_CHAR
    cmp bl, 0
    jne read_prog

    stack_pop rax

    jmp clean_stack_condition               ; Clean the stack that was left after euron operations
    clean_stack:
    stack_pop rbx
    clean_stack_condition:
    cmp STACK_SIZE, 0
    jg clean_stack

    load
    ret
