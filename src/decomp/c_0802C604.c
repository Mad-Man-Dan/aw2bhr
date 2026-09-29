#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802C604.
 * sub_0802C604 @ 0x0802C604
 */

/* PARKED ON THE PIPELINE, NOT ON THE DECOMPILATION (wave 24, W24-A).
 *
 * trymatch reports `+2` and simultaneously `bytes: 0 of 38 differ (100.0%
 * identical)` with `relocs: match`. The whole of the residual is a trailing
 * `.short 0x0000`: OptionsMenu_DeleteUsability is 38 bytes, THUMB code ending 2 mod 4 leaves
 * `.text` short of its four-byte section alignment, and trymatch reads the
 * section with `objcopy --only-section=.text`, so the pad is inside the window
 * it compares. Nothing writable in C moves it.
 *
 * The ROM spends those two bytes the same way. baserom.gba at 0x0802C62A reads
 * `00 00`, and data/asm-resident.json already records sub_0802C62A as "the
 * 0x0000 the assembler emits for `.align 2, 0` ahead of the 4-aligned
 * IsLinkGame". So this file reproduces 0x0802C604..0x0802C62B -- all forty
 * bytes, byte for byte. Promoting it must drop that 2-byte stub from asm/ as
 * well, which promote.py has never had to do: no matched function in the tree
 * has size % 4 == 2. See the wave-24 chapter in docs/agbcc-codegen.md.
 *
 * DO NOT go looking for another spelling. The derivation below is settled and
 * any edit that changes the size by two is wrong by two the other way.
 *
 * The inner range test is a SWITCH, not an `&&` chain, and that is the whole of
 * this function's difficulty.
 *
 * `if (v >= 1 && v <= 3)` is folded by fold_range_test into the unsigned trick
 * `subs r0,#1; cmp r0,#2; bhi`, which the ROM does not have. Splitting it into
 * nested `if`s defeats the fold but not the constant canonicalisation: agbcc
 * rewrites both `v >= 1` and `v < 1` as compares against 0, giving
 * `cmp r0,#0; ble`. The ROM's `cmp r0,#1; blt` compares against 1, which no
 * folded C comparison operator survives to produce -- but a switch does, because
 * expand_case emits the case node's own low and high bounds as RTL without going
 * through fold. Three contiguous case labels sharing one body become exactly
 * `cmp #3; bgt default` then `cmp #1; blt default`.
 *
 * GetCampaignMissionId returns `int`: the compares are signed and there is no
 * re-narrowing in front of them, which agbcc emits at every call site of a
 * narrow-returning callee.
 */

bool8 OptionsMenu_DeleteUsability(void)
{
    if (gPlaySt.gameMode == 1)
    {
        switch (GetCampaignMissionId())
        {
        case 1:
        case 2:
        case 3:
            return TRUE;
        }
    }

    return FALSE;
}
asm(".global sub_0802C604\n.thumb_set sub_0802C604, OptionsMenu_DeleteUsability\n");
