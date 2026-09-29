#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802D33C.
 * sub_0802D33C @ 0x0802D33C
 */

/* FindSlotRunningScript finds the gUnknown_03001470 slot running gUnknown_0848A42C and
 * this reads that slot's unk48 (a u16 at a displacement past `ldrh`'s 5-bit
 * range, hence `adds r0,#0x48`). The field was narrowed out of
 * struct Unk03001470's filler_3a in wave 29.
 *
 * The branch polarity is load-bearing and was measured: `if (x <= 0xf)
 * return 1; return 0x10;` compiles to `bls` to the return-1 block with the
 * 0x10 falling through, i.e. the mirror image of the ROM. Testing the OTHER
 * way round -- `> 0xf` first -- puts `movs r0,#1` in the fall-through and the
 * pool between the two arms, exactly as the ROM has it. Two spellings of one
 * comparison, and only one of them is this function. */

int GetOptionHelpWindowX(void)
{
    if (FindSlotRunningScript(gUnknown_0848A42C)->unk48 > 0xF)
        return 0x10;

    return 1;
}
asm(".global sub_0802D33C\n.thumb_set sub_0802D33C, GetOptionHelpWindowX\n");
