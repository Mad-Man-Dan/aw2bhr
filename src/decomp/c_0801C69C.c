#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801C69C.
 * sub_0801C69C @ 0x0801C69C
 */

/* The animation-handle initialiser: AP_Create allocates the slot and calls
 * this with its own three arguments forwarded. +0x20 is set to 0 and then to 2
 * when the u8 flag is non-zero -- two stores to the same byte, not a ternary
 * (a ternary puts the constant in a register and stores once).
 *
 * `a1->unk18 = 0;` APPEARS TWICE and that is the ROM, not a slip: `strh r5,
 * [r4, #0x18]` is emitted at +0x1c and again at +0x2a with the +0x1e/+0x14/
 * +0x21 stores between them. The two zeroes even come from different pseudos
 * (r5 for the halfwords, r0 for the +0x21 byte). */
void AP_Init(struct Unk0801C210 *a1, void *a2, u16 a3, u8 a4)
{
    a1->unk20 = 0;
    if (a4)
        a1->unk20 = 2;

    AP_LoadDefinition(a1, a2);

    a1->unk24 = NULL;
    a1->unk22 = 0;
    a1->unk18 = 0;
    a1->unk1e = a3;
    a1->unk14 = NULL;
    a1->unk21 = 0;
    a1->unk18 = 0;
    a1->unk1a = 0x100;
    a1->unk1c = 0;
    a1->unk28 = 0;
    AP_ExecFrame(a1);
}
asm(".global sub_0801C69C\n.thumb_set sub_0801C69C, AP_Init\n");
