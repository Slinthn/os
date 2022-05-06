typedef unsigned char u8;
typedef char s8;
typedef unsigned short u16;
typedef short s16;
typedef unsigned int u32;
typedef int s32;
typedef unsigned long long u64;
typedef long long s64;

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

#define KEY_LSHIFT_DOWN 0x1
#define KEY_RSHIFT_DOWN 0x4
#define KEY_SHIFT_DOWN (KEY_LSHIFT_DOWN | KEY_RSHIFT_DOWN)
#define KEY_CAPSLOCK_DOWN 0x2
#define KEY_UPPERCASE_DOWN (KEY_SHIFT_DOWN | KEY_CAPSLOCK_DOWN)

#define TRUE 1
#define FALSE 0
#define NULL 0
#define SizeofArray(x) (sizeof(x) / sizeof((x)[0]))

#pragma pack(push, 1)
typedef struct {
  u16 offset0;
  u16 selector;
  u8 zero0;
  u8 type;
  u16 offset1;
  u32 offset2;
  u32 reserved0;
} idt_entry_t;

typedef struct {
  u16 size;
  u64 offset;
} idt_descriptor_t;
#pragma pack(pop)

typedef struct {
  s8 name[16];
  void (*callback)(void);
} command_t;

typedef struct {
  u8 second, minute, hour, day, month;
  u32 year;
} time_t;

#define CONSOLE_WIDTH ((u64)80)
#define CONSOLE_HEIGHT ((u64)25)
#define CONSOLECOORDS(x, y) ((y * CONSOLE_HEIGHT) + x)

typedef struct {
  u32 cursor;
  u8 header[CONSOLE_WIDTH];
  u8 screen[CONSOLE_WIDTH * (CONSOLE_HEIGHT - 1)];
} console_t;
