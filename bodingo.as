MAIN:   add r3, LIST
LOOP:   prn #48
        leap STR, r6
        macr macro 
        cmp r3, #-6
        END
        endmacr
;        ;inc #6
        mov    *r6,K
        sub r1, r4
        macro
        dec K
        macro
        jmp LOOP
END:     stop
STR:    .strig "abcd
LIST:    .data 6, -9
LIST:        .data -100
K:       .data 31
        .extern LIST
