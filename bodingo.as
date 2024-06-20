MAIN:   add r3, LIST
LOOP:   prn #48
        macr m_macr
        cmp r3, #-6
        bne END
        endmacr
        lea STR, r6
        inc r6
        move    *r6,K
 ;HELLO:       sub r1, r4
        m_macr
               macr macr2
        whatupnewyork
        whatupmodiin
        whichisbetter
        endmacr
        m_macr
        macr2
        dec K
        jmp LOOP
END     stop
STR     .string "abcd"
LIST    .data 6, -9
        .data -100
K       .data 31