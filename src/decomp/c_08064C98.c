#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08064C98.
 * SetupObjectExitUp_Init @ 0x08064C98
 */

struct Unk64C98Obj
{
    /* 00 */ u8 filler_00[0x3a];
    /* 3a */ s16 unk3a;
    /* 3c */ s16 unk3c;
};

/* MATCHED, and the first of a byte-identical pair with sub_08065050. Two
 * halfword stores.
 *
 * `p->unk3c = 0xFFFE;` and not `= -2`: the ROM loads the value from the literal
 * pool (`ldr r1, =0x0000FFFE`), which is what a positive constant too large for
 * `movs` produces; `-2` into an s16 goes through `movs #2; negs`. Same readout
 * as sub_08005F1C's `0xFF`. */
void SetupObjectExitUp_Init(struct Unk64C98Obj *p)
{
    p->unk3c = 0xFFFE;
    p->unk3a = 0;
}
asm(".global sub_08064C98\n.thumb_set sub_08064C98, SetupObjectExitUp_Init\n");
