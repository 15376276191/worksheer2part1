#include "terminal.h"
#include "framebuffer.h"
#include "interrupts.h"
#include "string.h"

#ifndef NULL
#define NULL ((void*)0)
#endif

#define MAX_COMMAND_LENGTH 256
#define MAX_ARGUMENTS 10

struct command {
    char* name;
    void (*function)(char* args);
};

void cmd_echo(char* args);
void cmd_clear(char* args);
void cmd_help(char* args);
void cmd_version(char* args);

struct command commands[] = {
    {(char*)"echo", cmd_echo},
    {(char*)"clear", cmd_clear},
    {(char*)"help", cmd_help},
    {(char*)"version", cmd_version},
    {NULL, NULL}
};


void show_prompt(void) {
    fb_write("myos> ");
}

void terminal_init(void) {
    fb_write("\n=== Terminal Started ===\n");
    show_prompt();
}

void cmd_echo(char* args) {
    if (args[0] == '\0') {
        fb_write("Usage: echo [text]\n");
    } else {
        fb_write(args);
        fb_write("\n");
    }
}

void cmd_clear(char* args) {
    (void)args;
    fb_clear();
    show_prompt();
}

void cmd_help(char* args) {
    (void)args;
    fb_write("Available commands:\n");
    for (int i = 0; commands[i].name != NULL; i++) {
        fb_write("  ");
        fb_write(commands[i].name);
        fb_write("\n");
    }
}

void cmd_version(char* args) {
    (void)args;
    fb_write("MyOS Version 1.0\n");
}

void split_command(char* input, char* command, char* args) {
    int i = 0;
    int j = 0;
    
    while (input[i] == ' ') i++;
    
    while (input[i] != ' ' && input[i] != '\0' && input[i] != '\n') {
        command[j++] = input[i++];
    }
    command[j] = '\0';
    
    while (input[i] == ' ') i++;
    
    j = 0;
    while (input[i] != '\0' && input[i] != '\n') {
        args[j++] = input[i++];
    }
    args[j] = '\0';
}

void terminal_process_command(const char* input) {
    char command[MAX_COMMAND_LENGTH];
    char args[MAX_COMMAND_LENGTH];
    
    char input_copy[MAX_COMMAND_LENGTH];
    int i = 0;
    while (input[i] != '\0' && i < MAX_COMMAND_LENGTH - 1) {
        input_copy[i] = input[i];
        i++;
    }
    input_copy[i] = '\0';
    
    split_command(input_copy, command, args);
    
    if (command[0] == '\0') {
        show_prompt();
        return;
    }
    
    for (int i = 0; commands[i].name != NULL; i++) {
        if (strcmp(command, commands[i].name) == 0) {
            commands[i].function(args);
            show_prompt();
            return;
        }
    }
    
    fb_write("Unknown command: ");
    fb_write(command);
    fb_write("\nType 'help' for available commands.\n");
    show_prompt();
}

void terminal_run(void) {
    char input[MAX_COMMAND_LENGTH];
    
    while (1) {
        u32int len = readline(input, sizeof(input));
        
        if (len > 0) {
            terminal_process_command(input);
        }
    }
}
