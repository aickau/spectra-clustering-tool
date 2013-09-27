# High-performance spectra compare and adapt functions for X64 linux assembler
#
# extern "C" void spectraCompareX64(const float *a0, const float *a1, float *errout, size_t numsamples);

.global spectraCompareX64 

.text
spectraCompareX64:
# a0         = rdi
# a1         = rsi
# errout     = rdx 
# numsamples = rcx

		shr $3, %rcx				# numsamples /= 8
		xorps %xmm0, %xmm0


loop1:
		prefetcht0 4*4*64(%rsi)		# prefetch a1
		prefetcht0 4*4*64(%rdi)		# prefetch a0 

		movaps 4*4*0(%rdi), %xmm1 	# a0[0] -> xmm1
		movaps 4*4*0(%rsi), %xmm2	# a1[0] -> xmm2
		movaps 4*4*1(%rdi), %xmm3	# a0[4] -> xmm3
		movaps 4*4*1(%rsi), %xmm4	# a1[4] -> xmm4

		subps %xmm2, %xmm1			# xmm1 = xmm1 - xmm2
		subps %xmm4, %xmm3 			# xmm3 = xmm3 - xmm4

		mulps %xmm1, %xmm1 			# xmm1^2
		mulps %xmm3, %xmm3 			# xmm3^2

		addps %xmm1, %xmm0			# xmm0 += xmm1
		addps %xmm3, %xmm0			# xmm0 += xmm3

		add $4*4*2, %rdi 			# rdi += 32
		add $4*4*2, %rsi 			# rsi += 32

		dec %rcx					
		jnz loop1

		movaps %xmm0, (%rdx) 		# xmm0 -> errout
				
		ret


#extern "C" void spectraAdaptX64(const float *a0, const float *a1, float *adaptionRate, size_t numsamples);

.global spectraAdaptX64 

.text
spectraAdaptX64:

; a0           = rdi
; a1           = rsi
; adaptionRate = rdx 
; numsamples   = rcx


		shr $3, %rcx					# numsaples /= 8
		movaps (%rdx), %xmm0			# adaptionrate -> xmm0


loop2:
		prefetchnta 4*4*64(%rsi)		# prefetch a1
		prefetchnta 4*4*64(%rdi)		# prefetch a0 

		movaps 4*4*0(%rdi), %xmm1 		# dst: m_Amplitude[c]
		movaps 4*4*0(%rsi), %xmm2		# src: _spectra.m_Amplitude[c]
		movaps 4*4*1(%rdi), %xmm3		# dst: m_Amplitude[c+4]
		movaps 4*4*1(%rsi), %xmm4		# src:_spectra.m_Amplitude[c+4]

		subps %xmm1, %xmm2				# _spectra.m_Amplitude[c]-m_Amplitude[c]
		subps %xmm3, %xmm4				# _spectra.m_Amplitude[c+4]-m_Amplitude[c+4]

		mulps %xmm0, %xmm2				# *=_adaptionRate
		mulps %xmm0, %xmm4				# *=_adaptionRate

		addps %xmm2, %xmm1 				# add to dst
		addps %xmm4, %xmm3				# add to dst

		movaps %xmm1, 4*4*0(%rdi)   	# this is the slowest bit
		movaps %xmm3, 4*4*1(%rdi) 		# this is the slowest bit

		add $4*4*2, %rdi
		add $4*4*2, %rsi

		dec %rcx
		jnz loop2
		
		ret

