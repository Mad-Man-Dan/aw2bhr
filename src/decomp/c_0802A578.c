#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802A578.
 * sub_0802A578 @ 0x0802A578
 */

struct Unk2A578Rec
{
    /* 00 */ u8 filler_00[0x02];
    /* 02 */ u8 unk02;
    /* 03 */ u8 unk03;
};
struct Unk2A578Proc
{
    /* 00 */ u8 filler_00[0x4c];
    /* 4c */ struct Unk2A578Rec *unk4c;
};

/* MATCHED, and the first of a byte-identical pair with UnitDestroyed_ScrollToUnit. Reads a
 * byte pair out of the record at +0x4c and forwards it. The two `ldrb`s come
 * off ONE `ldr` of the pointer, so the member really is a pointer and not two
 * inline bytes. */
void sub_0802A578(struct Unk2A578Proc *p)
{
    ScrollCameraToKeepCellInView(p->unk4c->unk02, p->unk4c->unk03);
}
