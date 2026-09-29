#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08031A9C.
 * sub_08031A9C @ 0x08031A9C
 */

/* One record type described by two tags. The 0x1c-byte slot record at
 * gUnknown_02025564.unk20[] and struct Unk020280C0 are the same layout -- same
 * 0x11-byte name string at +0x02, same 0xff "empty" flag at +0x13 -- and this
 * function copies twelve of them wholesale, so the original source plainly had
 * ONE type for both. The two tags are left exactly as they are (LinkClearMapListAndNames
 * reads unk00/unk01/unk14/unk19/unk1a, which only Unk02025584 declares), and
 * the cast lives here instead.
 *
 * The cast has to go on the DESTINATION. Casting the source
 * (`g020280C0[i] = *(struct Unk020280C0 *)&g02025564.unk20[i]`) is identical
 * everywhere except the last loop's preheader, where it costs a NINTH pool
 * word: the ADDR_EXPR makes `&gUnknown_02025564.unk20[0]` its own address
 * constant, loaded in a single `ldr`. Left as a plain rvalue the source shares
 * loop 1's `gUnknown_02025564` word and adds 0x20 at run time
 * (`ldr r0,=g; adds r4,r0,#0; adds r4,#0x20`), which is what the ROM does and
 * the same shape LinkClearMapListAndNames's own +0x20 loop produces. Two instructions
 * longer in .text, one word shorter in the pool -- IDENTICAL SIZE either way,
 * so only the byte diff shows it and a size check never would.
 *
 * The volatility split is visible in the loop shapes and corroborates the
 * struct: unk00/unk02/unk05 are volatile, so loops 1-3 keep the `base + i`
 * form, while the non-volatile unk20 lets loop 4 strength-reduce to a pair of
 * pointer cursors. */
void LinkStoreMapList(void)
{
    int i;

    for (i = 0; i < 2; i++)
        gUnknown_02028030.unk10[i] = gUnknown_02025564.unk00[i];

    for (i = 0; i < 3; i++)
        gUnknown_02028030.unk2a[i] = gUnknown_02025564.unk02[i];

    for (i = 0; i < 0x18; i++)
        gUnknown_02028030.unk12[i] = gUnknown_02025564.unk05[i];

    if (gUnknown_0849B018->unk00 == 1)
    {
        for (i = 0; i < 12; i++)
            *(struct Unk02025584 *)&gUnknown_020280C0[i] = gUnknown_02025564.unk20[i];
    }
}
asm(".global sub_08031A9C\n.thumb_set sub_08031A9C, LinkStoreMapList\n");
