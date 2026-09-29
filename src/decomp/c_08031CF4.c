#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08031CF4.
 * sub_08031CF4 @ 0x08031CF4, sub_08031D54 @ 0x08031D54, sub_08031DC0 @ 0x08031DC0
 */

#include "proc.h"
struct Unk08031CF4Proc
{
    /* 0x00 */ u8 filler_00[0x1e];
    /* 0x1e */ s16 unk1e;
    /* 0x20 */ s16 unk20;
};
struct Unk08031D54Proc
{
    /* 0x00 */ u8 filler_00[0x1e];
    /* 0x1e */ s16 unk1e;
    /* 0x20 */ s16 unk20;
    /* 0x22 */ s16 unk22;
};

void sub_08031CF4(struct Unk08031CF4Proc *proc)
{
    FillTilemapRect(gBG0TilemapBuffer, 0xc, 8, 2, 2, 0);
    BG_EnableSyncBG0();

    PutOamHi(proc->unk1e, proc->unk20,
                 gUnknown_0849B074[((u32)gGameClock >> 3) % 3], 0x4054);
}

void sub_08031D54(struct Unk08031D54Proc *proc)
{
    proc->unk22 = gUnknown_0849B060->unk0a + 0x64;

    if (proc->unk22 != proc->unk1e && (gGameClock & 1) == 0)
        proc->unk1e++;

    PutOamHi(proc->unk1e, proc->unk20,
                 gUnknown_0849B074[((u32)gGameClock >> 3) % 3], 0x4054);
}

/* Sets up the unit records the 0x08031 screen edits: publishes
 * &gUnits[1] as gUnknown_030040D8, redraws the 0xc-wide window, and
 * writes two fields on element 1 and two on element 2.
 *
 * THE +0x18 / +0x1C STORES ARE ELEMENT 2, NOT MEMBERS 0x18 AND 0x1C. struct
 * Unit is 0x0c bytes, so 0x18 == 2 * 0x0c is that element's `type` and
 * 0x1c its unk04_0 -- the `u16 unk04_0:7` bitfield already declared there,
 * which is exactly what the `AND ~0x7f; OR 0x64` store is. agbcc synthesises
 * 0xFFFFFF80 as `subs r0,#0x81` off the `movs r0,#1` the previous statement
 * left live, which is why the mask does not look like one. Both displacements
 * fit `strb`'s imm5, so no address arithmetic appears and the element indices
 * are invisible unless you know the 0x0c stride.
 *
 * THE ARRAY BASE IS BOUND TO A LOCAL. Naming gUnits at each of its
 * three uses reloads the pointer twice more and puts a fourth word in the
 * literal pool; the ROM materialises it once into r4 and keeps it. By contrast
 * gUnknown_030040D8 IS reloaded for each of its two stores, so that one is
 * named directly -- the two spellings sit side by side in this function. */
void sub_08031DC0(void)
{
    struct Unit *p;

    p = gUnits;

    gUnknown_030040D8 = (struct Unk030040D8 *)&p[1];

    FillTilemapRect(gBG0TilemapBuffer, 0xc, 8, 2, 2, 0);

    p[2].type = 1;
    p[2].hp = 0x64;

    gUnknown_030040D8->unk00 = 0x17;
    gUnknown_030040D8->unk07[0] = 2;

    PutTextScriptImmediate(0x18, 8, gBG0TilemapBuffer, gUnknown_08090D18, 0x8000, 0);
    BG_EnableSyncBG0();
}
