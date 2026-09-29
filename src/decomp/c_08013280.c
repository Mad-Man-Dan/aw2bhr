#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08013280.
 * sub_08013280 @ 0x08013280
 */

#include "proc.h"
/* The same two-Proc_Break shape as the promoted WhiteFlash_FadeOut, with the counter
 * at +0x58 (a whole word) and the signed-halfword duration at +0x66 instead of
 * +0x5c / +0x68, and with no Interpolate in between -- a bare frame counter
 * that breaks when it reaches its limit.
 *
 * The duration is read TWICE through the same `adds r1, #0x66` base, which is
 * what fixes it as one field rather than two: agbcc keeps r1 live across the
 * branch. `ldrsh` makes it s16. */
struct Unk08013280
{
    /* 0x00 */ u8 filler_00[0x58];
    /* 0x58 */ int unk58;
    /* 0x5c */ u8 filler_5c[0x66 - 0x5c];
    /* 0x66 */ s16 unk66;
};

void WhiteFlash_Hold(struct Unk08013280 *proc)
{
    if (proc->unk66 == 0)
    {
        Proc_Break(proc);
    }
    else
    {
        if (++proc->unk58 == proc->unk66)
            Proc_Break(proc);
    }
}
asm(".global sub_08013280\n.thumb_set sub_08013280, WhiteFlash_Hold\n");
