#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08031BE0.
 * sub_08031BE0 @ 0x08031BE0
 */

struct UnkP31BE0
{
    /* 0x00 */ u8 filler_00[0x2f];
    /* 0x2f */ u8 unk2f;
};

void sub_08031BE0(struct UnkP31BE0 *a)
{
    gUnknown_0849B060->unk0a = a->unk2f;
}
