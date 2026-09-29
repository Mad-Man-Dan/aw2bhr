#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0806F550.
 * sub_0806F550 @ 0x0806F550
 */

#include "hardware.h"
#include "proc.h"
/* The menu's key handler: B confirms and breaks, Left/Right step unk38 round a
 * 0..0x19 ring.
 *
 * unk38 is `s8` and not `u8`: THUMB has no `ldrsb` with an immediate offset, so
 * an s8 field at +0x38 reads as `ldrb` plus `lsls #0x18` -- once with `asrs`
 * for the `> 0x19` compare and the 0x0202F2D8 store, once WITHOUT for the
 * `< 0` sign test, which is exactly what a u8 field could never produce.
 *
 * The `u16 t` local is load-bearing. `gpKeySt->repeated & DPAD_LEFT` is narrowed
 * `lsls #0x10; lsrs #0x10` where the neighbouring `& 2` and `& 0x10` tests are
 * not -- that only happens when the value lands in a u16 pseudo. It is also
 * why the wrap store in the Right branch is `strb r2` and not a fresh `movs`:
 * gcc knows t == 0 on that path and reuses the register for the literal 0. */
struct Unk0806F550Proc
{
    /* 0x00 */ u8 filler_00[0x38];
    /* 0x38 */ s8 unk38;
};

void SoundRoomGallery_Input(struct Unk0806F550Proc *proc)
{
    u16 t;

    if (gpKeySt->pressed & 2)
    {
        PlayMusicOrSfx2(0x66);
        gUnknown_0202F2D8 = proc->unk38;
        Proc_Break(proc);
    }
    else
    {
        t = gpKeySt->repeated & DPAD_LEFT;

        if (t != 0)
        {
            proc->unk38--;

            if (proc->unk38 < 0)
                proc->unk38 = 0x19;

            FlickSoundRoomArrow(0);
            Proc_Goto(proc, 0);
        }
        else if (gpKeySt->repeated & DPAD_RIGHT)
        {
            proc->unk38++;

            if (proc->unk38 > 0x19)
                proc->unk38 = 0;

            FlickSoundRoomArrow(1);
            Proc_Goto(proc, 0);
        }
    }
}
asm(".global sub_0806F550\n.thumb_set sub_0806F550, SoundRoomGallery_Input\n");
