#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802C16C.
 * sub_0802C16C @ 0x0802C16C, sub_0802C184 @ 0x0802C184
 */

/* Two bare statements. sub_0802C154 takes `int` (the promoted definition in
 * src/decomp/c_0802C154.c), so the u16 gUnknown_030033EC arrives as a plain
 * `ldrh` with no narrowing -- contrast sub_0802C118, whose callee takes u8 and
 * gets an `ldrb` out of the same global. `pop {r0}; bx r0`, so void.
 */

void sub_0802C16C(void)
{
    sub_0802C154(gUnknown_030033EC);
    ParkMapState();
}

/* sub_0802C16C's sibling: the same sub_0802C154(gUnknown_030033EC) opener, then
 * a guarded four-argument call instead of a bare one.
 *
 * `adds r0, #0x32; ldrb r0, [r0]` rather than `ldrb r0, [r0, #0x32]` is not a
 * member-array tell here -- 0x32 is simply past the 5-bit displacement `ldrb`
 * offers, so the base has to be advanced first. It is the scalar
 * gPlaySt.unk32.
 *
 * The four `mov #imm8` argument setups are in argument order because they are
 * all one operand class; SendActionCommand's declared (int, u8, int, int) costs
 * nothing extra for literal zeroes. `pop {r0}; bx r0`, so void.
 */

void sub_0802C184(void)
{
    sub_0802C154(gUnknown_030033EC);

    if (gPlaySt.savingEnabled != 0)
        SendActionCommand(0x13, 0, 0, 0);
}
