#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080375D4.
 * sub_080375D4 @ 0x080375D4
 */

/* The proc-slot frame tick for the gUnknown_03001470 slot at unk18. unk1e is
 * the frame counter struct Unk03001470 already documents; the `ldrh` before
 * the increment is the POST-increment's saved value, and `& 0x3f` makes the
 * cycle 64 frames long. Three of the 64 phases do something. */
void MapPreviewPictureScript_Loop(struct Unk03001470 *p)
{
    switch (p->unk1e++ & 0x3f)
    {
    case 0:
        sub_0801B6EC((void *)p->unk18);
        break;
    case 1:
        sub_0801B6FC((void *)p->unk18);
        break;
    case 0x16:
        sub_08037A78(p->unk18);
        break;
    }
}
asm(".global sub_080375D4\n.thumb_set sub_080375D4, MapPreviewPictureScript_Loop\n");
