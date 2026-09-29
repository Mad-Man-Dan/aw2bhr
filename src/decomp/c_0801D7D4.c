#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801D7D4.
 * sub_0801D7D4 @ 0x0801D7D4, sub_0801D7EC @ 0x0801D7EC, sub_0801D804 @ 0x0801D804, sub_0801D81C @ 0x0801D81C
 */

/* sub_0801D804 with argument 4 kept rather than forced to 0. */
int StartSpriteScript(int a, int b, int c, int d)
{
    return StartSpriteScriptInRange(a, b, c, d, 0, 0x1d);
}
asm(".global sub_0801D7D4\n.thumb_set sub_0801D7D4, StartSpriteScript\n");

/* Five parameters: the fifth arrives at [sp,#0x10], which is the first incoming
 * stack slot once two registers are pushed and 8 bytes of outgoing space are
 * reserved. Argument 4 of the callee is forced to 0 and this function's own
 * fourth and fifth become the callee's fifth and sixth. */
int sub_0801D7EC(int a, int b, int c, int d, int e)
{
    return StartSpriteScriptInRange(a, b, c, 0, d, e);
}

/* Three parameters -- it writes r3 itself before the call, which is what
 * separates it from StartSpriteScript. */
int sub_0801D804(int a, int b, int c)
{
    return StartSpriteScriptInRange(a, b, c, 0, 0, 0x1d);
}

/* The slot's unk38 is an s16 sentinel: -1 means "no id", anything else is the
 * id EndSlotScriptAt stops. `gUnknown_0200E438[a].unk38` is named twice rather
 * than bound to a local -- binding it would have hoisted the element address
 * out of the branch. */
void EndSpriteScript(int a)
{
    if (gUnknown_0200E438[a].unk38 != -1)
        EndSlotScriptAt(gUnknown_0200E438[a].unk38);
    else
        FreeSpriteScript(a);
}
asm(".global sub_0801D81C\n.thumb_set sub_0801D81C, EndSpriteScript\n");
