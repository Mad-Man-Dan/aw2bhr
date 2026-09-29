#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08087104.
 * sub_08087104 @ 0x08087104, sub_08087168 @ 0x08087168
 */

/* Reads the candidate-list byte at gUnknown_02027F74 + 4 + proc->unk58 and, when
 * IsCampaignMapUnlocked accepts it, copies the four bytes at gUnknown_02027F74 +0..+3 into
 * the proc's four halfwords at +0x64; otherwise fills all four with -1.
 *
 * THE ADDRESS ARITHMETIC IS src/decomp/c_08086E54.c's IDIOM, not a member array.
 * `(u8 *)&gUnknown_02027F74` bumped by 4 in a statement of its OWN is what puts
 * the base in r5 and the bumped pointer in r0 (`adds r0, r5, #4`), leaving r5
 * live for the four `ldrb`s off +0..+3. The member-array spelling
 * `gUnknown_02027F74.unk04[p->unk58]` keeps the pool word equally clean but
 * assigns the two scratch registers the other way round (`adds r1, r5, #4` /
 * `ldr r0, [r4, #0x58]` / `adds r0, r0, r1`), which is the 4-byte residual wave
 * 34 parked this function on at 96.0%. Both spellings were put through one
 * compile_probe side by side in wave 46; the local reproduces the ROM and the
 * member array reproduces the park. The `q` pseudo is created by its own
 * statement and so takes the lower allocno, which is what wins it r0.
 *
 * This CORRECTS the note on struct Unk02027F74.unk04 in include/unknown-globals.h,
 * which recorded that UpdateMapSelectPropertyCounts reaches the list "through this struct rather
 * than through the overlapping gUnknown_02027F78 extern". The clean pool word is
 * not evidence for the member array -- both spellings produce it -- and the
 * member array is the one that does NOT match.
 *
 * The `lsls #0x18` after the `bl` is NOT a narrow return type: src/decomp/c_0803CA54.c
 * is promoted and defines `int IsCampaignMapUnlocked(u32)`, so the byte test belongs to
 * this caller and is spelled `(u8)IsCampaignMapUnlocked(...)`. try_match compiles one unit
 * and cannot see that; proto_check.py is what catches it.
 *
 * The two spellings of -1 in the else arm are NOT interchangeable: unk64 gets
 * `0xFFFF` (`ldr r0,=0xffff`) and the other three get `-1` (`movs r0,#1; rsbs`).
 * Same bits, different codegen, and the ROM has one of each. */

struct Unk87104
{
    /* 0x00 */ u8 filler_00[0x58];
    /* 0x58 */ int unk58;
    /* 0x5c */ u8 filler_5c[0x08];
    /* 0x64 */ u16 unk64;
    /* 0x66 */ u16 unk66;
    /* 0x68 */ u16 unk68;
    /* 0x6a */ u16 unk6a;
};

void UpdateMapSelectPropertyCounts(void *arg)
{
    struct Unk87104 *p = arg;
    u8 *q;

    q = (u8 *)&gUnknown_02027F74;
    q += 4;

    if (q[p->unk58] <= 0xb3 && (u8)IsCampaignMapUnlocked(q[p->unk58]))
    {
        p->unk64 = gUnknown_02027F74.unk00;
        p->unk66 = gUnknown_02027F74.unk01;
        p->unk68 = gUnknown_02027F74.unk02;
        p->unk6a = gUnknown_02027F74.unk03;
    }
    else
    {
        p->unk64 = 0xFFFF;
        p->unk66 = -1;
        p->unk68 = -1;
        p->unk6a = -1;
    }
}
asm(".global sub_08087104\n.thumb_set sub_08087104, UpdateMapSelectPropertyCounts\n");

/* Draws up to four HUD elements, in two layouts chosen by gPlaySt.gameMode.
 *
 * THE CONDITION IS TWO STATEMENTS, NOT ONE EXPRESSION, and that is what wave 34
 * parked this function on at 92.4%. The ROM's window is
 *
 *   ldrb / subs r0,r0,r5 / ldr r1,=g03005928 / ldr r1,[r1] / adds r0,#1 / cmp r1,r0
 *
 * i.e. the global's two loads sit BETWEEN the subtract and the `+ 1`, and the
 * global is the FIRST operand of the compare. No single-expression spelling can
 * produce that: agbcc expands a comparison's op0 completely before op1, so
 * `expr != g` puts both loads after the `+ 1` (and compares `r0, r1`), while
 * `g != expr` puts them both before the `ldrb` and inverts the pool word order
 * to (03003FC0, 03005928, 02027F74) where the ROM has (03003FC0, 02027F74,
 * 03005928). Both were measured this wave.
 *
 * Splitting `unk37 - a` into its own statement makes it a PRECEDING statement
 * rather than part of op1, so the comparison then legitimately evaluates op0
 * (the global) first -- which lands its loads exactly where the ROM has them
 * and gives `cmp r1, r0`. Binding the global too is what forces its `ldr; ldr`
 * pair to be emitted as a unit: left as a bare MEM, expand_expr returns the MEM
 * unloaded, the address load floats to the top of the comparison and the value
 * load is deferred to the `cmp`, leaving the `+ 1` wedged between them.
 *
 * `n = (g = gUnknown_02027F74.unk37 - a);` in the else arm is NOT decoration and
 * NOT a typo -- it is a dead store to `g` that decomp-permuter found, and it is
 * load-bearing. It shortens the allocation so `n` reuses r0 from the `ldrb`
 * (`subs r0, r0, r5`, then the 2-operand `adds r0, #1`); written as the plain
 * `n = gUnknown_02027F74.unk37 - a;` that the m == 2 arm uses, `n` gets its own
 * pseudo in r2 and the whole three-instruction window shifts registers -- the
 * SAME instructions in the SAME order, 12 bytes different, 93.5%. Whatever the
 * original source really wrote there, do not "clean this up": the two arms
 * genuinely compile differently and only this pair matches.
 *
 * Also settled and unchanged from wave 34's reading:
 *   - `m` must be `int`, not `u8`. A `u8` local costs an extra `adds r5,r0,#0`
 *     copy; the ROM loads straight into r4 with `ldrb r4,[r0,#1]`.
 *   - The comparison against gUnknown_03005928 is SIGNED -- the second arm's
 *     `cmp r2,r0; ble` is a signed branch. Binding it to the `int` local `g` is
 *     what makes it signed here, so the explicit `(int)` casts the parked draft
 *     carried are no longer needed. Wave 46 probed declaring the GLOBAL signed
 *     instead and it is byte-neutral in this function, so that is NOT evidence
 *     for retyping gUnknown_03005928; it is left `u32` as declared.
 *   - The first arm passes `m` as the 5th argument (the ROM reuses r4) while
 *     the second passes the literal 2. Not the same spelling, same value. */

void MapSelectList_DrawScrollArrows(int a)
{
    int m;
    int n;
    int g;

    m = gPlaySt.gameMode;

    if (m == 2)
    {
        if (a != 0)
            DrawOamObject(0x43, 0x24, 0x20, 0, m);

        n = gUnknown_02027F74.unk37 - a;
        g = gUnknown_03005928;
        if (g != n + 1)
            DrawOamObject(0x44, 0x24, 0x98, 0, m);
    }
    else
    {
        if (a != 0)
            DrawOamObject(0x43, 0x20, 0x40, 0, 2);

        n = (g = gUnknown_02027F74.unk37 - a);
        g = gUnknown_03005928;
        if (g != n + 1 && gUnknown_02027F74.unk37 > g)
            DrawOamObject(0x44, 0x20, 0x98, 0, 2);

        DrawOamObject(0x45, 0, 0x28, 0, 2);
        DrawOamObject(0x46, 0x48, 0x28, 0, 2);
    }
}
asm(".global sub_08087168\n.thumb_set sub_08087168, MapSelectList_DrawScrollArrows\n");
