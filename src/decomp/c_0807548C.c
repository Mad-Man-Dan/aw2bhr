#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0807548C.
 * sub_0807548C @ 0x0807548C
 */

#include "hardware.h"
#include "proc.h"
/* Starts the gUnknown_086143B8 blend proc (c_08075368.c is its body) and arms
 * the blend registers.
 *
 * THE FIRST TWO PARAMETERS ARE s16, not the `int` the header used to declare.
 * With `int` the subtract happens in SImode before the narrowing (`subs #16;
 * lsls #0x10; lsrs #0x10`) and needs no pool word; only a 16-bit parameter lets
 * combine fold the subtract into the SHIFTED domain, giving the ROM's `lsls
 * #0x10; adds <0xFFF00000>; lsrs #0x10` -- and that is also what makes both
 * parameters share the single 0xFFF00000 pool word. u16 is wrong for a
 * different reason: it hoists both `lsls` above the pool load.
 *
 * The locals are s16 carrying an EXPLICIT (u16) truncation, and the two halves
 * of that are separately load-bearing. The (u16) is why the narrowing is `lsrs`
 * and not `asrs`; the s16 declaration is why `-x` re-widens with `lsls #0x10;
 * asrs #0x10` before the `rsbs` instead of collapsing to a bare `negs`. Written
 * with u16 locals and `-(s16)x` at the use, agbcc knows the pseudo is already
 * 16-bit and drops the extension -- 8 bytes across the two coordinates.
 *
 * +0x2e and +0x30 are u16 HERE, where the promoted c_08075368.c calls them s16:
 * this function reads them straight back for SetBgScrollShadow's u16 parameters and
 * the ROM uses plain `ldrh`, which an s16 field cannot produce. The paired
 * stores are chained assignments -- `p->unk2a = p->unk2e = v` evaluates the
 * inner one first, which is what puts +0x2e ahead of +0x2a.
 *
 * The blend tail is c_0806BB08.c's verbatim with 0xC for its 9: two `.raw`
 * read-modify-writes that agbcc folds into ONE `ldrh`/`strh` pair because the
 * shadow is not volatile. */
struct Unk807548C
{
    /* 0x00 */ u8 filler_00[0x2a];
    /* 0x2a */ u16 unk2a;
    /* 0x2c */ u16 unk2c;
    /* 0x2e */ u16 unk2e;
    /* 0x30 */ u16 unk30;
    /* 0x32 */ u16 unk32;
    /* 0x34 */ u16 unk34;
    /* 0x36 */ u8 filler_36[0x2];
    /* 0x38 */ int unk38;
    /* 0x3c */ int unk3c;
};

void StartWorldMapSelectionFrame(s16 a1, s16 a2, int a3, ProcPtr parent)
{
    struct Unk807548C *proc;
    s16 x;
    s16 y;

    x = (u16)(a1 - 0x10);
    y = (u16)(a2 - 0x10);

    if (a3 != 2)
    {
        x = 0xFFE2;
        y = 0xFFC0;
    }

    proc = Proc_Start(gUnknown_086143B8, parent);

    proc->unk2a = proc->unk2e = -x;
    proc->unk2c = proc->unk30 = -y;
    proc->unk32 = proc->unk34 = 0;
    proc->unk38 = a3;
    proc->unk3c = 0;

    SetBgScrollShadow(1, proc->unk2e, proc->unk30);

    gUnknown_030030E0.bits.effect = 1;
    gUnknown_03002020 = 0xC;
    gUnknown_03002B28 = 0x10;
    gUnknown_03001FFC = 0;
    *(u16 *)&gUnknown_030030E0 = (*(u16 *)&gUnknown_030030E0 & 0xFFE0) | 2;
    *(u16 *)&gUnknown_030030E0 = (*(u16 *)&gUnknown_030030E0 & 0xE0FF) | 0x1800;
}
asm(".global sub_0807548C\n.thumb_set sub_0807548C, StartWorldMapSelectionFrame\n");
