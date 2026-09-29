#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0805F4F8.
 * sub_0805F4F8 @ 0x0805F4F8
 */

/* AiEmbarkOrFallback @ 0x0805F4F8, 476 bytes. MATCHED.
 *
 * The capture/repair-target chooser: it rebuilds the reachable-cell list, then
 * scans for the cheapest cell whose terrain low-5-bits are 0xd or 0xb, and
 * either hands the winning cell to AiAdvanceToward or moves onto it directly.
 * Every failure path calls AiFallbackMove, which longjmps out -- which is why
 * those calls read as unconditional statements with execution continuing after
 * them in the listing.
 *
 * gUnknown_0816DAAC is NOT a global: the ROM word there holds 0x030040D8, so it
 * is agbcc's own -fforce-addr constant for gUnknown_030040D8. The promotion
 * must carry it -- data/promoted.json needs "rodata": ["0x0816DAAC"].
 *
 * `cur` is a LOCAL 4-BYTE AGGREGATE of two u16, not a `u16 [2]`. The
 * discriminator is the word read-modify-write `ldr [sp,#4] / and 0xFFFF0000 /
 * orr 0x270F / str [sp,#4]`: a struct that fits in a word gets SImode, so a
 * member store goes through store_bit_field, while an ARRAY element store is a
 * plain `strh` through `mov r1,sp`. In the loop both halves are written in one
 * basic block and combine merges the two inserts into the single
 * `(u16)x | (y << 16)` word store; the `y << 16` is LICM-hoisted into `ip`.
 * Reads stay ordinary -- extract_bit_field narrows an aligned half back to a
 * HImode load -- and `AiPublishAction(cur.x, cur.y, ...)` gets `ldrsh` with no
 * cast in the source, because the u16 members convert to the callee's `s16`
 * parameters and combine folds the sign extension into the load.
 *
 * `cell` is BOUND and read as `*(s8 *)cell` twice, which is what makes those
 * two loads `ldrsb r0,[r4,r0]`; the third, unsigned read in the `best`
 * assignment is written as `gUnknown_03003340[y][x]` and recomputes its
 * address, exactly as the ROM does.
 *
 * OPERAND ORDER in the `best` assignment is load-bearing and cost an attempt:
 * `gUnknown_03003340[y][x] + unk3C72[...]` matches, the other way round does
 * not. It is not only the two address computations that swap -- writing it
 * backwards also flips `map` and the `& 0x1f` temporary between r1 and r2
 * for the whole terrain block twenty instructions earlier, which read as a
 * second, unrelated defect. One statement, 20 bytes, two apparent bugs.
 *
 * RECORDED, NOT ACTED ON: sub_0805A854's result is narrowed here with
 * `lsls r0,#0x18` before the truth test, which is the caller-side evidence for
 * a `u8` return -- the same evidence that retyped its twin AiPickShoalParkBeside in
 * wave 48. src/decomp/c_0805A854.c defines it `int` and a promoted definition
 * wins over any declaration, so this file reproduces the narrowing with an
 * explicit (u8) cast rather than retyping a file it does not own. Its body
 * only ever returns 0 and 1, so the retype should be byte-neutral there; it
 * wants an orchestrator's re-sweep, not a mid-wave unilateral edit.
 *
 * AiFillLanderCostToShoalPlane takes `struct Unk5A514Cell *`, a file-local tag repeated
 * verbatim in c_0805A268.c / c_0805A514.c / c_0805A744.c and therefore
 * unnameable from a shared header (see the wave-52 note in
 * include/unknown-functions.h). An incomplete declaration is enough here.
 */

struct Unk5A514Cell;
void AiFillLanderCostToShoalPlane(struct Unk5A514Cell *);

/* The file-local bitfield view of gUnknown_030040D8 offset 0x09 that
 * include/unknown-globals.h's struct Unk030040D8 unk07[] note (wave 47, W47-G)
 * describes, and that src/decomp/c_0805BF3C.c already uses under the name
 * struct Unk5BFDCCtl. A plain `u8` masking spelling gives
 * `movs r0,#0xc7; ands` where the ROM builds ~0x38 as `movs r0,#0x39;
 * rsbs r0,r0,#0` -- store_bit_field's output, one instruction longer. */
struct Unk5F4F8Ctl
{
    /* 0x00 */ u8 filler_00[0x09];
    /* 0x09 */ u8 unk09_0 : 3;
               u8 unk09_3 : 3;
               u8 unk09_6 : 2;
};

void AiEmbarkOrFallback(void)
{
    struct { u16 x; u16 y; } cur;
    struct Unk03003338 *list;
    u8 *cell;
    int x;
    int y;
    int t;
    u8 best;

    list = gUnknown_03003F20;
    best = 0x7f;

    if (!(gUnknown_030046B8 & 2))
        AiFallbackMove();
    if (gUnknown_085767A0[gUnknown_030040D8->unk00 - 1] == 0)
        AiFallbackMove();

    ((struct Unk5F4F8Ctl *)gUnknown_030040D8)->unk09_3 = 3;
    AiBoardTransport();

    if (!AiListLandersWithFreeBerth((u8 *)list))
        AiFallbackMove();
    AiFillLanderCostToShoalPlane((struct Unk5A514Cell *)list);

    gUnknown_030013EC(gUnknown_030040D8->unk02, gUnknown_030040D8->unk03,
                      gUnknown_030040D8->unk00, 0x78, 0);

    cur.x = 0x270f;

    for (y = 0; y < gMap->height; y++)
    {
        for (x = 0; x < gMap->width; x++)
        {
            cell = gUnknown_03003340[y] + x;
            if (*(s8 *)cell < 0)
                continue;
            t = gMap->terrain[gMap->rowOffset[y] + x] & 0x1f;
            if (t != 0xd && t != 0xb)
                continue;
            if (gMap->unk3C72[gMap->rowOffset[y] + x] == 0x7f)
                continue;
            if (*(s8 *)cell + gMap->unk3C72[gMap->rowOffset[y] + x] >= best)
                continue;
            cur.x = x;
            cur.y = y;
            best = gUnknown_03003340[y][x] + gMap->unk3C72[gMap->rowOffset[y] + x];
        }
    }

    if (cur.x == 0x270f)
        AiFallbackMove();

    if ((s8)gUnknown_03003340[cur.y][cur.x] > GetUnitMovementBudget((struct Unit *)gUnknown_030040D8))
    {
        AiAdvanceToward(&cur);
    }
    else
    {
        GenerateUnitMovementMap(gUnknown_030040D8);
        if (!(u8)sub_0805A854((u16 *)&cur))
            AiFallbackMove();
        AiPublishAction(cur.x, cur.y, 2, 0, 0);
    }
}
asm(".global sub_0805F4F8\n.thumb_set sub_0805F4F8, AiEmbarkOrFallback\n");
