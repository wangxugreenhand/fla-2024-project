;make true the first programme
; Input: a string of a^ib^j(i,j > 0), e.g. 'aaabbbb'

; the finite set of states
#Q = {0,mv_ar,mv_br,rm_b,mv_l,cp,fin,ans,stop,r1,r2,r_1,r_2,r_3,r_4,r_5,r_6,r_7,r_8,r_9,r_10,r_11,r_12,r_13,r_rm}

; the finite set of input symbols
#S = {a,b}

; the complete set of tape symbols
#G = {a,b,c,_,i,l,e,g,n,p,u,t} ;include "illegal_input"   

; the start state
#q0 = 0

; the blank symbol
#B = _

; the set of final states
#F = {stop}   

; the number of tapes
#N = 2    ; 第二条用于暂时保存“c”

; the transition functions

; State 0: start state
0 a_ a_ ** mv_ar
0 __ __ ** r1  ;in the first position ,refuse 
0 b_ b_ ** r2 

;state mv_ar： the first time move
mv_ar a_ a_ r* mv_ar
mv_ar b_ b_ ** mv_br  ;move to  the last "b"
mv_ar __ __ ** r2

;state mv_br: move to the last "b"  
mv_br b_ b_ r* mv_br
mv_br __ __ l* rm_b
mv_br a_ a_ ** r2

;state rm_b: remove the last "b"
rm_b b_ __ l* mv_l

;state mv_l: move to the first a
mv_l *_ *_ l* mv_l
mv_l __ __ r* cp

;state cp: cp the "c"
cp a_ ac rr cp
cp b_ b_ ** mv_br
cp __ __ ll fin  ;finished

;state r1: no valid symbol
r1 __ i_ r* r_1
r_1 __ l_ r* r_2
r_2 __ l_ r* r_3
r_3 __ e_ r* r_4
r_4 __ g_ r* r_5
r_5 __ a_ r* r_6
r_6 __ l_ r* r_7
r_7 __ __ r* r_8
r_8 __ i_ r* r_9
r_9 __ n_ r* r_10
r_10 __ p_ r* r_11
r_11 __ u_ r* r_12
r_12 __ t_ r* r_13

;state r2: have one or more symbols
r2 *_ *_ r* r2
r2 __ __ l* r_rm  ;remove symbols (a or b)
r_rm *_ __ l* r_rm
r_rm __ __ r* r1

;state fin: move to the first "a"
fin ac _c l* fin
fin _c _c r* ans

;state ans: copy "c" in the second tape to the first tape
ans _c cc rl ans
ans __ __ ll stop








