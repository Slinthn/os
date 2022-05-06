bits 64

%macro pusha 0
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
%endmacro

%macro popa 0
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
%endmacro

global KeyboardInterruptRequestHandler
extern KeyboardService
KeyboardInterruptRequestHandler:
  pusha
 
  call KeyboardService
    
  mov al, 0x20
  out 0x20, al

  popa
  iretq

global TimerInterruptRequestHandler
extern TimerService
TimerInterruptRequestHandler:
  pusha
 
  call TimerService
    
  mov al, 0x20
  out 0x20, al

  popa
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




global PITSetup
PITSetup:
  cli

  mov al, 0b00110100
  out 0x43, al

  mov eax, 0x10000
  out 0x40, al
  mov al, ah
  out 0x40, al

  sti
  ret
