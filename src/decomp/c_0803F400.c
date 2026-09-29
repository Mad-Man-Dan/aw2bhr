#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803F400.
 * sub_0803F400 @ 0x0803F400
 */

#include "proc.h"
/* The proc VolcanoRock_WaitImpact also runs on: +0x50 holds the AP_Create handle and
 * +0x54/+0x58 are the screen-space x/y that VolcanoRock_WaitImpact converts back to
 * camera-relative coordinates. */
struct UnkF400Proc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x2c);
    /* 2c */ int unk2c;
    /* 30 */ STRUCT_PAD(0x30, 0x50);
    /* 50 */ struct Unk0801C210 *unk50;
    /* 54 */ int unk54;
    /* 58 */ int unk58;
};

/* `ldr r1, [r1]` before any index proves gUnknown_08499590 is a POINTER, and
 * `movs r2, #6; ldrsh r1, [r1, r2]` is a SIGNED halfword at a constant
 * displacement -- Thumb has no immediate-offset ldrsh, so the register-offset
 * form is how agbcc spells `*(s16 *)(p + 6)`, not an array index. */
void VolcanoRock_Init(struct UnkF400Proc *proc)
{
    struct Unk0801C210 *sprite = AP_Create(gUnknown_081171EC, 1, 1);

    proc->unk50 = sprite;
    proc->unk54 = proc->unk2c * 16 + 8;
    proc->unk58 = gMap->scrollY + 0x10;

    sprite->unk22 = 0x51CA;
}
asm(".global sub_0803F400\n.thumb_set sub_0803F400, VolcanoRock_Init\n");
