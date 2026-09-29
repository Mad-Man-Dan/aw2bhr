#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0806F0A0.
 * sub_0806F0A0 @ 0x0806F0A0
 */

#include "proc.h"
/* struct Unk08582764's +0x04 again (c_0806EB5C.c sets proc->unk34 from the
 * same field). Bit 0x8000 of that halfword means "the value is an index into
 * gUnknown_0858273C, pick an entry from the list it points at instead", and
 * the low byte is that index -- `ands r3,r1` overwrites the flag word in
 * place, which is why v is one variable and not two. */
struct Unk0806F0A0Proc
{
    /* 0x00 */ u8 filler_00[0x30];
    /* 0x30 */ s32 unk30;
    /* 0x34 */ u16 unk34;
};

void SoundRoom_RefreshCoPortrait(struct Unk0806F0A0Proc *proc)
{
    u16 v;

    v = gUnknown_08582764[proc->unk30].unk04;

    if (v & 0x8000)
        v = PickRandomCoFromList(proc->unk34, gUnknown_0858273C[v & 0xff]);

    if (proc->unk34 != v)
    {
        proc->unk34 = v;
        StartSoundRoomCoSwap(proc->unk34, proc);
    }
}
asm(".global sub_0806F0A0\n.thumb_set sub_0806F0A0, SoundRoom_RefreshCoPortrait\n");
