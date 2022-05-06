u8 RTCGetRegister(u32 reg) {
  u8 ret;
  asm(
      "out %0, $0x70;"
      "in $0x71, %0"
      : "=r"(ret) : "r"(reg)
  );
  return ret;
}

u32 RTCUpdateInProgress(void) {
  return RTCGetRegister(0xA) & 0x80;
}


u32 TimeToString(s8 *buffer, u32 size) {
  time_t time = SystemGetTime();

  s8 *bufferend = buffer + size;
  s8 *integer = buffer + IntegerToAscii(buffer, size, time.hour, 10, 2);
  *integer++ = ':';
  integer += IntegerToAscii(integer, bufferend - integer, time.minute, 10, 2);
  *integer++ = ':';
  integer += IntegerToAscii(integer, bufferend - integer, time.second, 10, 2);

  *(integer) = 0;
  return integer - buffer;
}

time_t SystemGetTime() {
  while (RTCUpdateInProgress());

  u8 last_second, last_minute, last_hour, last_day, last_month, last_year, registerB;

  u8 second, minute, hour, day, month;
  u32 year;

  second = RTCGetRegister(0);
  minute = RTCGetRegister(0x2);
  hour = RTCGetRegister(0x4);
  day = RTCGetRegister(0x7);
  month = RTCGetRegister(0x8);
  year = RTCGetRegister(0x9);

  do {
    last_second = second;
    last_minute = minute;
    last_hour = hour;
    last_day = day;
    last_month = month;
    last_year = year;
    
    while (RTCUpdateInProgress());

    second = RTCGetRegister(0);
    minute = RTCGetRegister(0x2);
    hour = RTCGetRegister(0x4);
    day = RTCGetRegister(0x7);
    month = RTCGetRegister(0x8);
    year = RTCGetRegister(0x9);
  } while (last_second != second || last_minute != minute || last_hour != hour || last_day != day || last_month != month || last_year != year);

  registerB = RTCGetRegister(0xB);

  if (!(registerB & 0x4)) {
    second = (second & 0xF) + ((second / 16) * 10);
    minute = (minute & 0xF) + ((minute / 16) * 10);
    hour = ((hour & 0xF) + (((hour & 0x70) / 16) * 10)) | (hour & 0x80);
    day = (day & 0xF) + ((day / 16) * 10);
    month = (month & 0xF) + ((month / 16) * 10);
    year = (year & 0xF) + ((year / 16) * 10);
  }

  if (!(registerB & 0x2) && (hour & 0x80))
    hour = ((hour & 0x7F) + 12) % 24;

  year += (2022 / 100) * 100;
  if (year < 2022) year += 100;


  time_t time;
  time.second = second;
  time.minute = minute;
  time.hour = hour;
  time.day = day;
  time.month = month;
  time.year = year;
  return time;
}
