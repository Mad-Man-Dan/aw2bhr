#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080771C0.
 * sub_080771C0 @ 0x080771C0, sub_080771F0 @ 0x080771F0
 */

#include "hardware.h"

/* Half of an HBlank scroll ping-pong: push the offset to BG0 and BG2, set the
 * next compare line, and register SplitScroll_BottomHandler as the next handler -- which
 * registers this one back. The pair alternates every scanline pair.
 *
 * Two separate pool words for the two registers, where SplitScroll_BottomHandler derives the
 * second with `adds r0, #8` from the first: the difference is that here the
 * value being stored occupies a register of its own, so agbcc has one free for
 * a second address and does not need to reuse the first.
 *
 * The `(int)` cast is what SetVCountInterruptHandler's declared parameter forces. */
void SplitScroll_TopHandler(void)
{
    REG_BG0VOFS = gUnknown_0300064C;
    REG_BG2VOFS = gUnknown_0300064C;
    SetVCountCompareLine(0);
    SetVCountInterruptHandler((int)SplitScroll_BottomHandler);
}
asm(".global sub_080771C0\n.thumb_set sub_080771C0, SplitScroll_TopHandler\n");

/* The other half of SplitScroll_TopHandler's ping-pong: zero the two scroll registers,
 * arm the compare at line 0x80, and hand control back.
 *
 * `adds r0, #8` reaches REG_BG2VOFS from REG_BG0VOFS's address here because the
 * stored value is a constant and leaves a register free; the two are 8 bytes
 * apart, which fits an add-immediate. Not a different addressing decision from
 * SplitScroll_TopHandler, just a different register pressure. */
void SplitScroll_BottomHandler(void)
{
    REG_BG0VOFS = 0;
    REG_BG2VOFS = 0;
    SetVCountCompareLine(0x80);
    SetVCountInterruptHandler((int)SplitScroll_TopHandler);
}
asm(".global sub_080771F0\n.thumb_set sub_080771F0, SplitScroll_BottomHandler\n");
