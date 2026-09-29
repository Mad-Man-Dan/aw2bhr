#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801A474.
 * sub_0801A474 @ 0x0801A474
 */

/* Draws the standard window frame into gUnknown_0849958C's tilemap buffer and
 * uploads it: the top strip, `h - 2` middle strips alternating between the two
 * table forms, the bottom strip, each one row (0x40 bytes) further on.
 *
 * gUnknown_0849958C is named HONESTLY, twice. agbcc's -fforce-addr then parks
 * its address in this unit's own .rodata, which is the ROM word at 0x0808E5C4,
 * and that is where the base's two `ldr r0,[r0]` come from -- there is no
 * second pointer variable. */
void DrawWindowFrameAndUpload(s16 a1, s16 a2, s16 a3, s16 a4)
{
    u16 *dst;
    u16 i, alt;

    dst = gUnknown_0849958C + a1 + a2 * 32;
    DrawWindowFrameTopRow(dst, a1, a3, 8);
    dst += 32;

    alt = 1;
    for (i = 0; i < a4 - 2; i++)
    {
        DrawWindowFrameMiddleRow(dst, a1, a3, alt, 8);
        dst += 32;
        alt = 1 - alt;
    }

    DrawWindowFrameBottomRow(dst, a1, a3, 8);

    CpuCopyAuto(gUnknown_0849958C, (void *)0x0600E800, 0x200);
}
asm(".global sub_0801A474\n.thumb_set sub_0801A474, DrawWindowFrameAndUpload\n");
