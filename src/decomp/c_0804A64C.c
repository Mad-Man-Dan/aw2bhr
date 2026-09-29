#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0804A64C.
 * sub_0804A64C @ 0x0804A64C
 */

/* The index `i` is a u8 LOCAL, not a re-read of unk5d: the ROM keeps it in r4
 * across the `bl` and the clamp writes it back through `lsls #0x18; lsrs #0x18`,
 * which is a u8 assignment and not a cast at a use.
 *
 * `blo` (unsigned) on two u8 fields, so the clamp is `i >= unk5f`, and the
 * fixup value is `unk5f - 1` re-narrowed to u8.
 *
 * `lsls r0, r1, #4; subs r0, r0, r1` is a MULTIPLY by 15 -- agbcc's expansion
 * of `unk20 * 15`, not a hand-rolled shift-and-subtract.
 *
 * gUnknown_030044E0 is spelled out at every use rather than bound to a local:
 * the ROM keeps only the ADDRESS in r5 across the `bl` and reloads the pointer
 * afterwards (`ldr r1, [r5]`), which is what a global read on both sides of a
 * call compiles to. A local pointer would have survived the call in a
 * callee-saved register instead. */
void NameEntry_TypeChar(void)
{
    u8 i = gUnknown_030044E0->unk5d;

    if (i >= gUnknown_030044E0->unk5f)
        i = gUnknown_030044E0->unk5f - 1;

    gUnknown_030044E0->unk2c[i] =
        GetNameEntryGridChar(gUnknown_030044E0->unk20 * 15 + gUnknown_030044E0->unk1e);
}
asm(".global sub_0804A64C\n.thumb_set sub_0804A64C, NameEntry_TypeChar\n");
