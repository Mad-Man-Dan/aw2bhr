#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08061E80.
 * sub_08061E80 @ 0x08061E80
 */

/* A pointer-parameter type with two bitfield containers. +0x04 is read as
 * seven bits at bit 0 (`ldrb; lsls #0x19; lsrs #0x19`) and +0x09 has three bits
 * at bit 0 cleared (`mov #8; neg; and`, mask ~0x07).
 */
struct Unk61E80
{
    /* 00 */ u8 filler_00[0x04];
    /* 04 */ u8 unk04_0 : 7;
    /* 04 */ u8 unk04_7 : 1;
    /* 05 */ u8 filler_05[0x09 - 0x05];
    /* 09 */ u8 unk09_0 : 3;
    /* 09 */ u8 unk09_3 : 5;
};

void AiUpdateModeRepair(struct Unk61E80 *p)
{
    if (p->unk04_0 > 0x5b)
        p->unk09_0 = 0;
}
asm(".global sub_08061E80\n.thumb_set sub_08061E80, AiUpdateModeRepair\n");
