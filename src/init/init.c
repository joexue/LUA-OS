#include <stdio.h>

extern unsigned char lua_os[];
extern int main(int argc, char *argv[]);

void init()
{
    char *argv[] = {"lua", "-e", (char *)lua_os, NULL};

    main(3, argv);

    printf("Exit the Lua OS\n");
    while(1) {
        ;
    }
}
