#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802BFD0.
 * sub_0802BFD0 @ 0x0802BFD0
 */

#include "proc.h"
/* gUnknown_08090BD0 and gUnknown_08090BD4 are agbcc -fforce-addr address
 * words, not globals: the ROM holds 0x08499590 and 0x03003100 at those two
 * addresses. gUnknown_08499590 is itself a pointer, so naming it honestly
 * gives the ROM's THREE-level read (pool word -> &gUnknown_08499590 -> the
 * map), while gUnknown_03003100 is the object and gives two levels. */
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

void StartSubmarineDiveEffect(int a1)
{
    if (!(gPlayers[gUnknown_030033EC].turnState & 2)
        && gMap->unk234A[
               gMap->rowOffset[gUnknown_03003100.pos.unk02]
               + gUnknown_03003100.pos.unk00] == 0)
        return;

    if ((u32)(gUnknown_03003100.pos.unk00
              - ((s16)gMap->scrollX >> 4)) > 0xe)
        return;

    if ((int)(gUnknown_03003100.pos.unk02
              - ((s16)gMap->scrollY >> 4)) < 0)
        return;

    if ((int)(gUnknown_03003100.pos.unk02
              - ((s16)gMap->scrollY >> 4)) > 9)
        return;

    if (a1 == 0)
        PlayMusicOrSfx2(0x37);

    if (a1 == 1)
        PlayMusicOrSfx2(0x36);

    Decompress((u8 *)gUnknown_081248F8, (void *)0x06013940);
    ApplyPaletteExt((u16 *)gUnknown_08125190, 0x260, 0x20);
    APProc_Create(gUnknown_08124FB8,
                 gUnknown_03003100.pos.unk00 * 16
                     - (s16)gMap->scrollX + 8,
                 gUnknown_03003100.pos.unk02 * 16
                     - (s16)gMap->scrollY + 8,
                 0x31CA, a1, 0);
    Proc_Start(gUnknown_0849A480, (ProcPtr)3);
}
asm(".global sub_0802BFD0\n.thumb_set sub_0802BFD0, StartSubmarineDiveEffect\n");
