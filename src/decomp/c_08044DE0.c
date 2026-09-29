#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08044DE0.
 * sub_08044DE0 @ 0x08044DE0, sub_08044DF8 @ 0x08044DF8
 */

struct Unk08044DE0
{
    /* 0x00 */ u8 filler_00[0x29];
    /* 0x29 */ u8 unk29;
    /* 0x2a */ u8 unk2a;
    /* 0x2b */ u8 unk2b;
};
struct Unk08044DF8
{
    /* 0x00 */ u8 filler_00[0x29];
    /* 0x29 */ u8 unk29;
    /* 0x2a */ u8 unk2a;
    /* 0x2b */ u8 unk2b;
};

void CoPowerDamageHeal_ResetCursors(struct Unk08044DE0 *p)
{
    p->unk29 = 1;
    p->unk2a = 1;
    p->unk2b = 0;
}
asm(".global sub_08044DE0\n.thumb_set sub_08044DE0, CoPowerDamageHeal_ResetCursors\n");

void CoPowerDamageHeal_ResetCursors2(struct Unk08044DF8 *p)
{
    p->unk29 = 1;
    p->unk2a = 1;
    p->unk2b = 0;
}
asm(".global sub_08044DF8\n.thumb_set sub_08044DF8, CoPowerDamageHeal_ResetCursors2\n");
