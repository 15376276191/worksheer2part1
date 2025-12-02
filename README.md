## Worksheet 2 Part 1

### Task 1: Booting a Tiny OS
- **Description:** Developed a minimal operating system that boots from scratch using GRUB bootloader
- **Key Features:**
  - Multiboot-compliant kernel
  - Custom linker script for memory layout
  - ISO image generation for booting
- **Files:**
  - `source/loader.asm` - Boot loader in assembly
  - `source/link.ld` - Linker script
  - `iso/boot/grub/menu.lst` - GRUB configuration

**Screenshots:**
![Task 1.1](task1.1.png) - GRUB boot menu  
![Task 1.2](task1.2.png) - Kernel loading process  
![Task 1.3](task1.3.png) - Successful boot verification

### Task 2: Calling C from Assembler
- **Description:** Extended the kernel to support calling C functions from assembly code
- **Key Features:**
  - Mixed assembly and C programming
  - Function parameter passing between languages
  - Multiple custom C functions implementation
- **Implemented Functions:**
  - `sum_of_three(int a, int b, int c)` - Returns sum of three integers
  - `multiply(int a, int b)` - Returns product of two integers  
  - `factorial(int n)` - Returns factorial of a number

**Screenshots:**
![Task 2.1](task2.1.png) - Function implementation  
![Task 2.2](task2.2.png) - Assembly to C calling mechanism  
![Task 2.3](task2.3.png) - Compilation process  
![Task 2.4](task2.4.png) - Function testing output  
![Task 2.5](task2.5.png) - Memory verification  
![Task 2.6](task2.6.png) - Complete test suite

### Task 3: Framebuffer Driver Development
- **Description:** Implemented a complete framebuffer driver for text output with full 2D API
- **Key Features:**
  - Memory-mapped I/O for framebuffer access
  - I/O port control for hardware cursor
  - Complete 2D text rendering API
  - Color support and cursor positioning

**Screenshots:**
![Task 3.1](task3.1.png) - Successful compilation with framebuffer support
![Task 3.2](task3.2.png) - File structure showing driver implementation
![Task 3.3](task3.3.png) - Complete test output showing all functionality working


## Worksheet 2 Part 2

### Task 1: Keyboard Input & Interrupt Handling
- **Description:** Implemented keyboard input handling through hardware interrupts using the Programmable Interrupt Controller (PIC)
- **Key Features:**
  - Interrupt Descriptor Table (IDT) setup for handling hardware interrupts
  - PIC initialization and remapping to avoid conflicts with CPU exceptions
  - Keyboard interrupt (IRQ1) handling and unmasking
  - Scan code to ASCII conversion with full US QWERTY keyboard support
- **Files Modified:**
  - `drivers/interrupts.h` - IDT structures and function declarations
  - `drivers/interrupts.c` - Interrupt handling implementation
  - `drivers/interrupt_asm.s` - Assembly interrupt handlers
  - `drivers/interrupt_handlers.s` - IDT loading functions
  - `drivers/pic.h`, `drivers/pic.c` - PIC configuration
  - `drivers/keyboard.h`, `drivers/keyboard.c` - Keyboard driver

**Screenshots:**

![Task 1.1](p2task1.1.png) - IDT initialization and basic interrupt setup  
![Task 1.2](p2task1.2.png) - Keyboard interrupt unmasking and PIC configuration  
![Task 1.3](p2task1.3.png) - Hardware interrupts successfully enabled  
![Task 1.4](p2task1.4.png) - Basic keyboard input test showing character echo

### Task 2: Input Buffer API Implementation
- **Description:** Created an efficient keyboard input buffer system with circular buffer management
- **Key Features:**
  - Circular buffer implementation for efficient input storage
  - Thread-safe buffer access with interrupt synchronization
  - Complete API for character and line input retrieval
  - Buffer overflow protection and error handling
- **API Functions:**
  - `u8int getc(void)` - Retrieves a single character from input buffer
  - `u32int readline(char* buffer, u32int max_length)` - Reads a complete line until Enter
- **Files Modified:**
  - `drivers/interrupts.c` - Added buffer management and API functions
  - `drivers/interrupts.h` - Added API function declarations

**Screenshots:**

![Task 2.1](p2task2.1.png) - `getc()` function implementation in interrupts.c  
![Task 2.2](p2task2.2.png) - `readline()` function implementation and buffer management  
![Task 2.3](p2task2.3.png) - Buffer API testing code in kernel.c  
![Task 2.4](p2task2.4.png) - Successful line input using `readline()` function

### Task 3: Terminal Implementation
- **Description:** Developed a basic Unix-like terminal interface with command processing capabilities
- **Key Features:**
  - Custom command prompt "myos>"
  - Command parsing with argument separation
  - Extensible command system with function table
  - Graceful error handling for unknown commands
  - 4 implemented commands (exceeding minimum requirement of 2)
- **Implemented Commands:**
  - `echo [text]` - Displays provided text
  - `clear` - Clears the terminal screen
  - `help` - Lists all available commands
  - `version` - Displays operating system version
- **Files Created:**
  - `drivers/terminal.h`, `drivers/terminal.c` - Terminal implementation
  - `drivers/string.h`, `drivers/string.c` - String utility functions

**Screenshots:**

![Task 3.1](p2task3.1.png) - Terminal startup with "myos>" prompt and initialization  
![Task 3.2](p2task3.2.png) - Help command showing all available commands  
![Task 3.3](p2task3.3.png) - Echo command demonstrating argument processing
