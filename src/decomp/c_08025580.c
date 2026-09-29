#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08025580.
 * sub_08025580 @ 0x08025580
 */

/* Save-call-restore around gUnknown_030032C0, and it is NOT void: the epilogue
 * is `pop {r1}; bx r1`, which leaves r0 alone, and the only thing in r0 is
 * GetNextReadyUnit's result. The restoring `strh` uses r4/r5 precisely so that the
 * returned pointer survives it. */

struct Unit *PeekNextReadyUnit(void)
{
    u16 saved = gUnknown_030032C0;
    struct Unit *r = GetNextReadyUnit();

    gUnknown_030032C0 = saved;

    return r;
}
asm(".global sub_08025580\n.thumb_set sub_08025580, PeekNextReadyUnit\n");
