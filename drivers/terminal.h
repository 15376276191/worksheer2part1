#ifndef INCLUDE_TERMINAL_H
#define INCLUDE_TERMINAL_H

#include "types.h"

void terminal_init(void);
void terminal_run(void);
void terminal_process_command(const char* input);

#endif /* INCLUDE_TERMINAL_H */
