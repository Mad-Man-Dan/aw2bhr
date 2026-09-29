#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803F80C.
 * sub_0803F80C @ 0x0803F80C, sub_0803F880 @ 0x0803F880
 */

/* MATCHED (wave 49, W49-C), second attempt. 116/116 bytes.
 *
 * PROMOTION NOTE: the only reloc difference is agbcc's -fforce-addr copy of
 * &gPlayers at 0x08091304 (the ROM word there contains 0x08499598),
 * which gen_lds.py names gUnknown_08091304. Do NOT declare it -- write the
 * honest `gPlayers[i]`, which is what emits the ROM's three-level
 * `ldr rA,=<word>; ldr rA,[rA]; ldr rA,[rA]` chain. This entry needs
 *   "rodata": ["0x08091304"]
 * in data/promoted.json, then tools/split_rodata.py + tools/gen_lds.py.
 *
 * `pal` MUST be bound to a local. Naming gUnknown_080D3EE4 inline at all three
 * sites is size-exact at 50.0% with one extra instruction: LICM hoists the
 * symbol into the loop preheader as a pseudo of its own and emits
 * `adds r7, r4, #0` to get it there, and the loop's address add comes out
 * `adds r0, r0, r7` where the ROM has `adds r0, r7, r0`. One pseudo used three
 * times removes both.
 *
 * The `(u16)` casts are real source: ApplyPaletteExt's second parameter is
 * declared u32, so nothing narrows for free, and `(u16)(x * 0x20)` is the
 * spelling shorten_binary_op folds into the ROM's `lsls #0x15; lsrs #0x10`
 * pair -- `((x * 0x20) & 0xffff)` or a shift would each need three.
 */

void LoadArmyObjPalettes(int a1)
{
    u16 (*pal)[16];
    int i;

    pal = gUnknown_080D3EE4;
    ApplyPaletteExt(pal[gPlayers[0].teamColor], (u16)((a1 + 0x10) * 0x20), 0x20);
    ApplyPaletteExt(pal[6], (u16)((a1 + 0x15) * 0x20), 0x20);
    for (i = 1; i < 5; i++)
        ApplyPaletteExt(pal[gPlayers[i].teamColor], (u16)((a1 + 0x10 + i) * 0x20), 0x20);
}
asm(".global sub_0803F80C\n.thumb_set sub_0803F80C, LoadArmyObjPalettes\n");

/* MATCHED (wave 49, W49-C), first attempt. 96/96 bytes, relocs match.
 *
 * `gPlayers[i + 1]` and not a new member: the ROM reads
 * `adds r0, r5, r0; adds r0, #0x56; ldrb r0, [r0]` with the giv r5 stepping
 * 0x3c, and 0x56 is 0x3c + 0x1a -- the NEXT record's unk1a, reached through one
 * induction variable. struct PlayerStruct is 0x3c bytes, so an 0x56 member would
 * not fit it in the first place.
 *
 * ApplyPaletteAndUploadNow's second parameter is `u16`, and that is what makes the whole
 * offset expression a single giv: gcc strength-reduces the narrowing's own
 * `lsls #0x10` into the induction variable, so r4 carries the value pre-shifted
 * left 16 (init `((a2 + 0x11) << 21) + 0xc0000`, step 0x200000) and the use is a
 * bare `lsrs r1, r4, #0x10`. Do not try to author that -- write the arithmetic
 * and let loop.c produce it.
 *
 * sub_08026340() sits IN the loop condition, re-evaluated every iteration.
 */

void sub_0803F880(int a1, int a2)
{
    u16 (*pal)[16];
    int i;

    pal = a1 ? gUnknown_080D3DE4 : gUnknown_080D3EE4;
    for (i = 0; i < sub_08026340(); i++)
        ApplyPaletteAndUploadNow(&pal[gPlayers[i + 1].teamColor][6], (a2 + 0x11 + i) * 0x20 + 0xc, 2);
}
