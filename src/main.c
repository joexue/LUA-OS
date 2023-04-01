#include <stdio.h>

extern void shell(void);

void c_entry()
{
    shell();
    while(1) {
        ;
    }
}
