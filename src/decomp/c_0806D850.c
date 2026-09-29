#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0806D850.
 * sub_0806D850 @ 0x0806D850
 */

/* gUnknown_08580934 is reached through agbcc's own -fforce-addr word (the ROM
 * word at 0x0816E1A4, which holds 0x08580934), hence the `ldr r5,=..;
 * ldr r4,[r5]; ldr r0,[r4]` triple. Naming the global is what produces it.
 *
 * The delay loop really is empty -- gcc 2.9 has no dead-loop elimination, so
 * it survives -O2. But it has to be spelled with the guard and the counter as
 * TWO locals and an explicit `if`: `adds r2, r0, #0` is a copy the allocator
 * did not coalesce, and every one-variable spelling of the same countdown
 * (`while (n != 0) n--`, `for (i = 0; i < unk08; i++)`, `for (i = unk08;
 * i != 0; i--)`, with a fresh name or a reused one) coalesces it away and
 * comes out one instruction short. Probed, all five.
 *
 * The `v` round-trip in the last loop is a caller-side cast, not a claim about
 * unk32's type. Written `gUnknown_08580934->unk32 |= 0xff`, shorten_binary_op
 * narrows the OR to QImode, `fold` sees an all-ones mask and collapses the
 * whole statement to `strb -1` -- two instructions where the ROM has four.
 * Going through an int keeps the read-modify-write; reading through `(u8 *)`
 * is what makes it `ldrb` rather than the `ldrsb` the s8 member would give.
 * Probed: `|=` folds, `x = x | 0xff` folds, and a volatile cast drops the OR
 * instead. */
void RulesScreenInitState(void)
{
    int i;
    int v;
    int n;

    gUnknown_08580934->unk2c = gUnknown_0202F2C8;
    FillMatchSettingsRecord(gUnknown_08580934);
    RulesScreenPackRuleIndices();

    n = gUnknown_08580934->unk08;
    if (n != 0)
    {
        i = n;
        do
            i--;
        while (i != 0);
    }

    gUnknown_08580934->unk30 = 0;
    gUnknown_08580934->unk33 = 0;

    for (i = 0; i < 8; i++)
    {
        v = *(u8 *)&gUnknown_08580934->unk32;
        v |= 0xff;
        gUnknown_08580934->unk32 = v;
    }

    gUnknown_08580934->unk2d = 0;
}
asm(".global sub_0806D850\n.thumb_set sub_0806D850, RulesScreenInitState\n");
