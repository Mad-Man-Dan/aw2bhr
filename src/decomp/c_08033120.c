#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08033120.
 * sub_08033120 @ 0x08033120, sub_08033150 @ 0x08033150, sub_08033174 @ 0x08033174
 */

/* Six teardown calls in ROM order, then the two BG scroll shadows cleared.
 * Both globals are `volatile u16` but the stores are bare scalar assignments,
 * which is byte-identical with or without the qualifier -- the volatile on
 * these two was settled elsewhere (see include/unknown-globals.h). */
void LinkScreenClearBackgrounds(void)
{
    ClearBg0Tilemap();
    ClearBg1Tilemap();
    ClearBg2Tilemap();
    BG_EnableSyncBG0();
    BG_EnableSyncBG1();
    BG_EnableSyncBG2();

    gUnknown_03002F18 = 0;
    gUnknown_03002B34 = 0;
}
asm(".global sub_08033120\n.thumb_set sub_08033120, LinkScreenClearBackgrounds\n");

/* Seven bare `bl`s, every result discarded. Note the order: EndLinkMapPick runs
 * BEFORE EndLinkPlayerCursor, which is not the address order the callee list is
 * printed in. */
void LinkScreenEndAll(void)
{
    EndLinkLobbySlots();
    sub_08011B18();
    EndLinkTransferPercent();
    sub_08031E6C();
    EndLinkMapPick();
    EndLinkPlayerCursor();
    UnlockMainMenu();
}
asm(".global sub_08033150\n.thumb_set sub_08033150, LinkScreenEndAll\n");

/* 0xFFD0 is a POSITIVE literal and not -48: SetBgScrollShadow's second parameter is
 * `u16`, and the ROM materialises the value with a pool `ldr`. A -48 would have
 * been `movs r1,#0x30; rsbs r1,r1,#0`, two instructions and no pool word.
 *
 * The pool `ldr` landing before both `movs` is argument setup grouped by
 * operand class, not argument order. */
void sub_08033174(void)
{
    SetBgScrollShadow(0, 0xFFD0, 0);
    SetBgScrollShadow(3, 0, 0);
}
