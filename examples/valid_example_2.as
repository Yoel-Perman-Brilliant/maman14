cmp A, B

A: red *r5

; r8 is a symbol, not a register
B: prn r8

.entry A
not r2

.data 100, 25, 25, 25, 0

X: inc r3
dec *r3
mov #-5, r5

sub #0, X

; This is considered to be a string whose content is: hello" "sd
.string "hello" "sd"

bne r8
stop
r8: jmp A
