; VALID EXAMPLE 1

            mov r1, *r2
            clr r3
NUMBERS:    .data 4, 5, -234
            jsr FUNC
            
; THIS IS A MACRO DEFINITION
            macr m_macr
            add #4, A
            sub r4, B
            sub A, B
            endmacr
            
FUNC:       red r6
            add r6, r2
            prn r6
            m_macr
            rts
            
            .extern B
            .entry NUMBERS
A:          .string "HI"

            m_macr
            .entry FUNC         