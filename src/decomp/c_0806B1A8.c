#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0806B1A8.
 * sub_0806B1A8 @ 0x0806B1A8
 */

#include "proc.h"
#include "hardware.h"
struct UnkB1A8Proc
{
    /* 0x00 */ u8 filler_00[0x58];
    /* 0x58 */ int unk58;
    /* 0x5c */ int unk5c;
};

/* Wave 53, W53-D. MATCHED.
 *
 * PROMOTION NEEDS A .rodata POOL WORD PLACED:
 *   "rodata": ["0x0816E170"]
 * then re-run tools/split_rodata.py and tools/gen_lds.py.
 *
 * Four things in here are not what they look like:
 *
 *  - `gUnknown_0816E170` is NOT a global. The ROM word at 0x0816E170 is
 *    0x08499578, i.e. agbcc's own -fforce-addr address constant for
 *    gBG0TilemapBuffer, which this function reads twice (CpuFastSet and
 *    StartTextBoxViaRecord). Naming the global directly is what reproduces the
 *    `ldr rN,<pool>; ldr rM,[rN]; ldr rK,[rM]` double indirection. Note this
 *    is TWO uses, not four -- the "it needs four uses to hold" line in
 *    docs/agbcc-codegen.md does not apply to a word the ROM already carries.
 *
 *  - `x * 0x421` is wrong for the grey ramp -- see c_08066EBC.c and
 *    c_0806DF20.c. It must be `(x << 10) + (x << 5) + x`, and the second ramp
 *    entry is the proof: its two shifted terms strength-reduce into two
 *    separate givs (step 0x400 and step 0x20) with `0x10 + i` left as a plain
 *    add, where a MULT_EXPR would give ONE giv stepping 0x421.
 *
 *  - The 0x2280 bias must NOT be bound to a local. Reading
 *    gUnknown_08581984[i & 0x1f] TWICE in the source is what stops loop.c
 *    hoisting the `movs #0x8a; lsls #6` constant into the preheader; with a
 *    `t` local holding the element, the constant becomes a movable, is
 *    hoisted, and costs a register plus two bytes. Wave-49 W49-A ("NOT binding
 *    a VALUE to a local is a lever") -- and it was NOT register pressure, r7
 *    is free in the ROM throughout this loop.
 *
 *  - gUnknown_030030E0's two BLEND target groups are written through `.raw`
 *    with group masks, not field by field. `.bits.effect = 1` is a BYTE
 *    read-modify-write and a QImode store cannot be forwarded into the HImode
 *    load that follows, which is exactly why the ROM commits the effect byte
 *    and then re-`ldrh`s. Ten individual `.bits.targetN_enable_*` writes merge
 *    the effect RMW into the byte-0 group instead and lose the `ldrh`/`strh`
 *    pair -- the group masks 0xFFE0 and 0xE0FF are the tell, since five 1-bit
 *    field writes give five separate `mov #N; neg; and` steps.
 */
void CreditsEpilogue_Init(struct UnkB1A8Proc *proc)
{
    int i;
    int x;
    int zero;
    u16 v;

    ResetWindowShadows();
    SetDefaultColorEffects();
    SetupBackgrounds(gUnknown_0849D16C);

    gDispIo.disp_ct.obj_mapping = TRUE;
    SetDispEnable(1, 1, 1, 1, 0);

    gUnknown_03002B6C.bits.priority = 0;
    gUnknown_030030B4.bits.priority = 1;
    gUnknown_0300251C.bits.priority = 2;
    gUnknown_03001FE8.bits.priority = 3;

    SetBgScrollShadow(0, 0, 0);

    zero = 0;
    CpuFastSet(&zero, gBG0TilemapBuffer, 0x01000200);

    for (i = 1; i < 0x10; i++)
    {
        x = i > 7 ? i : i >> 1;
        gPal[0x10 + i] = (x << 10) + (x << 5) + x;
        gPal[0x20 + i] = ((0x10 + i) << 10) + ((0x10 + i) << 5) + (0x10 + i);
    }

    Decompress(gUnknown_081A3DA4, (void *)0x06005000);

    for (i = 0; i < 0x280; i++)
    {
        v = gUnknown_08581984[i & 0x1f] + 0x2280;
        if (gUnknown_08581984[i & 0x1f] > 7)
            v -= 0x1000;
        gBG1TilemapBuffer[i] = v;
    }

    ApplyPaletteExt((u16 *)gUnknown_0823BDE0, 0, 0x20);
    ApplyPaletteExt(gUnknown_08194280, 0x80, 0x20);
    ApplyPaletteExt(gUnknown_081320AC, 0x60, 0x20);
    Decompress(gUnknown_0823A3D4, (void *)0x06008000);
    Decompress(gUnknown_081933F4, (void *)0x0600CC00);
    Decompress(gUnknown_081942A0, gBG3TilemapBuffer);
    Decompress(gUnknown_081942A0, gBG2TilemapBuffer);
    StartTextBoxViaRecord(4, 0x14, gBG0TilemapBuffer, 0x873, 0x3000, 0x40);

    BG_EnableSyncBG0();
    BG_EnableSyncBG1();
    BG_EnableSyncBG2();
    BG_EnableSyncBG3();

    proc->unk58 = 0;
    proc->unk5c = 0;

    gUnknown_030030E0.bits.effect = 1;
    gUnknown_03002020 = 8;
    gUnknown_03002B28 = 8;
    gUnknown_03001FFC = 0;
    gUnknown_030030E0.raw = (gUnknown_030030E0.raw & 0xffe0) | 4;
    gUnknown_030030E0.raw = (gUnknown_030030E0.raw & 0xe0ff) | 0x800;
}
asm(".global sub_0806B1A8\n.thumb_set sub_0806B1A8, CreditsEpilogue_Init\n");
