void ConsoleFlush(console_t *console) {
  u16 *base = (u16 *)0xB8000;
  for (u32 i = 0; i < CONSOLE_WIDTH; i++, base++)
    *base = (0xF0 << 8) | console->header[i];

  for (u32 i = 0; i < CONSOLE_WIDTH * (CONSOLE_HEIGHT - 1); i++, base++)
    *base = (0xF << 8) | console->screen[i];

  ConsoleSetCursorPosition(console->cursor + CONSOLE_WIDTH);
}

void ConsoleClearHeader(console_t *console) {
  u64 *base = (u64 *)console->header;
  for (u32 i = 0; i < (CONSOLE_WIDTH / 8); i++) {
    *base++ = 0;
  }
}

void ConsoleClear(console_t *console, u16 movecursor) {
  u64 *base = (u64 *)console->screen;
  for (u32 i = 0; i < (CONSOLE_WIDTH * (CONSOLE_HEIGHT - 1)) / 8; i++) {
    *base++ = 0;
  }

  if (movecursor)
    console->cursor = 0;
}

void ConsoleString(console_t *console, s8 *string, u16 offset, u8 movecursor) {
  u8 *base = (u8 *)console->screen;
  if (offset == (u16)-1)
    offset = console->cursor;

  base += offset;

  while (*string) {
    *base++ = *string;
    string++;
  }

  if (movecursor)
    console->cursor = base - console->screen;
}

void ConsoleChar(console_t *console, s8 c, u16 offset, u8 movecursor) {
  if (offset == (u16)-1)
    offset = console->cursor;
 
  u8 *base = (u8 *)console->screen;
  base += offset;
  *base = c;

  if (movecursor)
    console->cursor += 1;
}

void ConsoleNewLine(console_t *console, u16 movecursor) {
  u16 offset = console->cursor;
  offset /= CONSOLE_WIDTH;
  offset += 1;
  offset *= CONSOLE_WIDTH;
  if (movecursor)
    console->cursor = offset;
}

void HeaderUpdate(void) {
  ConsoleClearHeader(&mainconsole);
 
  s8 buffer[32];

  u32 length = TimeToString(buffer, sizeof(buffer));
  s8 *bufferptr = buffer;
  for (s32 i = 0; *bufferptr; i++, bufferptr++) {
    mainconsole.header[CONSOLE_WIDTH - length + i] = *bufferptr;
  }

  s8 *str = "Fancy Operating System";
  for (u32 i = 0; *str; i++, str++) {
    mainconsole.header[i] = *str;
  }

  ConsoleFlush(&mainconsole); // TODO warning this is a bit of a hack. it refreshes the console automaticaly for me every time.
  //ConsoleString(&mainconsole, s32eger, CONSOLE_WIDTH - (s32eger - buffer) - 1, FALSE);
}
