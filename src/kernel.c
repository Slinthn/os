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

#define CONSOLE_WIDTH 80
#define CONSOLE_HEIGHT 25

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

VOID __attribute__((section(".kernelentry"))) KernelEntry(VOID) { 
  ConsoleClear();
  ConsoleString("Hello, World!", 0, 0);
  ConsoleString("OS", 50, 10);
  return;
}


