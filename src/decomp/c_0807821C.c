#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0807821C.
 * sub_0807821C @ 0x0807821C
 */

/* Reports bit 4 of gUnknown_08615194[i].unk02 as a bool. The 0x30 stride is
 * `lsls #1; adds rI,rI,r0; lsls #4` (x*3*16), and the byte member folds into
 * the `ldrb` displacement, which is what an array global's `g[i].member` does
 * for byte and halfword accesses.
 * The literal pool sitting BETWEEN the two return blocks is the if/return
 * form's signature -- a ternary would put it after the `bx lr`. */

bool8 MissionTriggersUnlockScript(int i)
{
    if (gUnknown_08615194[i].specialProperty & 0x10)
        return 1;

    return 0;
}
asm(".global sub_0807821C\n.thumb_set sub_0807821C, MissionTriggersUnlockScript\n");
