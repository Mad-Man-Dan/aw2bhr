#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802A3FC.
 * sub_0802A3FC @ 0x0802A3FC
 */

/* The gate is `s8 FindSlotScript(s32)`, so agbcc re-narrows the result
 * (`lsls #0x18; asrs #0x18`) before comparing it against -1.
 *
 * gUnknown_03001FBC is read three times across control-flow merges and so
 * picks up agbcc's -fforce-addr `.rodata` word, which the ROM prints as
 * `gUnknown_08090B80`; gUnknown_03001470 is referenced twice and gets a plain
 * inline pool word. Both come from the one honest spelling -- do not declare
 * a gUnknown_08090B80 object.
 *
 * The two hits are SEPARATE `if` statements, not one `||`. Their bodies are
 * identical source, and gcc cross-jumped the common tail from the
 * `lsls r1,#1` onwards -- but not the `ldrsh` above it, because the two
 * copies picked different scratch registers for the mandatory zero index
 * (`movs r4,#0` vs `movs r2,#0`) and cross-jumping stops at the first
 * instruction that differs. An `||` has one body and would emit that `ldrsh`
 * once.
 *
 * The exit test after the loop is `i == 0x33`, which is how the two `break`s
 * are told apart from running the scan dry -- a `break` leaves i <= 0x32.
 * struct Unk03001470's unk38 is read here into a u16 cursor, which is the
 * plain `ldrh` whatever the member's signedness. */

/* sub_0802A38C cannot go in the shared header: it is promoted in
 * src/decomp/c_0802A38C.c with a file-local `struct Unk2A38C *` parameter, so
 * a header declaration would be a conflicting type for that unit. This is the
 * weakest spelling that agrees with both call sites, and it matches how
 * sub_0802A2E4 and CalcCargoResupplyCost are declared (`void *`).
 *
 * sub_0802A258 is SIXTEEN bits, measured here and nowhere else: the truth
 * test is `lsls r0, #0x10`, where a bool8/u8 return gives `lsls #0x18`. The
 * work/ draft declares it `bool8`; that is refuted. It is declared locally
 * for the same file-local-tag reason. */
s16 sub_0802A258(struct Unit *);
bool8 sub_0802A38C(void *, int (*)(void *));

void TurnStartSupply_Loop(void)
{
    struct Unit *p;
    u16 i;

    if (FindSlotScript((s32)gUnknown_0849A0A8) != -1)
        return;

    for (i = gUnknown_03001470[gUnknown_03001FBC].unk38; i <= 0x32; i++)
    {
        p = &gUnits[gUnknown_03003F2C + i];

        if (p->type == 0)
            continue;

        if (p->flags & 9)
            continue;

        if (p->type == 0x16 && sub_0802A38C(p, CalcCargoResupplyCost))
        {
            gUnknown_03001470[gUnknown_03001FBC].unk38 = i + 1;
            break;
        }

        if (sub_0802A258(p) && sub_0802A38C(p, sub_0802A2E4))
        {
            gUnknown_03001470[gUnknown_03001FBC].unk38 = i + 1;
            break;
        }
    }

    if (i == 0x33)
        ClearSlotScriptCallback(gUnknown_03001FBC);
}
asm(".global sub_0802A3FC\n.thumb_set sub_0802A3FC, TurnStartSupply_Loop\n");
