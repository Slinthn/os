ENTRY(KernelEntry)
OUTPUT_FORMAT("elf64-x86-64")

SECTIONS {
	.text : {
		*(.kernelentry)
		*(.text)
	}

	.data : {
		*(.data)
	}
}

