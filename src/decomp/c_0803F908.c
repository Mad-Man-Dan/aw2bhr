#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803F908.
 * sub_0803F908 @ 0x0803F908
 */

/* WAVE 78 (W78-D): MATCHED configured, 136/136. Assigning the scaled screen
 * coordinates back into the x/y parameters, rather than fresh locals or
 * inline call arguments, gives them their own r4/r5 pseudos and produces the
 * two argument-register copies present in the ROM. The required force-address
 * pool entry remains at 0x08091308.
 *
 * HISTORICAL PARK -- wave 32, W32-B. 132 bytes against 136 (-4), 63.2%. Everything is
 * reproduced -- the three-level gUnknown_08499590 read through agbcc's own
 * -fforce-addr word at 0x08091308, the +0x417A rowOffset / +0x234A flag-plane
 * lookup, the `(u16)((a4 + 8) * 0x1000)` shift pair, the cross-jumped join --
 * except the two scaled coordinates.
 *
 * The ROM computes each into its OWN register and then copies it into the
 * argument register:
 *     subs r4, r1, r0   ...   adds r1, r4, #0
 *     subs r5, r1, r0   ...   adds r2, r5, #0
 * The candidate computes them straight into r1 and r2, which is two
 * instructions shorter. Binding them to `int` locals -- the lever that fixed
 * exactly this shape in DeathRayFire_StartBeam this wave -- does NOT reintroduce the
 * copies here (measured, both spellings): with only one call in the function
 * there is no reason for agbcc to keep them anywhere but the argument
 * registers, whereas in DeathRayFire_StartBeam the two calls ahead of the use forced it.
 *
 * So the ROM must be keeping both coordinates live across something this model
 * does not have. Most likely they are computed BEFORE the `oam2` if/else rather
 * than after it -- that would span the branch and force real registers. Worth
 * one probe next wave; that is where I would start.
 *
 * The x/y registers follow from the same decision (ROM r4/r5, candidate r5/r6),
 * so the percentage is one allocation decision renaming the rest. */
/* WAVE 77, W77-E -- SIGNATURE RECONCILED; THIS DRAFT HAD NEVER COMPILED.
 * Until now `u16 *obj` conflicted with include/unknown-functions.h, so every
 * screen in the project reported this function "NOT MEASURED" and no oracle
 * had ever judged the body above -- the 63.2% quoted in the wave-32 note is
 * from before the header gained its prototype.
 *
 * Changed on the DRAFT side, not the header: `u16 *obj` -> `const u8 *obj`.
 * The parameter is a pure pass-through (`mov ip, r2`, straight back out to
 * PutSprite), so both spellings are byte-neutral, and the 0x0849FAxx blobs are
 * already typed `const u8 []` in unknown-globals.h and passed that way by the
 * matched src/decomp/c_0803FC28.c. Agreeing with the promoted side costs
 * nothing; the cast below is where the real type shows. See the header note
 * for why those blobs are probably `const u16 []` and why that is a separate
 * job.
 *
 * The FIFTH parameter went the other way -- the header said `int`, the ROM's
 * own prologue does `ldr [sp,#0x18]; lsls #0x18; lsrs #0x18`, which is a
 * sub-word parameter's conversion and nothing else. Header corrected to u8. */
void PutMapObjectSprite(int x, int y, const u8 *obj, int a4, u8 a5)
{
    int oam2;

    if (a4 < 0)
    {
        oam2 = 0xC000;
    }
    else
    {
        oam2 = (u16)((a4 + 8) * 0x1000);

        if (a5 != 0
         && gMap->unk234A[
                gMap->rowOffset[y] + x] == 0)
            oam2 = 0xD000;
    }

    x = x * 16 - gMap->scrollX;
    y = y * 16 - gMap->scrollY;

    PutSprite(4, x, y, (u16 *)obj, oam2 + 0x48);
}
asm(".global sub_0803F908\n.thumb_set sub_0803F908, PutMapObjectSprite\n");
