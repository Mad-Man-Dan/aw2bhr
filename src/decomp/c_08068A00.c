#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08068A00.
 * sub_08068A00 @ 0x08068A00
 */

#include "proc.h"
#include "hardware.h"
struct Unk68A00Proc
{
    /* 0x00 */ u8 filler_00[0x2c];
    /* 0x2c */ int unk2c;
};

/* Per-frame step of a cutscene proc: acts when its countdown (unk2c) hits one
 * of a few values, then counts down by one.
 *
 * Why the C looks odd: the compiler builds its compare tree from the number of
 * case values, so the original switch had more values than it acts on. The
 * `goto done` arms for 1 and 0x28..0x2a stand in for those unused values; their
 * exact numbers are not recoverable, only that they sort into the right places. */
void sub_08068A00(struct Unk68A00Proc *proc)
{
    switch (proc->unk2c)
    {
    case 1:
        goto done;
    case 0x28:
        goto done;
    case 0x29:
        goto done;
    case 0x2a:
        goto done;
    case 0xc7:
        SetDispEnable(1, 1, 1, 0, 1);
        Decompress(gUnknown_0817DE24, (void *)0x06009400);
        break;
    case 0xb4:
        Decompress(gUnknown_0818E364, (void *)0x06010000);
        break;
    case 0x80:
        sub_080678D4(-1);
        break;
    case 0x62:
        goto done;
    case 0x60:
        sub_080673D0(0x40, 1, proc);
        break;
    case 0x58:
        goto done;
    case 0x4e:
        goto done;
    case 0x26:
        goto done;
    case 0:
        Proc_EndEach(gUnknown_08580FF4);
        Proc_EndEach(gUnknown_08580FCC);
        Proc_Break(proc);
        break;
    }

done:
    proc->unk2c--;
}
