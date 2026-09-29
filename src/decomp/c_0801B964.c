#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801B964.
 * sub_0801B964 @ 0x0801B964, sub_0801B998 @ 0x0801B998
 */

/* Draws one run through PutCachedTextString and advances the two cursors it consumed
 * by however much the callee reported.
 *
 * The struct is a pointer parameter only, so it lives here rather than in
 * include/unknown-globals.h. unk32 is reached as `adds r2, #0x32; ldrb [r2]`
 * rather than a displaced load because Thumb's `ldrb` displacement stops at 31.
 *
 * THE HALFWORD SUM MUST BE ITS OWN STATEMENT. Folded into the argument list the
 * callee's second argument is evaluated first, gUnknown-style: `unk20` loads
 * into r1 before the two `ldrh`s, which then need r5 and a wider push. Bound to
 * a local, the sum is computed while r1 is still free and the ROM's register
 * assignment falls out. The operand order inside it is also fixed -- agbcc
 * loads the SECOND operand first, so `unk2c + unk2e` is what emits
 * `ldrh [#0x2e]` ahead of `ldrh [#0x2c]`. */
struct Unk1B998
{
    /* 00 */ u8 filler_00[0x20];
    /* 20 */ u32 unk20;
    /* 24 */ u8 filler_24[0x2c - 0x24];
    /* 2c */ u16 unk2c;
    /* 2e */ u16 unk2e;
    /* 30 */ u8 filler_30[0x32 - 0x30];
    /* 32 */ u8 unk32;
};

/* Advances a pair of counters in step and wraps only the first: returns 1 when
 * gUnknown_03000058 passed 7 and was pulled back by 8, 0 when it did not.
 *
 * `cmp #7; bgt` is a SIGNED compare, which is what makes both globals `int`
 * rather than unsigned; every access is a bare `ldr`/`str`, so they are words.
 * The wrap arm re-uses the value it just stored (`adds r0, r3, #0`) instead of
 * re-loading it, which is the ordinary non-volatile store-forwarding and rules
 * volatile out for gUnknown_03000058. */
int AdvanceTextPixelCursor(int a)
{
    gUnknown_03000058 += a;
    gUnknown_0300005C += a;

    if (gUnknown_03000058 > 7)
    {
        gUnknown_03000058 -= 8;
        return 1;
    }
    else
        return 0;
}
asm(".global sub_0801B964\n.thumb_set sub_0801B964, AdvanceTextPixelCursor\n");

void PutCachedTextRun(struct Unk1B998 *p, int b, u16 c)
{
    u16 width = p->unk2c + p->unk2e;
    int used = PutCachedTextString(b, p->unk20, width, c);

    p->unk32 += used;
    p->unk20 += used;
}
asm(".global sub_0801B998\n.thumb_set sub_0801B998, PutCachedTextRun\n");
