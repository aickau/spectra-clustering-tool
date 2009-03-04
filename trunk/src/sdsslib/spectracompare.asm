
_STACKRESERVE    equ 64 ; Space to reserve on the stack to save GPRs.



;-------------------------------------------------------------------------------------------------------------------------
;   Save general purpose non-volatile registers.
;   Note the prolog statements for unwinding in the event of an exception.
;
;   For functions that do not need to save the general purpose registers we will add an
;   empty prolog statement, e.g. .allocstack 0 followed by .endprolog
;-------------------------------------------------------------------------------------------------------------------------
SaveGPR MACRO 

    sub rsp,_STACKRESERVE
   .allocstack _STACKRESERVE
    mov [rsp+56],r15
   .savereg r15,56
    mov [rsp+48],r14
   .savereg r14,48
    mov [rsp+40],r13
   .savereg r13,40
    mov [rsp+32],r12
   .savereg r12,32
    mov [rsp+24],rsi
   .savereg rsi,24
    mov [rsp+16],rdi
   .savereg rdi,16
    mov [rsp+8],rbx
   .savereg rbx,8
    mov [rsp+0],rbp
   .savereg rbp,0
   .endprolog

ENDM

;---------------------------------------------------------------------------------------------
;   Restore general purpose non-volatile registers.
;---------------------------------------------------------------------------------------------
RestoreGPR MACRO 

    mov r15,[rsp+56]
    mov r14,[rsp+48]
    mov r13,[rsp+40]
    mov r12,[rsp+32]
    mov rsi,[rsp+24]
    mov rdi,[rsp+16]
    mov rbx,[rsp+8]
    mov rbp,[rsp+0]
    add rsp,_STACKRESERVE
    
ENDM


public spectraCompareX64 
.code
align 16
spectraCompareX64 PROC frame ;a0:PTR, a1:PTR, result:PTR, numSamples:QWORD
; a0 = rcx
; a1 = rdx
; r8 = result
; r9 = numsamples
		
		SaveGPR
		
		push rdi
		push rsi

		mov	rdi, rcx
		mov rsi, rdx
		mov rcx, r9
		mov rdx, r8
		shr rcx, 3
		xorps xmm0, xmm0


loop1:
		prefetcht0 [rsi+4*4*64]
		prefetcht0 [rdi+4*4*64]

		movaps xmm1, [rdi+4*4*0]
		movaps xmm2, [rsi+4*4*0]
		movaps xmm3, [rdi+4*4*1]
		movaps xmm4, [rsi+4*4*1]

		subps xmm1, xmm2
		subps xmm3, xmm4

		mulps xmm1, xmm1
		mulps xmm3, xmm3

		addps xmm0, xmm1
		addps xmm0, xmm3

		add rdi, 4*4*2
		add rsi, 4*4*2

		dec rcx
		jnz loop1

		movaps [rdx], xmm0
				
		pop rdi
		pop rsi
		
		RestoreGPR
		ret

spectraCompareX64 endp

end

