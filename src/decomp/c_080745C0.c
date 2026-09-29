#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080745C0.
 * sub_080745C0 @ 0x080745C0, sub_080745E0 @ 0x080745E0
 */

/* The 42 bytes of gUnknown_0202FDFC.unk12 are cleared through a pointer to the
 * s16 that follows them, not through the array member. That is what the pool
 * word says: the original materialises 0x0202FE38 (= &gUnknown_0202FDFC.unk3c)
 * once, keeps it live across the loop, derives the cursor as that minus one,
 * and stores the -1 through it with a ZERO displacement. Reaching unk12[] by
 * name instead makes -fforce-addr route the symbol through .rodata, because the
 * record is then used both before and after the loop.
 */
struct Unk80745C0
{
    /* 0x00 */ s16 unk_00;
};
#include "hardware.h"
struct Unk80745E0
{
    /* 0x00 */ u8 filler_00[0x4c];
    /* 0x4c */ u16 unk_4c;
};

void ClearWorldMapMarkers(void)
{
    s32 i;
    struct Unk80745C0 * p = (struct Unk80745C0 *)&gUnknown_0202FDFC.unk3c;

    for (i = 0; i < 0x2a; i++)
        ((u8 *)p)[i - 0x2a] = 0;

    p->unk_00 = -1;
}
asm(".global sub_080745C0\n.thumb_set sub_080745C0, ClearWorldMapMarkers\n");

void DarkenRampDown_Init(struct Unk80745E0 * proc)
{
    proc->unk_4c = 16;

    gUnknown_030030E0.bits.effect = 3;
    gUnknown_03001FFC = 16;
}
asm(".global sub_080745E0\n.thumb_set sub_080745E0, DarkenRampDown_Init\n");
