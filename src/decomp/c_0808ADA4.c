#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0808ADA4.
 * sub_0808ADA4 @ 0x0808ADA4
 */

/* WaitForFlashWrite_Common -- the flash WaitForFlashWrite poll/timeout loop.
 * PARKED at 14.3%, candidate 148 vs 140 (+8). Every branch, every compare and
 * the whole control-flow skeleton are already right; read the diff, not the
 * score.
 *
 * SETTLED, do not re-derive: all three parameter types (u8 phase, u8 *addr,
 * u8 lastData -- the two u8s from the entry `lsls/lsrs #24` pair, the pointer
 * from `gUnknown_03000F6C`'s declared `u8 (*)(u8 *)`), the u16 return,
 * `gUnknown_03005C78->unk14` as the 0x1CC2 chip-id test, the FLASH_WRITE
 * spelling, and `void StopFlashTimer(void)`.
 *
 * THE RESIDUAL IS ONE MISSING LICM HOIST. The ROM hoists `phase | 0xC000` into
 * the loop preheader (`movs r0,#0xc0; lsls r0,#8; orrs r4,r0; lsls r4,#0x10`)
 * and leaves only `lsrs r4,#0x10; mov r8,r4` at the use inside the timeout
 * block. agbcc will not hoist it here because that block is conditionally
 * executed, so the whole expression stays at the use.
 *
 * READ THE SPLIT SHIFT PAIR AS A CONSEQUENCE OF THE HOIST, NOT AS A WIDTH
 * FACT. It is tempting to conclude from `lsls #0x10 ... lsrs #0x10` that the
 * OR needs a u16 truncation and therefore that `phase` is wider than u8. It is
 * the opposite: with `u8 phase` the value provably fits in 16 bits, so when the
 * truncation lands in ONE block combine deletes it outright -- which is exactly
 * what this candidate does, emitting `orrs` with no shifts at all. The ROM's
 * pair survives only because LICM split it across two basic blocks, where
 * combine cannot see the two halves together. Do NOT retype `phase` to
 * manufacture the shifts; that inverts the evidence.
 *
 * The +8 bytes are the second-order effect of the same thing: with no hoist,
 * `phase` stays live all the way to the timeout block (r9) alongside `result`
 * (r4), one live value more than the ROM, and agbcc then spills
 * gUnknown_03000F6C's ADDRESS into its own -fforce-addr .rodata slot
 * (`R_ARM_ABS32 .rodata`) and reaches it with an extra `ldr` per call, where
 * the ROM keeps a direct `ldr r7, =gUnknown_03000F6C` pool word. Fix the hoist
 * and the register assignment, the pool word and the 8 bytes all follow.
 *
 * MEASURED AND WORSE (do not re-run): folding the two exits into one shared
 * `break` by nesting the second poll's test --
 *     if (gUnknown_03000F74 != 0) {
 *         if (gUnknown_03000F6C(addr) != lastData) { ...; result = ...; }
 *         break;
 *     }
 * -- scores 8.6% and is still +8. It hoists the gUnknown_03000F74 test ahead of
 * the loop's FIRST poll, which reorders the whole loop head. The form below,
 * with two independent `break`s, is the one that reproduces the ROM's block
 * order.
 */

u16 WaitForFlashWrite_Common(u8 phase, u8 *addr, u8 lastData)
{
    u16 result = 0;

    StartFlashTimer(phase);

    while (gUnknown_03000F6C(addr) != lastData)
    {
        if (gUnknown_03000F74 != 0)
        {
            if (gUnknown_03000F6C(addr) == lastData)
                break;

            if (gUnknown_03005C78->unk14 == 0x1CC2)
                *(vu8 *)(0x0E000000 + 0x5555) = 0xF0;

            result = phase | 0xC000;
            break;
        }
    }

    StopFlashTimer();

    return result;
}
asm(".global sub_0808ADA4\n.thumb_set sub_0808ADA4, WaitForFlashWrite_Common\n");
