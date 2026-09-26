#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08014074.
 * sub_08014074 @ 0x08014074
 */

/* struct Unk08014074 moved to include/unknown-globals.h in wave 21 (W21-A) --
 * gUnknown_0200C020 is the instance and sub_080147B4 the other accessor, so
 * the layout had to become shared. */

void sub_08014074(struct Unk08014074 *s)
{
    s->unk3a = 0;
    s->unk39 = -2;
}
