#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08017A0C.
 * sub_08017A0C @ 0x08017A0C
 */

/* Fills all 0x200 halfwords of the gUnknown_0849958C tilemap with 0x360, then
 * parks two volatile scroll registers at 0xFF60.
 *
 * The counter is s16: it carries the `lsl #16; asr #16` round trip at both the
 * index and the exit test. 0x360 is materialised as `movs #0xd8; lsls #2`
 * rather than a pool word, and the shared 0xFF60 pool word is copied once and
 * stored twice because both globals are `volatile u16`. */
void FillBlankBgTilemapAndSetScroll(void)
{
    s16 i;

    for (i = 0; i <= 0x1ff; i++)
        gUnknown_0849958C[i] = 0x360;

    gUnknown_0300309C = 0xff60;
    gUnknown_03002028 = 0xff60;
}
asm(".global sub_08017A0C\n.thumb_set sub_08017A0C, FillBlankBgTilemapAndSetScroll\n");
