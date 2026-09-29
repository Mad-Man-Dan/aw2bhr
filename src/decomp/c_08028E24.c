#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08028E24.
 * sub_08028E24 @ 0x08028E24
 */

#include "proc.h"
/* The per-frame repeat callback of a sprite-burst proc: it steps a run of
 * `unk64` two-vector particles held at proc->unk4c and blits each one relative
 * to the map's scroll origin, until proc->unk66 reaches 8.
 *
 * proc->unk4c's elements are 8 bytes -- position at +0 / +2 and velocity at
 * +4 / +6, all four `ldrh` and `strh` with no sign extension anywhere, and the
 * ROM CSEs the element address into one register for all six accesses, which is
 * the plain `s[i].member` subscript.
 *
 * gUnknown_08499590's +0x04 and +0x06 are s16 (`movs rI,#4 / ldrsh`, the
 * s16-object tell) and are subtracted from the particle's u16 position, so they
 * are a pixel scroll origin -- distinct from the +0x0C / +0x0E tile camera
 * recorded in include/unknown-globals.h. Reached through a locally-declared
 * struct cast per that file's standing rule for this symbol; the two loads are
 * re-issued every iteration because PutSprite may move the map.
 *
 * `i` is s16, hence the `lsls #0x10; asrs #0x10` at the top of each iteration
 * and the re-read of proc->unk64 through r5 at the bottom. The `mov r8, r2` is
 * LICM parking PutSprite's constant fifth argument, not source. */

struct Unk08028E24Sprite
{
    /* 0x00 */ u16 unk00;
    /* 0x02 */ u16 unk02;
    /* 0x04 */ u16 unk04;
    /* 0x06 */ u16 unk06;
};
struct Unk08028E24Proc
{
    /* 0x00 */ PROC_HEADER;
    STRUCT_PAD(0x29, 0x4c);
    /* 0x4C */ struct Unk08028E24Sprite *unk4c;
    STRUCT_PAD(0x50, 0x64);
    /* 0x64 */ s16 unk64;
    /* 0x66 */ s16 unk66;
};

void RangeSpread_Loop(struct Unk08028E24Proc *proc)
{
    struct Unk08028E24Sprite *s = proc->unk4c;
    s16 i;

    if (proc->unk66 == 8)
    {
        ShowRangeOverlay(gUnknown_03003100.pos.unk00, gUnknown_03003100.pos.unk02, 1);
        Proc_End(proc);
        return;
    }

    proc->unk66++;

    if (proc->unk64 > 0x10)
        return;

    for (i = 0; i < proc->unk64; i++)
    {
        s[i].unk00 += s[i].unk04;
        s[i].unk02 += s[i].unk06;
        PutSprite(0,
                  s[i].unk00 - gMap->scrollX,
                  s[i].unk02 - gMap->scrollY,
                  gUnknown_08499FE4, 0);
    }
}
asm(".global sub_08028E24\n.thumb_set sub_08028E24, RangeSpread_Loop\n");
