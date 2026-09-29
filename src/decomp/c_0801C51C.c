#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801C51C.
 * sub_0801C51C @ 0x0801C51C
 */

/* TWO parameters, and the second is invisible except by absence: this body
 * never touches r1, yet it calls AP_LoadDefinition, whose first act is
 * `str r1, [r0]`. A one-parameter AP_SetDefinition would be storing whatever the
 * caller happened to leave behind. A pass-through argument costs no
 * instruction, so the arity has to be read off the callee. */
void AP_SetDefinition(struct Unk0801C210 *a1, void *a2)
{
    if (a1 != NULL && a1->unk00 != NULL)
    {
        AP_LoadDefinition(a1, a2);
        AP_ExecDummyFrame(a1);
    }
}
asm(".global sub_0801C51C\n.thumb_set sub_0801C51C, AP_SetDefinition\n");
