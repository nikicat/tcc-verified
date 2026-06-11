/* Entry TU: linked against greet.o by the proven link step. */
#include "greet.h"

void _start(void)
{
    greet();
    sys3(60, 0, 0, 0); /* exit_group(0) */
    for (;;)
        ;
}
