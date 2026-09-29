#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08074584.
 * sub_08074584 @ 0x08074584, IsPlayer1CoPowerReady @ 0x08074598, HasPlayer1CoPowerCharge @ 0x080745A8
 */

const struct Unk08074584 *GetMapEventTable(void)
{
    return GetMapListEntry(gPlaySt.mapID)->dialogueHeader;
}
asm(".global sub_08074584\n.thumb_set sub_08074584, GetMapEventTable\n");

/* The `lsls #0x18; lsrs #0x18` in front of the `pop` is THIS function's own u8
 * return narrowing, not a re-narrowing of IsCoPowerReady's -- agbcc emits the
 * epilogue pair for a declared-narrow return whether or not the value it is
 * returning is already narrow.
 */
u8 IsPlayer1CoPowerReady(void)
{
    return IsCoPowerReady(1);
}

asm(".global sub_08074598\n.thumb_set sub_08074598, IsPlayer1CoPowerReady\n");

/* `movs #0` / `movs #1` split across a `b` is the two-return spelling, not
 * `return f() != 0;`.  Which constant falls through fixes the sense: the ROM
 * reaches `movs r0, #0` by falling through, so the `return 1;` is the THEN arm
 * and the test is `!= 0`.
 */
bool8 HasPlayer1CoPowerCharge(void)
{
    if (GetCoPowerCharge(1) != 0)
        return 1;

    return 0;
}

asm(".global sub_080745A8\n.thumb_set sub_080745A8, HasPlayer1CoPowerCharge\n");
