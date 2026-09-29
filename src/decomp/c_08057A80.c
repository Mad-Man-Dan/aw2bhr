#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08057A80.
 * sub_08057A80 @ 0x08057A80, sub_08057AE8 @ 0x08057AE8
 */

/* gUnknown_085D6A48's 24-byte rows are STRUCTS, not `u16[12]`: the column-1
 * offset stays in the `ldrh` displacement instead of being reassociated onto
 * the base.  See the "column-offset fold" chapter of docs/agbcc-codegen.md.
 * The global keeps its `u16 [][12]` declaration (wave 17 recorded why), so the
 * row shape is spelled locally and cast at the use. */
struct Unk085D6A48Row
{
    /* 0x00 */ u16 unk00;
    /* 0x02 */ u16 unk02;
    /* 0x04 */ u8 filler_04[0x14];
};

void sub_08057A80(u16 *dst)
{
    struct Unk085D6A48Row *rows;
    int pos;
    int i;
    int idx;

    LoadCoMiniPortraitOpaque(gUnknown_03004580[0][4], (u16 *)0x060059C0, 0x050000E0);
    LoadCoMiniPortraitOpaque(gUnknown_03004580[1][4], (u16 *)0x06005DC0, 0x05000100);

    for (i = 0; i <= 1; i++)
    {
        rows = (struct Unk085D6A48Row *)gUnknown_085D6A48;
        idx = rows[gUnknown_03004580[i][1]].unk02 * 2 + i;
        pos = i << 19;
        sub_08057A24(dst, idx, (struct Unk8057Pos *)&pos);
    }
}

/* MATCH (wave 48, W48-G).  NEEDS ITS .rodata POOL WORDS PLACED -- the promotion
 * entry must carry
 *   "rodata": ["0x081361A0", "0x081361A4", "0x081361A8", "0x081361AC"]
 * then tools/split_rodata.py + tools/gen_lds.py before the build.  Those four
 * ROM words are NOT objects: they hold 0x030005D8, 0x02029B78, 0x02029B7C and
 * 0x030005E0, i.e. they are agbcc's own -fforce-addr address constants for the
 * four arrays this loop walks.  Naming the real symbols is what reproduces the
 * double indirection (`ldr rX, =pool; ldr rY, [rX]`).  Same trap as
 * gUnknown_081D943C / gBG0TilemapBuffer; see include/unknown-globals.h.
 *
 * Two evaluation-order facts carry the loop body, and both are visible only as
 * instruction ORDER, never as a wrong instruction:
 *
 * 1. The numerator of the second divide is computed BEFORE the first divide and
 *    parked in a callee-saved register.  That is not scheduling -- it is
 *    `expand_expr` on `A / B` expanding A before B.  So the whole thing has to
 *    be ONE expression: split into `n = (s16)d << 16;` as its own statement and
 *    the store's destination address moves after the call instead of before it.
 *    Equally, `d` must be an `int`: with `s16 d` the narrowing binds to d's
 *    assignment and lands ahead of `ldr r1, [r7]`, where the ROM has it after.
 *    The ROM's narrowings are all casts inside the divide expression.
 *
 * 2. `q` is load-bearing.  The ROM computes gUnknown_02029B7C[i]'s ADDRESS
 *    before it loads gUnknown_02029B78[i]'s VALUE, which a plain
 *    `gUnknown_02029B78[i] - gUnknown_02029B7C[i]` cannot do -- binop expands
 *    operand 0 first, so the B load always precedes the C address.  An address
 *    evaluated before the other operand's load means that address was its own
 *    statement.
 */

void InitBattleHud(void)
{
    int i;
    int d;
    u16 *q;

    Decompress(gUnknown_0816CABC, (void *)0x06004000);
    CpuFastSet(gUnknown_0816D498 + gUnknown_03004500[0] * 32, (void *)0x05000140, 8);
    CpuFastSet(gUnknown_0816D498 + gUnknown_03004500[1] * 32, (void *)0x05000120, 8);
    ClearBg0Tilemap();

    for (i = 0; i <= 1; i++)
    {
        gUnknown_030005E8[i] = 0;
        gUnknown_030005D8[i] = gUnknown_02029B78[i] << 16;
        q = gUnknown_02029B7C + i;
        d = gUnknown_02029B78[i] - *q;
        gUnknown_030005E0[i] = ((s16)d << 16) / ((s16)((s16)d / 5) + 20);
    }

    DrawBothSideHud(gBG0TilemapBuffer);
    sub_08057A80(gBG0TilemapBuffer);
}
asm(".global sub_08057AE8\n.thumb_set sub_08057AE8, InitBattleHud\n");
