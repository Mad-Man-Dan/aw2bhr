#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0806E510.
 * sub_0806E510 @ 0x0806E510
 */

#include "proc.h"
/* Spawns the six gUnknown_08582B14 procs -- one per row of gUnknown_08582A7C,
 * whose 0x78 ROM extent is exactly the six 0x14-byte rows this loop walks --
 * and files each returned proc in the parent's +0x3c pointer array.
 *
 * `proc` is ONE pseudo, not two: binding a typed `struct ... *p = proc;` local
 * alongside the ProcPtr parameter makes agbcc spill the parameter to
 * [sp, #0xc], grow the frame from 0xc to 0x10 and re-load it for the parent
 * argument on every pass. The cast at the point of use keeps the single
 * register (sb) the ROM uses for both the array base and the parent argument.
 *
 * The two middle arguments are addressed off `sl + 4 + i*0x14` while the other
 * three use the walking `r6`; that second induction variable is
 * strength_reduce's and must not be authored. */

struct Unk0806E510Proc
{
    /* 0x00 */ u8 filler_00[0x3c];
    /* 0x3c */ ProcPtr unk3c[6];
};

void SpawnSoundRoomButtons(ProcPtr proc)
{
    int i;

    for (i = 0; i < 6; i++)
        ((struct Unk0806E510Proc *)proc)->unk3c[i] =
            StartSoundRoomButton(gUnknown_08582A7C[i].unk00,
                         gUnknown_08582A7C[i].unk04,
                         gUnknown_08582A7C[i].unk08,
                         gUnknown_08582A7C[i].unk10,
                         gUnknown_08582A7C[i].unk0c,
                         i, proc);
}
asm(".global sub_0806E510\n.thumb_set sub_0806E510, SpawnSoundRoomButtons\n");
