/*
 * shell.c
 * A simple shell for LUA-OS
 *
 * Copyright Joe Xue <lgxue@hotmail.com>
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct cmd_tbl_s {
    char        *name;
    int         (*cmd)(int argc, char *argv[]);
    char        *help;
} cmd_tbl_t;

static int cmd_hello(int argc, char *argv[])
{
	printf("Hello LUA-OS\n");

	return 0;
}

static int cmd_echo(int argc, char *argv[])
{
    printf("%s\n", argv[1]);

	return 0;
}

extern int main (int argc, char **argv);
static int cmd_lua(int argc, char *argv[])
{
    return main(argc, argv);
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
    {"hello\n",	cmd_hello,	"Just print hello"},
    {"echo\n",	cmd_echo,	"Echo parameters"},
    {"?\n",	cmd_help,	    "Print command help info"},
    {"lua\n",   cmd_lua,    "Run the lua interpreter"},
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

    for (;;) {
        memset(line, 0, LINE);
        printf("%s", PROMPT);
        fflush(stdout);
        if (fgets(line, LINE, stdin) != NULL) {
            argv[0] = line;
            cmd_process(1, argv);
        }
    }
}
