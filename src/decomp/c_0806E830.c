#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0806E830.
 * sub_0806E830 @ 0x0806E830
 */

#include "proc.h"
/* Two arrow sprites nudged in from a 3-byte row of gUnknown_08582C1C, each with
 * its own countdown: while the counter is non-zero an Interpolate offset is
 * subtracted (left arrow) or added (right arrow) and the counter ticks down.
 *
 * The row offset `proc->unk60 * 3` is computed BEFORE the SoundRoomCycleArrowPalette call and
 * held in r7 for all three reads -- that is the source order, not scheduling.
 * `movs r0,#0x10` serves both `0x10 - counter` and Interpolate's fifth
 * argument; one constant, two uses.
 *
 * Both offsets are initialised to 0 above the two `if`s, and the second one
 * lives in r8 because it has to survive the first PutSprite. */
struct Unk0806E830Proc
{
    /* 0x00 */ u8 filler_00[0x58];
    /* 0x58 */ int unk58;
    /* 0x5c */ int unk5c;
    /* 0x60 */ int unk60;
};

void SoundRoomArrows_Loop(struct Unk0806E830Proc *proc)
{
    int i;
    int a;
    int b;

    a = 0;
    b = 0;
    i = proc->unk60 * 3;

    SoundRoomCycleArrowPalette();

    if (proc->unk58 != 0)
    {
        a = Interpolate(4, 8, 0, 0x10 - proc->unk58, 0x10);
        proc->unk58--;
    }

    if (proc->unk5c != 0)
    {
        b = Interpolate(4, 8, 0, 0x10 - proc->unk5c, 0x10);
        proc->unk5c--;
    }

    PutSprite(0, gUnknown_08582C1C[i] - a, gUnknown_08582C1C[i + 2], gUnknown_0816E800, 0);
    PutSprite(0, gUnknown_08582C1C[i + 1] + b, gUnknown_08582C1C[i + 2], gUnknown_0816E800, 2);
}
asm(".global sub_0806E830\n.thumb_set sub_0806E830, SoundRoomArrows_Loop\n");
