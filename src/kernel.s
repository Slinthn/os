bits 64

global KeyboardInterruptRequestHandler
extern KeyboardService
KeyboardInterruptRequestHandler:
  push rax
  push rbx
  push rcx
  push rdx
  push rsi
  push rdi
  push rbp
  push rsp
  push r8
  push r9
  push r10
  push r11
  push r12
  push r13
  push r14
  push r15
    
  call KeyboardService
    
  mov al, 0x20
  out 0x20, al

  pop r15
  pop r14
  pop r13
  pop r12
  pop r11
  pop r10
  pop r9
  pop r8
  pop rsp
  pop rbp
  pop rdi
  pop rsi
  pop rdx
  pop rcx
  pop rbx
  pop rax

  iretq

global ConsoleSetCursorPosition:
ConsoleSetCursorPosition:
  mov dx, 0x3D4
  mov al, 0xF
  out dx, al

  mov dx, 0x3D5
  mov al, dil
  out dx, al

  mov dx, 0x3D4
  mov al, 0xE
  out dx, al

  mov dx, 0x3D5
  shr di, 8
  mov al, dil
  out dx, al

  ret

global ConsoleGetCursorPosition
ConsoleGetCursorPosition:
  mov dx, 0x3D4
  mov al, 0xE
  out dx, al

  mov dx, 0x3D5
  in al, dx
  mov bh, al

  mov dx, 0x3D4
  mov al, 0xF
  out dx, al

  mov dx, 0x3D5
  in al, dx
  mov bl, al
 
  xor rax, rax 
  mov ax, bx

  ret
