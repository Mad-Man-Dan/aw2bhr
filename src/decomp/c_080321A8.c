#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080321A8.
 * sub_080321A8 @ 0x080321A8, sub_080321D4 @ 0x080321D4, sub_080321F0 @ 0x080321F0, sub_08032234 @ 0x08032234
 */

#include "proc.h"
#include "hardware.h"

/* A wait-for-both: this proc blocks until NEITHER script has a live instance.
 * Its 0x08039xxx twin is IsSparkleEffectActive, which returns the same predicate
 * instead of acting on it. */

void LinkWaitMapTransferProcs(ProcPtr proc)
{
    if (Proc_Find(gUnknown_0849B868) == 0 && Proc_Find(gUnknown_0849B7D8) == 0)
        Proc_Break(proc);
}
asm(".global sub_080321A8\n.thumb_set sub_080321A8, LinkWaitMapTransferProcs\n");

/* `lsls r0, r0, #0x18` before the `cmp` is GetMainMenuLock's u8 return being
 * re-narrowed at the call site; an int-returning predicate emits a bare
 * `cmp r0, #0`. */

void LinkGotoIfMainMenuLocked(ProcPtr proc)
{
    if (GetMainMenuLock() != 0)
        Proc_Goto(proc, 0);
}
asm(".global sub_080321D4\n.thumb_set sub_080321D4, LinkGotoIfMainMenuLocked\n");

/* `adds r1, #0xa; adds r1, r1, r0` on the record base is the member-array
 * hoist: gUnknown_0849B018 carries a per-slot byte array at +0x0a indexed by
 * its OWN signed unk06.
 *
 * The index's `ldrb; lsls #0x18; asrs #0x18` is what settled unk06 as VOLATILE
 * (wave 30, W30-A). A non-volatile `s8` member in an array-index position is
 * rewritten by agbcc into `movs rI,#6; ldrsb rD,[rB,rI]` -- 2 bytes shorter,
 * and the only spelling difference in this whole function. */

void sub_080321F0(ProcPtr proc)
{
    gUnknown_0849B060->unk00 = LinkScreenSetMessage(gUnknown_0849B060->unk00, 4, 2);
    gUnknown_0849B060->unk02 = 1;

    if (gUnknown_0849B018->unk0a[gUnknown_0849B018->unk06] != 2)
        Proc_Goto(proc, 1);
}

/* `gUnknown_0849B060->unk09 = gUnknown_0849B060->unk04` is a plain `ldrh` even
 * though unk04 is s16: the destination is a u8, so only the low byte survives
 * and agbcc uses the cheaper load. LinkPlayerCursor_Loop is what proves the field
 * signed. */

void LinkChoosePlayer_Loop(ProcPtr proc)
{
    if (gpKeySt->pressed & 2)
        PlayMusicOrSfx2(0x68);

    if (gpKeySt->pressed & 9)
    {
        gUnknown_0849B060->unk09 = gUnknown_0849B060->unk04;

        PlayMusicOrSfx2(0x71);
        Proc_Break(proc);
    }
}
asm(".global sub_08032234\n.thumb_set sub_08032234, LinkChoosePlayer_Loop\n");
