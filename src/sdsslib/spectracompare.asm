
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
; a0         = rcx
; a1         = rdx
; result     = r8 
; numsamples = r9 
		
		SaveGPR
		
		push rdi
		push rsi

		mov	rdi, rcx					; a0 -> rdi
		mov rsi, rdx					; a1 -> rsi
		mov rcx, r9						; numsamples -> rcx
		mov rdx, r8						; result -> rdx
		shr rcx, 3 						; numsamples / 8
		xorps xmm0, xmm0				; xmm0 = 0


loop1:
		prefetcht0 [rsi+4*4*64]			; prefetch a1
		prefetcht0 [rdi+4*4*64]			; prefetch a0

		movaps xmm1, [rdi+4*4*0]		; a0[0] -> xmm1
		movaps xmm2, [rsi+4*4*0]		; a1[0] -> xmm2
		movaps xmm3, [rdi+4*4*1]		; a0[4] -> xmm3
		movaps xmm4, [rsi+4*4*1]  		; a1[4] -> xmm4

		subps xmm1, xmm2				; xmm1 = xmm1-xmm2
		subps xmm3, xmm4				; xmm3 = xmm3-xmm4

		mulps xmm1, xmm1 				; xmm1^2
		mulps xmm3, xmm3 				; xmm3^2

		addps xmm0, xmm1 				; xmm0 += xmm1
		addps xmm0, xmm3 				; xmm0 += xmm3

		add rdi, 4*4*2 					; next 8 floats
		add rsi, 4*4*2 					; next 8 floats

		dec rcx
		jnz loop1

		movaps [rdx], xmm0
				
		pop rdi
		pop rsi
		
		RestoreGPR
		ret

spectraCompareX64 endp



public spectraAdaptX64 
.code
align 16
spectraAdaptX64 PROC frame ;a0:PTR, a1:PTR, adaptionRate:PTR, numSamples:QWORD
; a0           = rcx
; a1           = rdx
; adaptionRate = r8 
; numsamples   = r9 

		SaveGPR
		
		push rdi
		push rsi
		
		mov	rdi, rcx
		mov rsi, rdx
		mov rcx, r9
		mov rdx, r8

		shr rcx, 3
		movaps xmm0, [rdx]


loop1:
		prefetchnta [rsi+4*4*64]
		prefetchnta [rdi+4*4*64]

		movaps xmm1, [rdi+4*4*0]	; dst: m_Amplitude[c]
		movaps xmm2, [rsi+4*4*0]	; src: _spectra.m_Amplitude[c]
		movaps xmm3, [rdi+4*4*1]	; dst: m_Amplitude[c+4]
		movaps xmm4, [rsi+4*4*1]	; src:_spectra.m_Amplitude[c+4]

		subps xmm2, xmm1			; _spectra.m_Amplitude[c]-m_Amplitude[c]
		subps xmm4, xmm3			; _spectra.m_Amplitude[c+4]-m_Amplitude[c+4]

		mulps xmm2, xmm0			; *=_adaptionRate
		mulps xmm4, xmm0			; *=_adaptionRate

		addps xmm1, xmm2			; add to dst
		addps xmm3, xmm4			; add to dst

		movaps [rdi+4*4*0], xmm1    ; this is the slowest bit
		movaps [rdi+4*4*1], xmm3	; this is the slowest bit

		add rdi, 4*4*2
		add rsi, 4*4*2

		dec rcx
		jnz loop1
		
		pop rdi
		pop rsi
		
		RestoreGPR
		ret

spectraAdaptX64 endp


end

