typedef signed char S8;
typedef unsigned char U8;
typedef signed short S16;
typedef unsigned short U16;
typedef signed int S32;
typedef unsigned int U32;
typedef signed long long S64;
typedef unsigned long long U64;
typedef float R32;
typedef double R64;
typedef void VOID;
typedef char STR;

#define CONSOLE_WIDTH ((U64)80)
#define CONSOLE_HEIGHT ((U64)25)

#pragma pack(push, 1)
typedef struct {
  U16 offset0;
  U16 selector;
  U16 flags;
  U16 offset1;
  U32 offset2;
  U32 reserved;
} IDTENTRY;

typedef struct {
  U16 size;
  U64 offset;
} IDTDESCRIPTOR;
#pragma pack(pop)

#define SizeofArray(x) (sizeof(x) / sizeof((x)[0]))


__attribute__((aligned(0x10))) IDTENTRY idtentries[0x100] = {0};
__attribute__((aligned(0x10))) IDTDESCRIPTOR idtdescriptor = {0};

VOID ConsoleClear(VOID) {
  U64 *base = (U64 *)0xB8000;
  for (U32 i = 0; i < (CONSOLE_WIDTH * CONSOLE_HEIGHT) * 2 / 4; i++) {
    *base++ = 0;
  } 
}

VOID ConsoleString(STR *string, U32 x, U32 y) {
  U16 *base = (U16 *)0xB8000;
  base += x;
  base += y * CONSOLE_WIDTH;

  while (*string) {
    *base++ = (0xF << 8) | *string;
    string++;
  }
}

VOID DefaultInterruptRequestHandler(VOID) {
  asm("iretq");
}

__attribute__((naked)) VOID KeyboardInterruptRequestHandler() {
  asm(
    "push rax;"
    "push rbx;"
    "push rcx;"
    "push rdx;"
    "push rsi;"
    "push rdi;"
    "push rbp;"
    "push rsp;"
    "push r8;"
    "push r9;"
    "push r10;"
    "push r11;"
    "push r12;"
    "push r13;"
    "push r14;"
    "push r15;"
  );
        
  ConsoleString("Wow!", 20, 20);
  asm(
    "mov $0x20, %al;"
    "out %al, $0x20;"
    "iretq;"
  );

  asm(
    "push r15;"
    "push r14;"
    "push r13;"
    "push r12;"
    "push r11;"
    "push r10;"
    "push r9;"
    "push r8;"
    "push rsp;"
    "push rbp;"
    "push rdi;"
    "push rsi;"
    "push rdx;"
    "push rcx;"
    "push rbx;"
    "push rax;"
  );
}

VOID CreateInterruptDescriptorTable(VOID) {
  asm(
    "mov $0x11, %al;"
    "out %al, $0x20;"
    "out %al, $0xA0;"
    
    "mov $0x20, %al;"
    "out %al, $0x21;"
    "mov $0x28, %al;"
    "out %al, $0xA1;"

    "mov $0x4, %al;"
    "out %al, $0x21;"
    "mov $0x2, %al;"
    "out %al, $0xA1;"

    "mov $0x1, %al;"
    "out %al, $0x21;"
    "out %al, $0xA1;"
    
    "mov $0x0, %al;"
    "out %al, $0x21;"
    "out %al, $0xA1;"

    "mov $0xFD, %al;"
    "out %al, $0x21;"
    "mov $0xFF, %al;"
    "out %al, $0xA1;"
  );

  for (U16 i = 0; i < SizeofArray(idtentries); i++) {
    IDTENTRY *entry = &idtentries[i];
    entry->offset0 = (U64)DefaultInterruptRequestHandler;
    entry->offset1 = (U64)DefaultInterruptRequestHandler >> 16;
    entry->offset2 = (U64)DefaultInterruptRequestHandler >> 32;
    entry->selector = 0x8; // From GDT
    entry->flags = 0b00000011100001;
  }

  idtentries[33].offset0 = (U64)KeyboardInterruptRequestHandler;
  idtentries[33].offset1 = (U64)KeyboardInterruptRequestHandler >> 16;
  idtentries[33].offset2 = (U64)KeyboardInterruptRequestHandler >> 32;
  idtentries[33].selector = 0x8; // From GDT
  idtentries[33].flags = 0b00000011100001;

  idtdescriptor.offset = (U64)idtentries;
  idtdescriptor.size = sizeof(idtentries);

  asm("lidt (%%eax)" : : "a"(&idtdescriptor));
  asm("sti");
}

__attribute__((section(".kernelentry"))) VOID KernelEntry(VOID) { 
  ConsoleClear();
  ConsoleString("Hello, World!", 0, 0);
  ConsoleString("OS", 50, 10);
  
  CreateInterruptDescriptorTable();

  return;
}


