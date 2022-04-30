org 0x7C00
bits 16

%define KERNEL 0x7E00
%define STACK 0x7C00

jmp MainEntry16

; data

GDT:
.START:
.NULL:
	dq 0

.CODE:
	dw 0xFFFF
	dw 0
	db 0
	db 0x9A
	db 0b11001111
	db 0

.DATA:
	dw 0xFFFF
	dw 0
	db 0
	db 0x92
	db 0b11001111
	db 0

.END:
.DESCRIPTOR:
	dw .END - .START + 1
	dd GDT

; code

MainEntry16:
	mov ax, 0
	mov es, ax

	mov ah, 0x2
	mov al, 0x20
	xor ch, ch
	mov cl, 0x2
	xor dh, dh
	mov bx, KERNEL
	int 0x13

	in al, 0x92
	or al, 0x2
	out 0x92, al

	cli
	lgdt [GDT.DESCRIPTOR]
	mov eax, cr0
	or eax, 0x1
	mov cr0, eax
	
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax

	jmp 0x8:MainEntry32

bits 32

MainEntry32:
	mov edi, 0x1000
	mov cr3, edi
	xor eax, eax
	mov ecx, 0x1000
	rep stosd
	mov edi, cr3

	mov dword [edi], 0x2000 | 0x3
	add edi, 0x1000
	mov dword [edi], 0x3000 | 0x3
	add edi, 0x1000
	mov dword [edi], 0x4000 | 0x3
	add edi, 0x1000

	mov ebx, 0x3
	mov ecx, 0x200

.PtEntry:
	
	mov dword [edi], ebx
	add ebx, 0x1000
	add edi, 0x8
	dec ecx
	jnz .PtEntry

	mov eax, cr4
	or eax, 1 << 5
	mov cr4, eax

	mov ecx, 0xC0000080
	rdmsr
	or eax, 1 << 8
	wrmsr

	mov eax, cr0
	or eax, 1 << 31
	mov cr0, eax

	mov byte [GDT.CODE + 0x6], 0b10101111	
	mov byte [GDT.DATA + 0x6], 0b10101111	
	lgdt [GDT.DESCRIPTOR]

	call 0x8:MainEntry64

bits 64

MainEntry64:
	mov rbp, STACK
	mov rsp, rbp

	mov rax, cr0
	and rax, ~0b00001100
	mov cr0, rax
	finit

	call KERNEL
	jmp $

times 510 - ($ - $$) db 0
dw 0xAA55
