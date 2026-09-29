#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08047C04.
 * sub_08047C04 @ 0x08047C04
 */

/*
 * MATCHES. Wave 53 (W53-A), 832 bytes. Four try_match attempts plus two
 * permuter runs; the permuter supplied the last 8 bytes and nothing else could.
 *
 * NO .rodata to place. try_match reports two relocations naming different
 * symbols that resolve to the same address (+0x240 and +0x25c): that is the
 * finding below, not a difference in the ROM.
 *
 * gUnknown_0200FC50, gUnknown_0200FE50 and gUnknown_02010450 ARE ONE OBJECT.
 * Proof: `ldr r2, =0xFFFFF800; adds r4, r4, r2` reaches gUnknown_0200FC50 by
 * subtracting 0x800 from the gUnknown_02010450 already in r4. agbcc can only
 * fold the difference of two address constants when they are the SAME symbol,
 * so 0200FE50 is gUnknown_0200FC50 + 0x200 and 02010450 is
 * gUnknown_0200FC50 + 0x800. Spelling all three as one symbol also removes a
 * `-fforce-addr` .rodata word that three separate symbols force and the ROM
 * does not have, and it makes every offset fall on a 0x400 front/back pairing:
 * 0x100/0x500, 0x200/0x600, 0x240/0x640, 0x2c0/0x6c0, 0x800/0xc00, 0x880/0xc80.
 * gUnknown_02010450 stays declared in unknown-globals.h because other matched
 * functions use it; this one does not need it.
 *
 * Three spellings, each of which was worth bytes:
 *
 *  - The `?:` for DrawTallNumberRightAligned's fourth argument must be an IF/ELSE STATEMENT.
 *    `a->unk21 == 0 ? 0 : a->unk1f + 1` is folded by the front end to
 *    `a->unk21 ? a->unk1f + 1 : 0`, which inverts the branch (`beq` to an
 *    out-of-line zero); the ROM has `bne` to an out-of-line `unk1f + 1`. The
 *    statement form is not folded, and it also moves argument 3's
 *    `ldr r2, [r6]` to AFTER the merge, which is where the ROM has it.
 *
 *  - The zero arm is `v = a->unk21`, not `v = 0`. The ROM copies the just-tested
 *    byte (`adds r3, r0, #0`) rather than materialising the constant, and CSE
 *    will not turn a `const_int 0` into that copy on its own -- the source has
 *    to name the value.
 *
 *  - `v` must be `s16`, not `int`. That one token swapped r5 and r6 across the
 *    whole tail (the gBG0TilemapBuffer pointer and the 0x8000 constant) and was
 *    worth 9 of the last 17 bytes. Found by the permuter.
 *
 * And the last 8: `base = 0x100;` INSIDE loop 1, binding the first source
 * offset to a local before the call rather than writing the constant inline.
 * Loop 1 and loop 2 are the same shape and the same source text, yet the ROM
 * allocates them differently -- loop 1 has the +0x500 giv in r7 and the
 * 0x060139C0 pool word in r5, loop 2 has them the other way round. Any draft
 * that writes the two loops identically gets loop 2's allocation for both. The
 * extra pseudo created at the top of loop 1's body is what flips the
 * strength-reduced givs' tie-break back. Same family as the wave-17 comma
 * anchor, but the anchor here is a CONSTANT bound to a local, and it is the
 * only thing in ~23,000 permuter iterations that moved those 8 bytes.
 */

struct Unk08047C04
{
    /* 0x00 */ u8 filler_00[0x1e];
    /* 0x1e */ u8 unk1e;
    /* 0x1f */ u8 unk1f;
    /* 0x20 */ u8 unk20;
    /* 0x21 */ u8 unk21;
    /* 0x22 */ u8 unk22;
    /* 0x23 */ u8 filler_23[0x28 - 0x23];
    /* 0x28 */ u16 unk28;
};

void UnitList_Init(struct Unk08047C04 * a)
{
    int i;
    s16 v;
    int base;

    a->unk28 = 0;
    a->unk22 = 0;
    a->unk1e = 0;
    a->unk1f = 0;
    a->unk20 = 0;

    Decompress(gUnknown_0823E140, gUnknown_0200FC50);

    for (i = 0; i < 2; i++)
    {
        base = 0x100;
        CpuFastSet(gUnknown_0200FC50 + base + i * 0x80, (void *)(0x060139C0 + i * 0x100), 0x20);
        CpuFastSet(gUnknown_0200FC50 + 0x500 + i * 0x80, (void *)(0x06013A40 + i * 0x100), 0x20);
    }

    CpuFastSet(gUnknown_0200FC50 + 0x200, (void *)0x06013BC0, 0x10);
    CpuFastSet(gUnknown_0200FC50 + 0x600, (void *)0x06013C00, 0x10);
    CpuFastSet(gUnknown_0200FC50 + 0x240, (void *)0x06013C40, 0x20);
    CpuFastSet(gUnknown_0200FC50 + 0x640, (void *)0x06013CC0, 0x20);

    for (i = 0; i < 2; i++)
    {
        CpuFastSet(gUnknown_0200FC50 + 0x2C0 + i * 0x80, (void *)(0x06013D40 + i * 0x100), 0x20);
        CpuFastSet(gUnknown_0200FC50 + 0x6C0 + i * 0x80, (void *)(0x06013DC0 + i * 0x100), 0x20);
    }

    CpuFastSet(gUnknown_0200FC50 + 0x800, (void *)0x06013F40, 0x20);
    CpuFastSet(gUnknown_0200FC50 + 0xC00, (void *)0x06013FC0, 0x20);
    CpuFastSet(gUnknown_0200FC50 + 0x880, (void *)0x06014040, 8);
    CpuFastSet(gUnknown_0200FC50 + 0xC80, (void *)0x06014060, 8);
    ApplyPaletteExt(gUnknown_0823E550, 0x2A0, 0x20);
    Decompress(gUnknown_0823FFA8, gUnknown_0200FC50);
    CpuFastSet(gUnknown_0200FC50, (void *)0x06014100, 0x10);
    sub_0801F114();
    InitTilePool(0, (void *)0x06010000, 0x20A, 0x16);
    LoadTilePoolGraphic(1);
    InitTilePool(1, (void *)0x06010000, 0x204, 0x14);
    LoadTilePoolGraphic(gPlayers[gUnknown_030033EC].teamColor + 0x3D);
    CpuCopyAuto(gUnknown_0812A2AC, (void *)0x06013940, 0x80);
    InitTextTileCache(0);
    ClearBg0Tilemap();
    ClearBg2Tilemap();
    TmApplyTsaClipped(gBG2TilemapBuffer, 1, 0, gUnknown_0812A8C8, 0x8360);
    sub_08047190(a, a->unk1e);
    PutTextScriptImmediate(4, 1, gBG0TilemapBuffer,
        gTextTable[gUnknown_0849F658[gPlayers[gUnknown_030033EC].teamColor]],
        0x8000, 0);
    PutTextTableEntryImmediate(8, 5, gBG0TilemapBuffer, 0x96F, 0x8000, 0);
    PutTextTableEntryImmediate(0xF, 5, gBG0TilemapBuffer, 0x970, 0x8000, 0);
    PutTextTableEntryImmediate(0x12, 5, gBG0TilemapBuffer, 0x971, 0x8000, 0);
    PutTextTableEntryImmediate(0x18, 5, gBG0TilemapBuffer, 0x972, 0x8000, 0);

    if (a->unk21 == 0)
        v = a->unk21;
    else
        v = a->unk1f + 1;

    DrawTallNumberRightAligned(0x18, 1, gBG0TilemapBuffer, v, 0x8000, 0);
    PutTextScriptImmediate(0x19, 1, gBG0TilemapBuffer, gUnknown_084C3F38, 0x8000, 0);
    DrawTallNumberRightAligned(0x1B, 1, gBG0TilemapBuffer, a->unk21, 0x8000, 0);
    DrawUnitListRows(a);
    BG_EnableSyncBG0();
    BG_EnableSyncBG2();
    LoadCursorSpriteGraphics();
    SetMapCursorDisplayPosition(0x1E, (a->unk1f - a->unk20) * 16 + 0x36);
}
asm(".global sub_08047C04\n.thumb_set sub_08047C04, UnitList_Init\n");
