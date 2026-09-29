#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08024720.
 * sub_08024720 @ 0x08024720
 */

/* WAVE 82 MATCHED (pool word +0x054 relocates to gUnknown_08090A30 via an
 * anonymous .rodata hop -- same-address pair trymatch resolves; promotion must
 * carry "rodata": ["0x08090A30"]). History in w82-snapshot.c: waves 39/73/81.
 * W82-B fix for the last two bytes of the 90.2% residual: the else arm binds p
 * AFTER materialising the second argument into a u16 temp (v = ...; then
 * p = gUnknown_0809139C;) -- the u16 type makes the argument truncation part of
 * the temp's own definition, so the base's pool ldr lands after it, exactly as
 * the ROM emits. The taken arm keeps the direct expression. j keeps two reaching
 * definitions ((j = idx) in the guard, -1 in the else) which is what preserves
 * the guard adds r0,r5,#0 copy and its double read by compare and shift. */
#include "global.h"

/* W82-B: the else-arm ordering residual is fixed by materialising the second
 * argument into a u16 temp BEFORE the p binding in the else arm (u16 makes the
 * truncation part of the temp's own definition, so the base's pool ldr lands
 * after it, exactly as the ROM emits); the taken arm keeps the direct
 * expression. j keeps two reaching definitions ((j = idx) in the guard, -1 in
 * the else) which is what preserves the guard's adds r0,r5,#0 copy and its
 * double read by compare + shift. */

void AnimatePowerActiveCoPalettes(void)
{
    u16 i;
    int idx;
    int j;
    u16 v;
    u16 *p;

    idx = ((u32)gGameClock >> 2) & 0xf;

    for (i = 1; i <= 4; i++)
    {
        if (gPlayers[i].aiControlled != 0)
        {
            if (IsCoPowerActive(i) && (j = idx) >= 0)
            {
                p = gUnknown_0809139C + j;
                ApplyPaletteAndUploadNow(p, (i + 0xb) * 32 + 0x1e, 2);
            }
            else
            {
                j = -1;
                v = (i + 0xb) * 32 + 0x1e;
                p = gUnknown_0809139C;
                ApplyPaletteAndUploadNow(p, v, 2);
            }
        }
    }
}
asm(".global sub_08024720\n.thumb_set sub_08024720, AnimatePowerActiveCoPalettes\n");
