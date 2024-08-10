macr macro1
    mov #5, #6
    dec HELLO
    add r4
    stop
    jsr LABEL 34
endmacr
    
macro1

macro1: jsr *r4
isThisAnOperand x, 5
clr
sub #g, r6

.data
NUMBERS: .data 4, 5, x
.data -3242343434
.data ,5
.data 6 7

.string "hhh
.string "

.extern NUMBERS
.entry SYMBOL
WHATSUP: .extern
G:
add r5, *r5 ;this adds stuff
