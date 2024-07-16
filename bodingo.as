MAIN:   add r3, LIST
LOOP:   prn #48
        lea STR, r6
        macr macro 
        cmp r3, #-6
        bne n
        endmacr
;        ;inc #6
        mov    *r6,K
        sub r1, r4
        macro
        dec K
        macro
        jmp LOOP
END:     stop
STRSTRSR:    .string "abcd"
LIST:    .data 6, -9
        .data -100
    .extern POO
K:       .string 
