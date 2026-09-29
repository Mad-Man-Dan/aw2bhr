#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0806574C.
 * sub_0806574C @ 0x0806574C
 */

/* Session setup: seeds the gUnknown_08580934 header, rebuilds the per-slot
 * tables and clears the cursor state, then hands off to FillMatchSettingsRecord and
 * MatchSetupPackRuleIndices.
 *
 * gUnknown_0816E108 is agbcc's own -fforce-addr word holding &gUnknown_08580934
 * (the same block of ROM words as gUnknown_0816E10C for MatchSetupPackRuleIndices and
 * gUnknown_0816E110 for MatchSetupUnpackRuleIndices). Naming the global honestly is what
 * produces the ROM's three-level `ldr r3,=<word>; ldr r2,[r3]; ldr r0,[r2]`
 * and the copy into r6 that survives the loop -- do not spell the word.
 *
 * `unk09[i] = i == 0 ? 1 : 2` then an unconditional `if (unk24 == 1)` override:
 * the second store reuses the loaded unk24 (known to be 1) as its source
 * operand and the +0x24 address register minus 0x1b as its destination, which
 * is why only one `ldrb` appears for two uses of the field. */
void MatchSetupInitState(void)
{
    int i;

    gUnknown_08580934->unk26 = 0;
    gUnknown_08580934->unk2c = gUnknown_0202F200;
    gUnknown_08580934->unk24 = gPlaySt.savingEnabled;
    gUnknown_08580934->unk25 = gUnknown_08580934->unk24 ? SioGetSelfId() : -1;
    FillMatchSettingsRecord(gUnknown_08580934);
    MatchSetupPackRuleIndices();

    for (i = 0; i < gUnknown_08580934->unk08; i++)
    {
        gUnknown_08580934->unk20[i] = gUnknown_08580934->unk18[gUnknown_08580934->unk1c[i]];
        gUnknown_08580934->unk09[i] = i == 0 ? 1 : 2;
        if (gUnknown_08580934->unk24 == 1)
            gUnknown_08580934->unk09[i] = 1;
    }

    gUnknown_08580934->unk30 = 0;
    gUnknown_08580934->unk33 = 0;
    gUnknown_08580934->unk32 = 0;
    gUnknown_08580934->unk2d = 0;
    gUnknown_08580934->unk2e = 0;
}
asm(".global sub_0806574C\n.thumb_set sub_0806574C, MatchSetupInitState\n");
