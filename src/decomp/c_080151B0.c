#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080151B0.
 * sub_080151B0 @ 0x080151B0, sub_08015224 @ 0x08015224, sub_0801527C @ 0x0801527C, sub_080152C0 @ 0x080152C0, sub_080152EC @ 0x080152EC
 */


/*
 * InitSlotScript -- fill in a gUnknown_03001470 script slot.
 *
 * Slot `b` is pointed at the script blob `a` (kept both as a pointer and as
 * the integer in the slot's first word), takes `c` in .unk14, and has
 * everything else reset: the timers and flags to 0, and the four words at
 * .unk28..unk34 to -1. The callers below set the slot's mode word and start
 * it running.
 *
 * Why the C looks odd: the four -1 stores are one chained assignment, not four
 * statements. As separate statements the compiler computes one address and
 * reuses the register, where the original computes all four addresses first
 * and then stores right to left -- which is a chain's evaluation order.
 */
void InitSlotScript(const void *a, u8 b, u8 c)
{
    gUnknown_03001470[b].unk00 = (u32)a;
    gUnknown_03001470[b].unk04 = a;
    gUnknown_03001470[b].unk10 = 0;
    gUnknown_03001470[b].unk0c = 0;
    gUnknown_03001470[b].unk38 = 0;
    gUnknown_03001470[b].unk14 = c;
    gUnknown_03001470[b].unk18 = 0;
    gUnknown_03001470[b].unk08 = 0;

    gUnknown_03001470[b].unk28 = gUnknown_03001470[b].unk2c =
        gUnknown_03001470[b].unk30 = gUnknown_03001470[b].unk34 = -1;

    gUnknown_03001470[b].unk24 = 0;
    gUnknown_03001470[b].unk22 = 0;
    gUnknown_03001470[b].unk20 = 0;
    gUnknown_03001470[b].unk1e = 0;
}
asm(".global sub_080151B0\n.thumb_set sub_080151B0, InitSlotScript\n");


/*
 * sub_08015224 -- install a script blob in slot `b` and start it, mode 0.
 *
 * gUnknown_03001FBC holds the slot the script system is currently working on:
 * it is saved, pointed at this slot while InitSlotScript fills the slot in and
 * StepSlotScript starts it, then restored. Returns the slot index.
 * sub_0801527C below is the same function storing 4 in the mode word.
 *
 * Why the C looks odd: the slot index is s16 here and u8 in sub_0801527C. Both
 * arrive zero-extended, but this one is sign-extended again where it indexes
 * the array; a u8 or u16 parameter indexes straight off the incoming value and
 * the output no longer matches.
 */
s8 sub_08015224(const void *a, s16 b, u8 c)
{
    s16 saved = gUnknown_03001FBC;

    gUnknown_03001FBC = b;
    InitSlotScript(a, b, c);
    gUnknown_03001470[b].unk12 = 0;
    StepSlotScript(b);
    gUnknown_03001FBC = saved;

    return b;
}


/*
 * sub_0801527C -- install a script blob in slot `b` and start it, mode 4.
 *
 * Identical to sub_08015224 above apart from that 4 and the narrower slot
 * index. gUnknown_03001FBC, the slot currently being worked on, is saved,
 * pointed at this slot for the duration, and restored. Returns the slot index.
 */
s8 sub_0801527C(const void *a, u8 b, u8 c)
{
    s16 saved = gUnknown_03001FBC;

    gUnknown_03001FBC = b;
    InitSlotScript(a, b, c);
    gUnknown_03001470[b].unk12 = 4;
    StepSlotScript(b);
    gUnknown_03001FBC = saved;

    return b;
}


/*
 * sub_080152C0 -- start a script in the first free slot, mode 0.
 *
 * FindSlotScript scans the thirty gUnknown_03001470 slots for one whose script
 * pointer equals its argument; with 0 that is the first unused slot, or -1 when
 * they are all taken. On success sub_08015224 fills the slot in and starts it.
 * Returns the slot index, or -1.
 *
 * The blob arrives as an s32 rather than as a pointer because src/proc.c
 * passes it that way, and the cast here only undoes that. Leave the parameter
 * type alone: proc.c is matching source that this project does not edit.
 */
s8 sub_080152C0(s32 a, u8 b)
{
    s8 i = FindSlotScript(0);

    if (i != -1)
        sub_08015224((const void *)a, i, b);

    return i;
}


/*
 * sub_080152EC -- start a script in the first free slot, mode 4, and return
 * the slot.
 *
 * sub_080152C0 above with sub_0801527C in place of sub_08015224, returning a
 * pointer to the gUnknown_03001470 entry instead of its index, or NULL when no
 * slot is free.
 *
 * Why the C looks odd: the "got a slot" case is the `if` body and the NULL
 * return is the fall-through. Writing the NULL case as a leading early return
 * compiles to the same instructions with the two blocks swapped, and drags the
 * constants the function loads into the middle of the code.
 */
struct Unk03001470 *sub_080152EC(const void *a, u8 b)
{
    s8 i;

    i = FindSlotScript(0);

    if (i != -1)
    {
        sub_0801527C(a, i, b);
        return &gUnknown_03001470[i];
    }

    return NULL;
}
