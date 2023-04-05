/*
 * shell.c
 * A simple shell for LUA-OS
 *
 * Copyright Joe Xue <lgxue@hotmail.com>
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#undef main
#define main(x, y) lua_main(x, y)
#include <lua.c>
#undef main

typedef struct cmd_tbl_s {
    char        *name;
    int         (*cmd)(int argc, char *argv[]);
    char        *help;
} cmd_tbl_t;

extern int echo_main(int argc, char **argv);
static int cmd_echo(int argc, char *argv[])
{
    return echo_main(argc, argv);
}

extern int ls_main(int argc, char **argv);
static int cmd_ls(int argc, char *argv[])
{
    return ls_main(argc, argv);
}

static int cmd_lua(int argc, char *argv[])
{
    return lua_main(argc, argv);
}

extern const cmd_tbl_t  cmd_table[];

static int cmd_help(int argc, char *argv[])
{
	int i;

	for (i = 0; cmd_table[i].cmd; i++ ) {
		printf(cmd_table[i].name);
		printf("      ");
		printf(cmd_table[i].help);
		printf("\n");
	}

    return 0;
}

const cmd_tbl_t  cmd_table[] = {
    {"echo",	cmd_echo,	" Echo parameters"},
    {"ls",      cmd_ls,     "   list the file/dir"},
    {"help",	cmd_help,	" Print command help info"},
    {"lua",     cmd_lua,    "  Run the lua interpreter"},
    {NULL,	NULL, NULL}
};

int cmd_process(int argc, char *argv[])
{
	int i;

	if (argc < 1) {
		return -1;
	}

	for (i = 0; cmd_table[i].cmd; i++ ) {
		if(!strcmp(cmd_table[i].name, argv[0])) {
			return cmd_table[i].cmd(argc, argv);
		}
	}

    if (!strcmp(argv[0], "\n")) {
        return 0;
    }

    if (argv[0][0] == '\0') {
        return 0;
    }

	printf("Error: command not found\n");
	return -1;
}

void shell(void)
{
#define LINE    1024
#define PARAMS  16
#define PROMPT  "LUA-OS # "
    char line[LINE];
    char *argv[PARAMS];
    int argc;
    int i;
    char c;

    for (;;) {
        memset(line, 0, LINE);
        printf("%s", PROMPT);
        fflush(stdout);
        if (fgets(line, LINE, stdin) != NULL) {
            argc = 0;
            i = 0;
            argv[argc] = line;
            do {
                c = line[i];
                if (c == ' ' || c == '\n') {
                    line[i] = '\0';
                    argc++;
                    argv[argc] = &line[i+1];
                }
                i++;

	        } while (c != '\0');

            cmd_process(argc, argv);
        }
    }
}
