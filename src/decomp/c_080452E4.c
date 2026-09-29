#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080452E4.
 * sub_080452E4 @ 0x080452E4
 */

struct Unk452E4
{
    /* 0x00 */ u8 filler_00[0x3c];
    /* 0x3c */ s16 unk3c;
    /* 0x3e */ u8 filler_3e[0x02];
    /* 0x40 */ int unk40;
};

void CoPowerUnitSparkle_ScrollToUnit(struct Unk452E4 *p)
{
    ScrollCameraToKeepCellInView(p->unk3c, p->unk40);
}
asm(".global sub_080452E4\n.thumb_set sub_080452E4, CoPowerUnitSparkle_ScrollToUnit\n");
