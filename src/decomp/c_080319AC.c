#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080319AC.
 * sub_080319AC @ 0x080319AC
 */

/* MATCHED, wave 51 (W51-B). The body is byte-for-byte the wave-45 draft that
 * sat at 97.50% for six waves -- not one character of C changed. The three
 * differing bytes were a HEADER regression:
 *
 *     ROM     ldrb r2, [r0, #9] / ldrb r0, [r1] / cmp r2, r0
 *     draft   ldrb r0, [r0, #9] / ldrb r1, [r1] / cmp r0, r1
 *
 * gUnknown_03003F1C is VOLATILE. A volatile QImode read cannot be folded into
 * the address register it is loaded through, so it takes a third register and
 * the ROM's operand order falls out. Declared plain `u8` the read reuses the
 * address register and costs exactly those 3 bytes.
 *
 * The five promoted STOREs (LinkStartKeySyncMode, LinkMapListExchange_Start, LinkDecodeSharedMapId,
 * LinkStartMapTransfer, LinkMapPick_WaitForPress) were all re-verified with the qualifier on and
 * are unchanged -- `x = 0;` on a volatile u8 whose value is discarded is a
 * bare `strb` either way. That is why five waves of writers never decided it,
 * and the header comment beside gUnknown_030044C4 says as much. This
 * function's read is the only discriminating use in the tree.
 *
 * c_08031948.c used to claim this global was fixed as NOT volatile, reasoning
 * across from the dead-`ldrb`-before-`strb` tell on its neighbours. That
 * inference is wrong: the tell identifies a volatile lvalue whose VALUE is
 * consumed (a chained assignment), and a store whose value is discarded shows
 * nothing either way. That comment has been corrected.
 *
 * The 0x08090D0C pool word is agbcc's own -fforce-addr word holding
 * &gUnknown_0849B018; naming the global directly is correct and the build
 * places the word. trymatch reports `.rodata` against the ROM's invented
 * gUnknown_08090D0C, the documented false mismatch. Promotion needs
 *   "rodata": ["0x08090D0C"]
 * in this function's data/promoted.json entry. */

int LinkWaitMapListBlocks(void)
{
    int r;

    gUnknown_0849B060->unk02 = 2;

    if ((gGameClock & 0xf) == 0)
    {
        gUnknown_02025760.unk00 = 0xab;
        gUnknown_02025760.unk01 = gUnknown_0849B018->unk06;
        gUnknown_02025760.unk02 = 0;

        sub_0802F588(&gUnknown_02025760, 0x84);
    }

    if (gUnknown_0849B018->unk09 != gUnknown_03003F1C)
    {
        r = 1;
    }
    else
    {
        gUnknown_0300410C = gUnknown_030040CC;
        gUnknown_030044C4 = 0;

        r = 0;
    }

    return r;
}
asm(".global sub_080319AC\n.thumb_set sub_080319AC, LinkWaitMapListBlocks\n");
