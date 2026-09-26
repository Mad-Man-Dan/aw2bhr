#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0800057C.
 * sub_0800057C @ 0x0800057C
 */

/*
 * sub_0800057C -- run one frame of the handler for the current map mode.
 *
 * gActiveMap->mode picks the handler; modes 4, 8 and 9 do nothing.
 *
 * Why the C looks odd: this spelling does not change what the code does, but
 * the original compiler only produces identical output with it.
 *   - `case 9:` must stay although its body is empty. The compiler sizes the
 *     jump table from the largest case label, so without it the table has
 *     eight entries instead of ten and the bounds check changes with it.
 */

void sub_0800057C(void)
{
    switch (gActiveMap->mode)
    {
    case 0:
        sub_080005FC();
        break;
    case 1:
        sub_0800081C();
        break;
    case 2:
        sub_08005F4C();
        break;
    case 3:
        sub_08004CA0();
        break;
    case 5:
        sub_08000694();
        break;
    case 6:
        sub_08000650();
        break;
    case 7:
        sub_08000664();
        break;
    case 9:
        break;
    }
}
