public spectraCompareX64 
.code
align 16
spectraCompareX64 PROC a0:PTR, a1:PTR, result:PTR, numSamples:QWORD

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
		ret

spectraCompareX64 endp

end

