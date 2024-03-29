[BITS 32]

; OS Entry Point
global start
start:
	; Call Main Kernel Function
	extern kmain
	call kmain
