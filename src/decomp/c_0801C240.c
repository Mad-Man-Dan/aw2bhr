#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801C240.
 * sub_0801C240 @ 0x0801C240
 */

struct Unk_0801C240
{
    /* 00 */ void * unk00;
};

void sub_0801C240(struct Unk_0801C240 * a)
{
    if (a != NULL && a->unk00 != NULL)
        a->unk00 = NULL;
}
