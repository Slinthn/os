void CommandClear(void) {
  ConsoleClear(&mainconsole, TRUE);
}

void CommandTime(void) {
  s8 buffer[32];
  TimeToString(buffer, sizeof(buffer));
  ConsoleString(&mainconsole, "The current time is: ", -1, TRUE);
  ConsoleString(&mainconsole, buffer, -1, TRUE);
}

void CommandHello(void) {
  ConsoleString(&mainconsole, "Hello", -1, TRUE);
}

void CommandBye(void) {
  ConsoleString(&mainconsole, "Bye", -1, TRUE);
}

command_t commands[] = {
  {"hello", CommandHello}, // TODO is the rest of the name array initialised to 0?
  {"bye", CommandBye},
  {"time", CommandTime},
  {"clear", CommandClear}
};

#define SHELL_MAX_COMMAND_SIZE 16
static s8 shellcommand[SHELL_MAX_COMMAND_SIZE + 1];
static u8 shellcommandsize = 0;

u8 IsPressableKey(u8 key) {
  if (key >= 'A' && key <= 'Z')
    return TRUE;

  return FALSE;
}

s8 ScancodeToCharacter(u16 scancode) {
  if (scancode > SizeofArray(scancodes))
    return 0;

  return scancodes[scancode];
}

u8 TranslateKey(u8 key) {
  if (key >= 'A' && key <= 'Z' && !(keymodifiers & KEY_UPPERCASE_DOWN))
    key += 0x20;

  return key;
}

void ShellBegin(void) {
  ConsoleString(&mainconsole, "Shell> ", -1, TRUE);
  shellcommandsize = 0;
}

void ShellFlush(void) {
  ConsoleNewLine(&mainconsole, TRUE);

  for (u64 i = 0; i < SizeofArray(commands); i++) {
    command_t *command = &commands[i];
    if (StringCompare(command->name, shellcommand, shellcommandsize))
      command->callback();
  }

  ConsoleNewLine(&mainconsole, TRUE);
  ShellBegin();
}

void ShellKeyboard(u8 scancode) {
  u8 key = ScancodeToCharacter(scancode);
  u8 pressed = scancode < 0x80;
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
      mainconsole.cursor -= 1;
      ConsoleChar(&mainconsole, NULL, -1, FALSE);
    }
  } break;

  case KEY_ENTER: {
    ShellFlush();
  } break;

  default: {
    if (!IsPressableKey(key))
      return;

    key = TranslateKey(key);

    if (shellcommandsize < SHELL_MAX_COMMAND_SIZE) {
      ConsoleChar(&mainconsole, key, -1, TRUE);
      shellcommand[shellcommandsize++] = key;
      shellcommand[shellcommandsize] = 0;
    }
  } break;
  }
}
