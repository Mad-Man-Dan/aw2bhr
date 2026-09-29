#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802BC80.
 * sub_0802BC80 @ 0x0802BC80
 */

/* WAVE 35: CANONICAL `struct Map`. Eight drafts across blocks 0x08029-0x0802B
 * each invented their own body for this tag, with 2 to 7 named fields. Every
 * one compiles and byte-matches ALONE, so trymatch cannot see the problem;
 * merged into a promoted unit it is a hard `redefinition of struct Map`.
 * All seven distinct bodies describe the SAME layout at different
 * resolutions -- the offsets reconcile exactly -- so this is their union, the
 * most refined reading of each region. Byte-neutral: only a field's START
 * OFFSET enters the address arithmetic, never its declared length, and no
 * draft referenced any filler. Keep the drafts in sync; sync_work.py
 * reintroduces whatever the drafts say. */

void DrawDayAndFundsBarByCursor(void)
{
    u16 x;
    s16 v;

    x = gUnknown_030033E4.unk00 * 16 - gMap->scrollX;

    if ((s16)(gUnknown_030033E4.unk02 * 16
              - gMap->scrollY) <= 0x40
        && (s16)x <= 0x7f)
        v = gUnknown_08090A98[1];
    else
        v = gUnknown_08090A98[0];

    DrawDayAndFundsBar(v);
}
asm(".global sub_0802BC80\n.thumb_set sub_0802BC80, DrawDayAndFundsBarByCursor\n");
