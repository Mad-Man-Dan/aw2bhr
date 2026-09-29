#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080401A4.
 * UnitDestroyed_ScrollToUnit @ 0x080401A4
 */

struct Unk401A4Rec
{
    /* 00 */ u8 filler_00[0x02];
    /* 02 */ u8 unk02;
    /* 03 */ u8 unk03;
};
struct Unk401A4Proc
{
    /* 00 */ u8 filler_00[0x4c];
    /* 4c */ struct Unk401A4Rec *unk4c;
};

/* MATCHED. Byte-for-byte the same function as sub_0802A578 -- identical
 * instruction stream and identical pool words. One C body, two
 * addresses; read that one for the derivation. */
void UnitDestroyed_ScrollToUnit(struct Unk401A4Proc *p)
{
    ScrollCameraToKeepCellInView(p->unk4c->unk02, p->unk4c->unk03);
}
asm(".global sub_080401A4\n.thumb_set sub_080401A4, UnitDestroyed_ScrollToUnit\n");
