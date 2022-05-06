#include "type.h"

extern u16 ConsoleGetCursorPosition(void);
extern void ConsoleSetCursorPosition(u16 position);
extern void TimerInterruptRequestHandler(void);
extern void KeyboardInterruptRequestHandler(void);
extern void PITSetup(void);

#define KEYLIST 0x0, KEY_ESCAPE, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', KEY_BACKSPACE, KEY_TAB, 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '[', ']', KEY_ENTER, KEY_LCONTROL, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ';', 0x27, '#', KEY_LSHIFT, 0x5C, 'Z', 'X', 'C', 'V', 'B', 'N', 'M', ',', '.', '/', KEY_RSHIFT, '*', KEY_LALT, ' ', KEY_CAPSLOCK, KEY_F1, KEY_F2, KEY_F3, KEY_F4, KEY_F5, KEY_F6, KEY_F7, KEY_F8, KEY_F9, KEY_F10, KEY_NUMLOCK, KEY_SCROLLOCK, '7', '8', '9', '-', '4', '5', '6', '+', '1', '2', '3', '0', '.', 0, 0, 0, 0, KEY_F11, KEY_F12, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0

time_t SystemGetTime(void);
u8 scancodes[] = {KEYLIST, KEYLIST};
static u8 keymodifiers = 0;
console_t mainconsole = {0};

#include "string.c"
#include "time.c"
#include "console.c"
#include "shell.c"
#include "interrupt.c"

__attribute__((section(".kernelentry"))) void KernelEntry(void) { 
  ConsoleClear(&mainconsole, TRUE);
  
  CreateInterruptDescriptorTable();

  PITSetup();
  ShellBegin();

  return;
}


