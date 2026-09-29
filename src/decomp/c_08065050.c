#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08065050.
 * sub_08065050 @ 0x08065050
 */

struct Unk65050Obj
{
    /* 00 */ u8 filler_00[0x3a];
    /* 3a */ s16 unk3a;
    /* 3c */ s16 unk3c;
};

/* MATCHED. Byte-for-byte the same function as SetupObjectExitUp_Init -- identical
 * instruction stream and identical pool words. One C body, two
 * addresses; read that one for the derivation. */
void sub_08065050(struct Unk65050Obj *p)
{
    p->unk3c = 0xFFFE;
    p->unk3a = 0;
}
