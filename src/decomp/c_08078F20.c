#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08078F20.
 * sub_08078F20 @ 0x08078F20
 */

#include "hardware.h"
#include "proc.h"
/* The fade-OUT counterpart of FadeToBlackFrom6_Loop, on a different counter (+0x64),
 * a different period (4) and with the counter incremented at the END rather
 * than the start -- which is why r4 holds &proc->unk_64 across the whole body
 * and the `ldrh`/`adds`/`strh` sits after the Proc_Break test.
 *
 * gUnknown_081D92E4 is the fourth `-fforce-addr` word holding 0x03001FFC.
 * Needs "rodata": ["0x081D92E4"] in promoted.json. */

struct Unk8078D40
{
    /* 0x00 */ u8 filler_00[0x64];
    /* 0x64 */ s16 unk_64;
    /* 0x66 */ u8 filler_66[0x02];
    /* 0x68 */ s16 unk_68;
};

void ResultsScreen_FadeFromWhite_Loop(struct Unk8078D40 *proc)
{
    if (DivRem(proc->unk_64, 4) == 0)
        gUnknown_03001FFC--;

    if (gUnknown_03001FFC == 0)
        Proc_Break(proc);

    proc->unk_64++;
}
asm(".global sub_08078F20\n.thumb_set sub_08078F20, ResultsScreen_FadeFromWhite_Loop\n");
