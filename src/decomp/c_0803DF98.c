#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803DF98.
 * sub_0803DF98 @ 0x0803DF98
 */

/* Fills a {u16, u16} pair out of a mode id in 2..8. void, not int: its caller
 * AddBlockedInventionRecord allocates the pair on its own stack and re-reads both halves
 * with `ldrh` after the `bl`, never touching r0.
 *
 * The three tails cross-jump automatically, so the only thing the source has
 * to get right is the ORDER of the case blocks -- GCC emits them in source
 * order, and putting the {1,1} arm second (the natural reading order of the
 * case labels) puts the two `strh`-sharing blocks the wrong way round.
 *
 * Wave 31: the out-parameter was `u16 *` here, which was a body-side spelling
 * -- writing two halfwords at +0 and +2 is the same code either way, so this
 * file had no oracle for it. `struct Unk02028360Pos *` is the settled model:
 * wave 29 established the struct from FindLivingInventionTargetAt, where the array spelling
 * measurably differs (it recomputes `sp + 2` into a scratch instead of reading
 * `ldrh r0,[r6,#2]`), and GetInventionTargetCell already takes it. Byte-for-byte
 * unchanged by the retype -- re-verified.
 */
void GetInventionOriginOffset(int a, struct Unk02028360Pos *out)
{
    switch (a)
    {
    case 3:
    case 5:
        out->unk00 = 1;
        out->unk02 = 1;
        break;

    case 2:
    case 7:
    case 8:
        out->unk00 = 1;
        out->unk02 = 2;
        break;

    default:
        out->unk00 = 0;
        out->unk02 = 0;
        break;
    }
}
asm(".global sub_0803DF98\n.thumb_set sub_0803DF98, GetInventionOriginOffset\n");
