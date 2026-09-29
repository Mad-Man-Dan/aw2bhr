#include "global.h"
#include "proc.h"
#include "hardware.h"

struct Unk6F41CProc
{
    /* 0x00 */ u8 filler_00[0x38];
    /* 0x38 */ s8 unk38;
};

/*
 * sub_0806F41C -- rebuild the screen for one entry of the screen table.
 *
 * The proc's byte at 0x38 picks a 16-byte entry of gUnknown_0816E808. The
 * windows are switched off and the procs belonging to the previous screen are
 * ended, then one of two things happens.
 *
 * If the entry has no graphics of its own (unk0e is zero), or if
 * sub_0803CBD8(0x22) says the caller is in the state that suppresses them, the
 * entry's graphics are loaded: either through sub_0806AEC4 when unk00 is a
 * small number rather than an address, or by decompressing unk00 into VRAM and
 * unk04 into gUnknown_08499578 and applying the palette at unk08, whose slot
 * count is unk0c.
 *
 * Otherwise the gUnknown_08582CAC proc is restarted over a cleared VRAM and the
 * caller jumps to its label 1.
 *
 * Bit 0x80 of gUnknown_03002B6C is set or cleared from the entry's unk0d.
 *
 * Why the C looks odd: these spellings do not change what the code does, but
 * the original compiler only produces the same instructions with them.
 *   - `flag` is set to zero and then reassigned from the comparison against
 *     itself. Written as one statement, the compiler keeps a different value
 *     live and needs an extra saved register.
 *   - The last argument's subscript goes through `new_var` rather than reading
 *     `proc->unk38` a third time, which is what lets the entry's address die
 *     between the two reads.
 */
void sub_0806F41C(struct Unk6F41CProc *proc)
{
    s8 new_var;
    struct Unk0816E808Entry *tbl;
    int flag;
    int fill;
    struct Unk0816E808Entry **pp;
    struct Unk0816E808Entry *t;

    SetWinEnable(0, 0, 0);
    sub_08012358();

    flag = 0;
    flag = sub_0803CBD8(0x22) != flag;

    Proc_EndEach(gUnknown_08582AF4);
    sub_0806E210(proc->unk38, proc);

    t = gUnknown_0816E808;
    pp = &gUnknown_0816E808;
    if (t[proc->unk38].unk0d == 1)
    {
        { u8 v = gUnknown_03002B6C.raw8; gUnknown_03002B6C.raw8 = v | 0x80; }
    }
    else
    {
        gUnknown_03002B6C.raw8 &= 0x7f;
    }

    tbl = *pp;

    if ((tbl[proc->unk38].unk0e == 0) || flag)
    {
        Proc_EndEach(gUnknown_08582CAC);

        if (tbl[proc->unk38].unk00 <= 0x11)
        {
            sub_0806AEC4(proc->unk38);
        }
        else
        {
            Decompress((u8 *)tbl[proc->unk38].unk00, (void *)0x06000000);
            Decompress(tbl[proc->unk38].unk04, gUnknown_08499578);
            new_var = proc->unk38;
            ApplyPaletteExt(tbl[proc->unk38].unk08, 0, tbl[new_var].unk0c << 5);
        }
    }
    else
    {
        do
        {
            Proc_EndEach(gUnknown_08582CAC);
            Proc_Start(gUnknown_08582CAC, proc);
            fill = 0;
            CpuFastSet(&fill, (void *)0x06000000, 0x01000010);
            sub_08013C00();
            Proc_Goto(proc, 1);
        }
        while (0);
    }

    sub_08013AEC();
}
