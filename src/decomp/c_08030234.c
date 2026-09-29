#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08030234.
 * sub_08030234 @ 0x08030234
 */

#include "hardware.h"

/* MATCHED (wave 49, W49-B). Needs its .rodata pool words placed:
 *   "rodata": ["0x08090CB4", "0x08090CB8"]
 * Both are agbcc -fforce-addr address constants (holding &gUnknown_0849B01C
 * and &gUnknown_0849B018), confirmed by dereferencing baserom.gba. Note that
 * gUnknown_0849B018 appears BOTH ways in this one function: through the
 * -fforce-addr word at the top, and as a plain .text pool word at the tail.
 * That is agbcc's own choice and needs no help from the source.
 *
 * gUnknown_03002040 is declared as a 0x58-byte struct, but this function walks
 * it with a 0x14 stride and hands each element to RefreshKeyStFromKeys(struct
 * Unk03002090 *, ...). 0x03002040 + 4 * 0x14 is exactly 0x03002090, so the
 * object is an array of struct Unk03002090 whose element [4] carries its own
 * name. Reached by cast, which is the idiom already used elsewhere in the tree
 * for this address; no new global is declared.
 *
 * Two things cost the attempts, both in loop 2:
 *
 *  1. The array base must be BOUND BEFORE the loop. Written as a cast at the
 *     use, the 0x14 giv comes out as a bare index (`i * 0x14`) and the base is
 *     re-loaded from the pool inside the conditional; bound to `p`, gcc
 *     strength-reduces the whole address and the preheader gets the ROM's
 *     `ldr r7,=gUnknown_03002040` with `adds r7,#0x14` at the bottom.
 *
 *  2. The `goto zero` is load-bearing and is NOT cosmetic. The ROM's three-way
 *     select shares ONE `movs r0,#0` reached from both arms. Written as two
 *     ordinary `= 0` statements, CSE runs first, spots that `nBad` is provably
 *     zero on its arm, and rewrites that arm as `mov r0,rNBAD` -- after which
 *     the two blocks are no longer identical and the later cross-jumping pass
 *     cannot merge them. Giving the zero block two predecessors in the SOURCE
 *     gets in ahead of CSE. The extra reference to nBad also flipped which of
 *     sl/sb it won, so the register swap in the diff was a symptom of this and
 *     not a second problem.
 *
 * unk1b is volatile (see its note in unknown-globals.h); `unk1b = nSplit` and
 * not `= 0` because the ROM stores the register, which is what the source
 * variable compiles to. */

void LinkApplyKeySyncToKeySts(void)
{
    struct Unk03002090 *p;
    s16 buf[4];
    int i;
    int nBad;
    int nSplit;
    int grp;

    nBad = 0;
    nSplit = 0;
    grp = (gUnknown_0849B01C->unk208[gUnknown_0849B018->unk06] & 0x1C00) >> 10;

    for (i = 0; i < 4; i++)
    {
        buf[i] = 0;
        if (SioIsPlayerLinked(i))
        {
            if (gUnknown_0849B01C->unk208[i] == 0xFFFF
             || gUnknown_0849B01C->unk208[i] == 0x5FFF
             || gUnknown_0849B01C->unk208[i] == 0x7FFF
             || !(gUnknown_0849B01C->unk208[i] & 0x8000))
                nBad++;
            if (((gUnknown_0849B01C->unk208[i] & 0x1C00) >> 10) != grp)
                nSplit++;
        }
    }

    p = (struct Unk03002090 *)&gUnknown_03002040;
    for (i = 0; i < 4; i++)
    {
        if (SioIsPlayerLinked(i))
        {
            if (nBad == 0)
            {
                if (nSplit != 0)
                    goto zero;
                buf[i] = gUnknown_0849B01C->unk208[i] & 0x3FF;
            }
            else if (nSplit != 0)
            {
zero:
                buf[i] = 0;
            }
            else
                buf[i] = p[i].unk06;
        }
    }

    if (nSplit != 0)
        gUnknown_0849B018->unk1b++;
    else
        gUnknown_0849B018->unk1b = nSplit;

    for (i = 0; i < 4; i++)
        RefreshKeyStFromKeys(&((struct Unk03002090 *)&gUnknown_03002040)[i], buf[i]);
}
asm(".global sub_08030234\n.thumb_set sub_08030234, LinkApplyKeySyncToKeySts\n");
