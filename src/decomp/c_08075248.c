#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08075248.
 * sub_08075248 @ 0x08075248
 *
 * Named per src/aw2e-names.s (proc-table labels auto-generated from
 * AW2E.lua). The old sub_XXXXXXXX symbols are kept as linker aliases
 * below so every other unit keeps resolving them unchanged.
 */

/* sub_08075008's twin one field-group up: the counter is the u16 at +0x3a
 * rather than the word at +0x34, so the `>> 3` is unsigned for free, and the
 * sprite blob is gUnknown_081CC4F0. The jitter lookup is bound to its own local
 * for the same reason as there -- it is what puts the table `ldr` ahead of the
 * two coordinate `ldrsh`s. */
struct Unk8075248
{
    /* 0x00 */ u8 filler_00[0x34];
    /* 0x34 */ s16 unk34;
    /* 0x36 */ s16 unk36;
    /* 0x38 */ u16 unk38;
    /* 0x3a */ u16 unk3a;
};

void DifficultyStars_HoldLoop(struct Unk8075248 *proc)
{
    int d = gUnknown_0861436C[(proc->unk3a >> 3) & 3];

    PutSprite(1, proc->unk34, proc->unk36 + d,
              gUnknown_081CC4F0, gUnknown_0861433C[proc->unk38]);
    proc->unk3a++;
}

asm(".global sub_08075248\n.thumb_set sub_08075248, DifficultyStars_HoldLoop\n");
