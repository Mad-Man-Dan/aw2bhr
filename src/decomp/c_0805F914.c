#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0805F914.
 * sub_0805F914 @ 0x0805F914
 */

/* Wave 55, W55-C. MATCHED, 604/604 bytes. RE-VERIFIED BY W55-F: trymatch
 * reports "MATCH -- links to identical bytes".
 * Promotion needs "rodata": ["0x0816DAB4", "0x0816DAB8", "0x0816DABC"]
 * (the -fforce-addr words for &gUnknown_085D5ABC, &gUnknown_030040D8 and
 * &gUnknown_08499590), then split_rodata.py + gen_lds.py.
 *
 * W55-F NOTE, because this function is the counter-example to a wave-55
 * mid-wave claim: it was circulated as a 13.7% MISS whose cause was "a local
 * .rodata literal written where the ROM names a global gUnknown_0816DABx". It
 * is neither. 13.7% was a stale score from before W55-C's fix, and the
 * `.rodata` relocations are the CORRECT output of the honest spelling --
 * trymatch resolves them against baserom.gba and prints
 *   "relocs: name different symbols that resolve to the same address ...
 *    The pool word is the same address spelled against a different symbol,
 *    which is a disassembly artefact, not a difference in the ROM."
 * Naming gUnknown_0816DABx as a global would be wrong; see the
 * gUnknown_081D9328 note in include/unknown-globals.h.
 *
 * TWO non-obvious things had to be right, and both were found by reading the
 * LITERAL POOL rather than the byte score:
 *
 * 1. THE OPERAND ORDER OF THE `&`. Writing the guard as
 *      (map->unk3262[...] & gUnknown_085D5ABC[p->unk00].unk1d) == 0
 *    puts gUnknown_08499590's pool word before gUnknown_085D5ABC's; the ROM has
 *    085D5ABC first. Pool words are emitted in order of FIRST REFERENCE, so the
 *    pool order reads out source evaluation order directly. Swapping to
 *      (gUnknown_085D5ABC[p->unk00].unk1d & map->unk3262[...]) == 0
 *    took this function from 13.7% to a match in one attempt -- every one of
 *    those "differing bytes" was the positional shift of `ldr rN,[pc,#X]`.
 *    Note the ROM's `ands r0, r1` leaves the result in the CELL's register even
 *    though the cell is operand 1, so do not try to read operand order off the
 *    destination register -- read it off the pool.
 *
 * 2. THE `v = 0` ARM IS THE ELSE. agbcc emits `if (c) X else Y` as
 *    jump-if-false-to-Y with X inline, so the ROM's `beq <block after the
 *    pool>` with the scoring arm inline means the source condition is the
 *    inverted one: `if (unk00 == 0 || tbl->unk1a != 0x10) v = <score>; else
 *    v = 0;`. The natural `&&` spelling with `v = 0` first is the same length,
 *    so this is invisible in a byte percentage.
 *
 * Also settled:
 *  - gUnknown_03003340's element is read two ways in one function:
 *    `((s8 *)g[y])[x]` (ldrsb) in the first scan, `(s8)g[y][x]` (ldrb +
 *    lsls/asrs) for the `best` store and throughout the second scan.
 *  - the second scan is AiFallbackMove's scoring idiom (W53-E):
 *    gUnknown_085D584C[terrain & 0x1f].unk00 * 10, compared signed.
 */

void AiRetreat(void)
{
    int best = 0x7fff;
    int selX = -1;
    int selY = 0;
    int best2 = 0;
    int x;
    int y;
    int v;

    AiBuildThreatPlane();

    if ((gUnknown_085D5ABC[gUnknown_030040D8->unk00].unk1d
         & gMap->dangerMask[
            gMap->rowOffset[gUnknown_030040D8->unk03]
            + gUnknown_030040D8->unk02]) == 0)
        return;

    GenerateUnitMovementMap(gUnknown_030040D8);

    for (y = 0; y < gMap->height; y++)
    {
        for (x = 0; x < gMap->width; x++)
        {
            if (((s8 *)gUnknown_03003340[y])[x] < 0)
                continue;
            if ((gUnknown_085D5ABC[gUnknown_030040D8->unk00].unk1d
                 & gMap->dangerMask[
                    gMap->rowOffset[y] + x]) != 0)
                continue;
            if (((s8 *)gUnknown_03003340[y])[x] > best)
                continue;
            if (!AiIsSettleCellOk(x, y))
                continue;
            best = (s8)gUnknown_03003340[y][x];
            selX = x;
            selY = y;
        }
    }

    if (selX != -1)
        AiPublishAction(selX, selY, 2, 0, 0);

    if (gUnknown_030045CC.unk00_1)
        return;

    selX = -1;

    for (y = 0; y < gMap->height; y++)
    {
        for (x = 0; x < gMap->width; x++)
        {
            if ((s8)gUnknown_03003340[y][x] < 0)
                continue;
            if (gUnknown_030040D8->unk00 == 0
             || gUnknown_085D5ABC[gUnknown_030040D8->unk00].deployLocation != 0x10)
                v = gUnknown_085D584C[
                        gMap->terrain[
                            gMap->rowOffset[y] + x]
                        & 0x1f].unk00 * 10;
            else
                v = 0;
            if (v < best2)
                continue;
            if (!AiIsSettleCellOk(x, y))
                continue;
            best2 = v;
            selX = x;
            selY = y;
        }
    }

    if (selX != -1)
        AiPublishAction(selX, selY, 2, 0, 0);
}
asm(".global sub_0805F914\n.thumb_set sub_0805F914, AiRetreat\n");
