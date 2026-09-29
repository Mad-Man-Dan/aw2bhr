#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0805FB70.
 * sub_0805FB70 @ 0x0805FB70
 */

struct Unk5FB70Rec
{
    /* 0x00 */ u8 filler_00[9];
    /* 0x09 */ u8 unk09_0 : 3;
               u8 unk09_3 : 3;
               u8 unk09_6 : 2;
};
struct Unk5FB70Unit
{
    /* 0x00 */ u8 filler_00[9];
    /* 0x09 */ u8 unk09_0 : 6;
               u8 unk09_6 : 2;
    /* 0x0a */ u8 filler_0a[2];
};

void AiBoardTransport(void)
{
    union Unk802C57CBuf v;
    struct Unit *u;

    v.pos.unk00 = 0x270F;
    GenerateUnitMovementMap(gUnknown_030040D8);
    sub_0805FC1C(((struct Unk5FB70Rec *)gUnknown_030040D8)->unk09_3, &v);
    if (v.pos.unk00 != 0x270F)
    {
        ((struct Unk5FB70Rec *)gUnknown_030040D8)->unk09_3 = 0;
        u = &gUnits[
            gMap->unit[
                gMap->rowOffset[v.pos.unk02]
                + v.pos.unk00]];
        ((struct Unk5FB70Unit *)u)->unk09_6++;
        AiPublishAction(v.spos.unk00, v.spos.unk02, 7, 0, 0);
    }
}
asm(".global sub_0805FB70\n.thumb_set sub_0805FB70, AiBoardTransport\n");
