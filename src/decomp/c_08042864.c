#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08042864.
 * sub_08042864 @ 0x08042864
 */

/* MATCHED (wave 77, W77-I). Was parked at 98.6% since wave 35 with a two-byte
 * residual in the range test at the tail:
 *
 *     ROM:   cmp r0,#2 / bgt end / cmp r0,#1 / blt end
 *     draft: cmp r0,#2 / bgt end / cmp r0,#0 / ble end
 *
 * i.e. the draft's `if (m < 1) return;` came out as `m <= 0`. That is real and
 * unavoidable: agbcc's combine canonicalises LT against a positive constant to
 * LE against (constant - 1), so NO `if` spelling of the test can emit `blt #1`.
 * Waves 35 and 38 correctly ruled out every syntactic variant of the ifs
 * (`&&` both ways -- fold_truthop merges those into one `subs/cmp/bhi` range
 * test; the nested positive test, byte-identical; `m - 1 < 0`, one instruction
 * longer) and correctly identified that the lever had to make `m <= 0` and
 * `m < 1` non-equivalent to agbcc. They looked for it in the TYPE of m, where
 * it is not -- the ROM's `bgt`/`blt` are signed, so m is a plain signed int.
 *
 * THE LEVER IS THAT THIS IS A `switch`, NOT A PAIR OF `if`s. A switch's bound
 * tests are emitted by stmt.c's emit_case_nodes directly against the case
 * values, and they never pass through combine's comparison canonicalisation at
 * all -- so a case range of 1..2 emits the ROM's literal `cmp #2 / bgt` and
 * `cmp #1 / blt`, in that order, with the un-decremented constant. This is the
 * general rule and it transfers: WHERE A RANGE TEST'S CONSTANT SURVIVES
 * UN-CANONICALISED, THE SOURCE WAS A SWITCH.
 *
 * SETTLED IN EARLIER WAVES AND UNCHANGED: the map header must be a struct laid
 * over gUnknown_08499590 (the wave-34 W34-F rule) -- `(map + K) + idx` only
 * survives as a COMPONENT_REF. gUnknown_03003100 is read `.pos` (the UNSIGNED
 * view of the coordinate pair): both reads are `ldrh`. */
void LoadUnitIntoTransport(void)
{
    struct Unit *e;
    int m;

    e = &gUnits[
        gMap->unit[
            gMap
                ->rowOffset[gUnknown_03003100.pos.unk02]
            + gUnknown_03003100.pos.unk00]];

    ResetCaptureProgressIfMoved();

    if (e->unk07 != 0)
        e->unk08 = gUnknown_03003F38;
    else
        e->unk07 = gUnknown_03003F38;

    gUnknown_030040D8->unk01 |= 8;
    e->flags |= 0x10;

    m = gUnknown_030040D8->unk00;
    switch (m)
    {
    case 1:
    case 2:
        PlayMusicOrSfx2(0x4f);
        break;
    }
}
asm(".global sub_08042864\n.thumb_set sub_08042864, LoadUnitIntoTransport\n");
