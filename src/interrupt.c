void DefaultInterruptRequestHandler(void) {
  asm("iretq");
}

void KeyboardService(void) {
  u8 scancode; 
  asm(
    "in $0x60, %0;"
    : "=r"(scancode)
  );

  ShellKeyboard(scancode);
}

void TimerService(void) {
  HeaderUpdate();
}

idt_entry_t idtentries[0x100] = {0};
idt_descriptor_t idtdescriptor = {0};

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

    "mov $0xFC, %al;"
    "out %al, $0x21;"
    "mov $0xFF, %al;"
    "out %al, $0xA1;"
  );

  for (u16 i = 0; i < 0x100; i++) {
    idt_entry_t *entry = &idtentries[i];
    entry->offset0 = (u64)DefaultInterruptRequestHandler;
    entry->offset1 = (u64)DefaultInterruptRequestHandler >> 16;
    entry->offset2 = (u64)DefaultInterruptRequestHandler >> 32;
    entry->selector = 0x8; // From GDT
    entry->zero0 = 0;
    entry->type = 0x8E;
  }

  idtentries[32].offset0 = (u64)TimerInterruptRequestHandler;
  idtentries[32].offset1 = (u64)TimerInterruptRequestHandler >> 16;
  idtentries[32].offset2 = (u64)TimerInterruptRequestHandler >> 32;

  idtentries[33].offset0 = (u64)KeyboardInterruptRequestHandler;
  idtentries[33].offset1 = (u64)KeyboardInterruptRequestHandler >> 16;
  idtentries[33].offset2 = (u64)KeyboardInterruptRequestHandler >> 32;
   
  idtdescriptor.offset = (u64)idtentries;
  idtdescriptor.size = 0x100 * sizeof(idt_entry_t) - 1;

  asm("lidt (%%eax)" : : "a"(&idtdescriptor));
  asm("sti");
}
