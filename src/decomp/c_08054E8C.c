#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08054E8C.
 * sub_08054E8C @ 0x08054E8C, sub_08054EE0 @ 0x08054EE0, sub_08054F50 @ 0x08054F50
 */

/* `case 1`'s else arm SPELLS OUT case 0's body rather than falling through to
 * it. The two produce the same instruction stream -- cross-jumping merges the
 * duplicate into one block and lays case 1 out first, which is why the ROM
 * looks like a fallthrough -- but they allocate differently: written as a
 * fallthrough, a1 keeps r0 and a4 keeps its incoming r3, where the ROM narrows
 * a1 straight into r4 and a4 into r1 and copies a1 to r0 at each call. Same
 * size either way; only the register assignment discriminates. */
u16 sub_08054E8C(u16 a1, u16 a2, u16 a3, u16 a4)
{
    switch (a2)
    {
    case 0:
        LoadInfantryEffectArt(a1, a4);
        a4 = 2;
        break;
    case 1:
        if (a3 == 1)
        {
            LoadMechEffectArt(a1, a4);
            a4 = 1;
        }
        else
        {
            LoadInfantryEffectArt(a1, a4);
            a4 = 2;
        }
        break;
    case 0x12:
        if (a3 == 1)
        {
            LoadBCopterEffectArt(a1, a4);
            a4 = 3;
        }
        break;
    }
    return a4;
}

/* `off` has to be its own statement, ahead of `v`. Inlined into the CpuFastSet
 * argument it becomes part of the call's argument setup and is evaluated AFTER
 * v and after the source pool word is loaded, which is the wrong order; as a
 * leading initialiser it is evaluated first, as the ROM has it. LoadMechEffectArt
 * next door is the same shape and confirms the ordering. */
void LoadInfantryEffectArt(u16 a1, u16 a2)
{
    u16 v;

    if (a2 != 2)
    {
        u16 off = a1 * 0x2000 + 0xa00;

        v = a1 * 0x100 + 0x50;
        CpuFastSet(gUnknown_08540FBC, (void *)(0x06010000 + off), 0x288);
    }
    else
    {
        v = 0x50;
    }
    gUnknown_020297C0[a1].unk00 = v;
    gUnknown_020296B0[a1].unk00 = v;
    gUnknown_020298E0[a1].unk00 = v;
}
asm(".global sub_08054EE0\n.thumb_set sub_08054EE0, LoadInfantryEffectArt\n");

void LoadMechEffectArt(u16 a1, u16 a2)
{
    u16 v;

    if (a2 != 1)
    {
        u16 r = gUnknown_03004580[a1][0];
        u16 off = a1 * 0x2000 + 0xa00;

        v = a1 * 0x100 + 0x50;
        CpuFastSet(gUnknown_0854E67C, (void *)(0x06010000 + off), 0x348);
        CpuFastSet(gUnknown_08553D80.unk68[r][0], (void *)(0x06010D40 + off), 0x88);
    }
    else
    {
        v = 0x50;
    }
    gUnknown_020297C0[a1].unk00 = v;
    gUnknown_020296B0[a1].unk00 = v;
    gUnknown_020298E0[a1].unk00 = v;
    gUnknown_02029710[a1].unk00 = v + 0x6a;
}
asm(".global sub_08054F50\n.thumb_set sub_08054F50, LoadMechEffectArt\n");
