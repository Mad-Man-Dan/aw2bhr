#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080484CC.
 * sub_080484CC @ 0x080484CC
 */

#include "hardware.h"

/* MATCHED byte-for-byte (wave 48, W48-I).
 *
 * PROMOTION NEEDS A .rodata POOL WORD PLACED:
 *   "rodata": ["0x0812A140"]   (then split_rodata.py + gen_lds.py)
 * 0x0812A140 is an agbcc -fforce-addr address constant holding 0x03001FBC,
 * i.e. &gUnknown_03001FBC -- one more member of the 0x0812Axxx run
 * unknown-globals.h documents, and NOT an object. The honest spelling
 * `ClearSlotScriptCallback(gUnknown_03001FBC)` is what produces it, and it appears in the
 * FIRST arm only: there the address must survive a call inside the loop and so
 * lands in a callee-saved register. The second arm's identical call gets a
 * plain inline pool word.
 *
 * The cursor is s16 and the scan DESCENDS: `ldrb p->unk1f; lsls #0x10;
 * asrs #0x10` with a `blt` exit, and the decrement re-narrows to u16 before the
 * next sign-extending read. gUnknown_03001FBC is declared s16 and ClearSlotScriptCallback
 * takes u8, so the narrowing folds into the `ldrb` -- there is no cast in the
 * source. `sel` is deliberately read after a loop that may not have run; the
 * ROM does the same (r5 is undefined on the zero-trip path).
 */

void UnitList_Loop(struct Unk0804769C *p)
{
    s16 i;
    u16 sel;

    if (gpKeySt->pressed & 1)
    {
        for (i = p->unk1f; i >= 0; i--)
        {
            sel = gUnknown_02028DD8[i];
            if (!sub_0804769C(p, sel))
                break;
        }
        p->unk28 = sel;
        PlayMusicOrSfx2(0x65);
        ClearSlotScriptCallback(gUnknown_03001FBC);
    }
    else if (gpKeySt->pressed & 2)
    {
        PlayMusicOrSfx2(0x66);
        ClearSlotScriptCallback(gUnknown_03001FBC);
        gUnknown_03002F1C = 1;
    }
    else
    {
        UnitList_DrawFrame(p);
    }
}
asm(".global sub_080484CC\n.thumb_set sub_080484CC, UnitList_Loop\n");
