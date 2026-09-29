#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08012260.
 * sub_08012260 @ 0x08012260
 */

#include "hardware.h"

/* The key-repeat tick. Reads REG_KEYINPUT (active low, hence `0x3FF ^`),
 * publishes the raw state, derives the newly-pressed set, and counts down the
 * repeat timer while the state is unchanged.
 *
 * TWO readings that are not free choices.
 *
 * The `ldr`/`str` pairs between three globals declared `u16` are 32-BIT copies
 * spelled `*(u32 *)&dst = *(u32 *)&src` -- the same scalar-cast idiom
 * src/decomp/c_0806EB5C.c uses on gUnknown_030030E0. A u16-to-u16 assignment
 * would be `ldrh`/`strh`.
 *
 * The `adds r1, r4, #0` in front of `cmp r1, #0` is the whole reason the guard
 * reads gUnknown_03002F04 rather than the local: cse substitutes the value just
 * stored, and the substitution lands in a fresh pseudo, which is that copy. The
 * final store uses the LOCAL and so needs no such copy.
 *
 * gUnknown_0808E524 is not a global -- it is agbcc's `-fforce-addr` word for
 * &gUnknown_03000040 (baserom.gba holds 0x03000040 there), which is why the
 * pool word relocates against .rodata here.
 *
 * PROMOTION NEEDS THE POOL WORD PLACED: this function's data/promoted.json
 * entry needs  "rodata": ["0x0808E524"]  and then tools/split_rodata.py +
 * tools/gen_lds.py before the build. */
void UpdateKeyRepeatState(void)
{
    u16 keys;

    keys = 0x3FF ^ REG_KEYINPUT;

    gUnknown_03002F04 = keys;
    gUnknown_030024C8 = keys & ~gUnknown_03000040;
    *(u32 *)&gUnknown_030030B0 = *(u32 *)&gUnknown_030024C8;

    if (gUnknown_03002F04 != 0 && gUnknown_03002F04 == gUnknown_03000040)
    {
        gUnknown_03000042--;

        if (gUnknown_03000042 == 0)
        {
            *(u32 *)&gUnknown_030030B0 = *(u32 *)&gUnknown_03002F04;
            gUnknown_03000042 = gUnknown_03000046;
        }
    }
    else
    {
        gUnknown_03000042 = gUnknown_03000044;
    }

    gUnknown_03000040 = keys;
}
asm(".global sub_08012260\n.thumb_set sub_08012260, UpdateKeyRepeatState\n");
