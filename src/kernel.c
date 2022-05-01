#define KEY_ESCAPE 0x01
#define KEY_BACKSPACE 0x02
#define KEY_TAB 0x03
#define KEY_ENTER 0x04
#define KEY_LCONTROL 0x05
#define KEY_LSHIFT 0x06
#define KEY_RSHIFT 0x07
#define KEY_LALT 0x09
#define KEY_CAPSLOCK 0x0A
#define KEY_F1 0x0B
#define KEY_F2 0x0C
#define KEY_F3 0x0D
#define KEY_F4 0x0E
#define KEY_F5 0x0F
#define KEY_F6 0x10
#define KEY_F7 0x11
#define KEY_F8 0x12
#define KEY_F9 0x13
#define KEY_F10 0x14
#define KEY_F11 0x15
#define KEY_F12 0x16
#define KEY_SCROLLOCK 0x17
#define KEY_NUMLOCK 0x18

#define KEYLIST 0x0, KEY_ESCAPE, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', KEY_BACKSPACE, KEY_TAB, 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '[', ']', KEY_ENTER, KEY_LCONTROL, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ';', 0x27, '#', KEY_LSHIFT, 0x5C, 'Z', 'X', 'C', 'V', 'B', 'N', 'M', ',', '.', '/', KEY_RSHIFT, '*', KEY_LALT, ' ', KEY_CAPSLOCK, KEY_F1, KEY_F2, KEY_F3, KEY_F4, KEY_F5, KEY_F6, KEY_F7, KEY_F8, KEY_F9, KEY_F10, KEY_NUMLOCK, KEY_SCROLLOCK, '7', '8', '9', '-', '4', '5', '6', '+', '1', '2', '3', '0', '.', 0, 0, 0, 0, KEY_F11, KEY_F12, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0

unsigned char scancodes[] = {KEYLIST, KEYLIST};

#define TRUE 1
#define FALSE 0
#define NULL 0

#define CONSOLE_WIDTH ((unsigned long long)80)
#define CONSOLE_HEIGHT ((unsigned long long)25)
#define CONSOLECOORDS(x, y) ((y * CONSOLE_HEIGHT) + x)

#pragma pack(push, 1)
typedef struct {
  unsigned short offset0;
  unsigned short selector;
  unsigned char zero0;
  unsigned char type;
  unsigned short offset1;
  unsigned int offset2;
  unsigned int reserved0;
} idt_entry_t;

typedef struct {
  unsigned short size;
  unsigned long long offset;
} idt_descriptor_t;
#pragma pack(pop)

#define SizeofArray(x) (sizeof(x) / sizeof((x)[0]))

void ConsoleString(char *string, unsigned short offset, unsigned char movecursor);

void CommandHello(void) {
  ConsoleString("Hello", -1, TRUE);
}

void CommandBye(void) {
  ConsoleString("Bye", -1, TRUE);
}

typedef struct {
  char name[16];
  void (*callback)(void);
} command_t;

command_t commands[2] = {
  {"hello", CommandHello}, // TODO is the rest of the name array initialised to 0?
  {"bye", CommandBye}
};

extern unsigned short ConsoleGetCursorPosition(void);

extern void ConsoleSetCursorPosition(unsigned short position);

extern void KeyboardInterruptRequestHandler(void);

//__attribute__((aligned(0x10))) idt_entry_t idtentries[0x100] = {0};
//__attribute__((aligned(0x10))) idt_descriptor_t idtdescriptor = {0};

#define SHELL_MAX_COMMAND_SIZE 16
static char shellcommand[SHELL_MAX_COMMAND_SIZE + 1];
static unsigned char shellcommandsize = 0;

unsigned int StringCompare(char *str0, char *str1, unsigned int length) {
  if (length == 0)
    return 0;
  
  while (length--) {
    if (*str0++ != *str1++)
      return 0;
  }
  return *str0 == 0 && *str1 == 0;
}

void ConsoleClear(unsigned short movecursor) {
  unsigned long long *base = (unsigned long long *)0xB8000;
  for (unsigned int i = 0; i < (CONSOLE_WIDTH * CONSOLE_HEIGHT) * 2 / 4; i++) {
    *base++ = 0x0F000F000F000F00;
  }

  if (movecursor)
    ConsoleSetCursorPosition(0);
}

void ConsoleString(char *string, unsigned short offset, unsigned char movecursor) {
  unsigned short *base = (unsigned short *)0xB8000;
  if (offset == (unsigned short)-1)
    offset = ConsoleGetCursorPosition();

  base += offset;

  while (*string) {
    *base++ = (0xF << 8) | *string;
    string++;
  }

  if (movecursor) {
    ConsoleSetCursorPosition(((long long)base - 0xB8000) >> 1);
  }
}

void ConsoleChar(char c, unsigned short offset, unsigned char movecursor) {
  if (offset == (unsigned short)-1)
    offset = ConsoleGetCursorPosition();
 
  unsigned short *base = (unsigned short *)0xB8000;
  base += offset;
  *base = (0xF << 8) | c;

  if (movecursor) {
    ConsoleSetCursorPosition(offset + 1);
  }
}

void ConsoleNewLine(unsigned short movecursor) {
  unsigned short offset = ConsoleGetCursorPosition();
  offset /= CONSOLE_WIDTH;
  offset += 1;
  offset *= CONSOLE_WIDTH;
  if (movecursor)
    ConsoleSetCursorPosition(offset);
}

void DefaultInterruptRequestHandler(void) {
  asm("iretq");
}

char ScancodeToCharacter(unsigned short scancode) {
  if (scancode > SizeofArray(scancodes))
    return 0;

  return scancodes[scancode];
}

void ShellBegin(void) {
  ConsoleString("Shell> ", -1, TRUE);
  shellcommandsize = 0;
}

void ShellFlush(void) {
  ConsoleNewLine(TRUE);

  for (unsigned long long i = 0; i < SizeofArray(commands); i++) {
    command_t *command = &commands[i];
    if (StringCompare(command->name, shellcommand, shellcommandsize))
      command->callback();
  }


  ConsoleNewLine(TRUE);
  ShellBegin();
}

#define KEY_LSHIFT_DOWN 0x1
#define KEY_RSHIFT_DOWN 0x4
#define KEY_SHIFT_DOWN (KEY_LSHIFT_DOWN | KEY_RSHIFT_DOWN)
#define KEY_CAPSLOCK_DOWN 0x2
#define KEY_UPPERCASE_DOWN (KEY_SHIFT_DOWN | KEY_CAPSLOCK_DOWN)
static unsigned char keymodifiers = 0;

unsigned char IsPrintableKey(unsigned char key) {
  if (key >= 'A' && key <= 'Z')
    return TRUE;

  return FALSE;
}

unsigned char TranslateKey(unsigned char key) {
  if (key >= 'A' && key <= 'Z' && !(keymodifiers & KEY_UPPERCASE_DOWN))
    key += 0x20;

  return key;
}

void KeyboardService(void) {
  unsigned char scancode; 
  asm(
    "in $0x60, %0;"
    : "=r"(scancode)
  );


  unsigned char key = ScancodeToCharacter(scancode);
  unsigned char pressed = scancode < 0x80;
  switch (key) {
  case KEY_LSHIFT: {
    if (pressed)
      keymodifiers |= KEY_LSHIFT_DOWN;
    else
      keymodifiers &= ~KEY_LSHIFT_DOWN;
  } break;
  case KEY_RSHIFT: {
    if (pressed)
      keymodifiers |= KEY_RSHIFT_DOWN;
    else
      keymodifiers &= ~KEY_RSHIFT_DOWN;
  } break;

  case KEY_CAPSLOCK: {
    if (!pressed)
      keymodifiers ^= KEY_CAPSLOCK_DOWN;
  } break;
  }

  if (!pressed)
    return;

  switch (key) {
  case KEY_BACKSPACE: {
    if (shellcommandsize > 0) {
      shellcommand[shellcommandsize--] = 0;
      ConsoleSetCursorPosition(ConsoleGetCursorPosition() - 1);
      ConsoleChar(NULL, -1, FALSE);
    }
  } break;

  case KEY_ENTER: {
    ShellFlush();
  } break;

  default: {
    if (!IsPrintableKey(key))
      return;

    key = TranslateKey(key);

    if (shellcommandsize < SHELL_MAX_COMMAND_SIZE) {
      ConsoleChar(key, -1, TRUE);
      shellcommand[shellcommandsize++] = key;
      shellcommand[shellcommandsize] = 0;
    }
  } break;
  }
}

void CreateInterruptDescriptorTable(void) {
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

  idt_entry_t *idtentries = (idt_entry_t *)0x101000;
  idt_descriptor_t *idtdescriptor = (idt_descriptor_t *)0x100000;

  for (unsigned short i = 0; i < 0x100; i++) {
    idt_entry_t *entry = &idtentries[i];
    entry->offset0 = (unsigned long long)DefaultInterruptRequestHandler;
    entry->offset1 = (unsigned long long)DefaultInterruptRequestHandler >> 16;
    entry->offset2 = (unsigned long long)DefaultInterruptRequestHandler >> 32;
    entry->selector = 0x8; // From GDT
    entry->zero0 = 0;
    entry->type = 0x8E;
  }

  idtentries[33].offset0 = (unsigned long long)KeyboardInterruptRequestHandler;
  idtentries[33].offset1 = (unsigned long long)KeyboardInterruptRequestHandler >> 16;
  idtentries[33].offset2 = (unsigned long long)KeyboardInterruptRequestHandler >> 32;
  idtentries[33].selector = 0x8; // From GDT
  idtentries[33].zero0 = 0;
  idtentries[33].type = 0x8E;
  //idtentries[33].reserved0 = 0;
   
  idtdescriptor->offset = (unsigned long long)idtentries;
  idtdescriptor->size = 0x100 * sizeof(idt_entry_t) - 1;

  asm("lidt (%%eax)" : : "a"(idtdescriptor));
  asm("sti");
}

__attribute__((section(".kernelentry"))) void KernelEntry(void) { 
  ConsoleClear(TRUE);
  
  CreateInterruptDescriptorTable();

  ShellBegin();

  return;
}


