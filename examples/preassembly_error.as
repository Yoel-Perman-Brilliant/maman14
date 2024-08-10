macr macr1 .
    mov #-5, x
    VERY LONG LINE VERY LONG LINE VERY LONG LINE VERY LONG LINE VERY LONG LINE VE
    VERY LONG LINE BUT NOT QUITE AS LONG AS THE PREVIOUS ONE, JUST ONE CHAR LESS
endmacr

HELLO: macr macr2
    dec *r6
    r3 = r7
    lea r4, r3
j: endmacr

macr2
macr1 j

macr _macr
    
    
endmacr

                                                                            _macr
HELLO: _macr
