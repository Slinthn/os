u32 StringCompare(s8 *str0, s8 *str1, u32 length) {
  if (length == 0)
    return 0;
  
  while (length--) {
    if (*str0++ != *str1++)
      return 0;
  }
  return *str0 == 0 && *str1 == 0;
}

u32 IntegerToAscii(s8 buffer[], u32 size, s32 value, s32 base, u32 minlength) {
  s8 tmp[512] = {0};

  u32 i = sizeof(tmp) - 1;
  for (; (value && i) && i > 0; i--, value /= base)
    tmp[i] = "0123456789ABCDEF"[value % base];

  for (; minlength > (sizeof(tmp) - 1) - i; i--)
    tmp[i] = '0';

  for (u32 j = 0; j < (sizeof(tmp) - 1) - i && j < size; j++)
    buffer[j] = tmp[i + j + 1];

  return (sizeof(tmp) - 1) - i;
}

