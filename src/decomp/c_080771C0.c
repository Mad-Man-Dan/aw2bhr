#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080771C0.
 * sub_080771C0 @ 0x080771C0, sub_080771F0 @ 0x080771F0
 */

#include "hardware.h"

/* Half of an HBlank scroll ping-pong: push the offset to BG0 and BG2, set the
 * next compare line, and register sub_080771F0 as the next handler -- which
 * registers this one back. The pair alternates every scanline pair.
 *
 * Two separate pool words for the two registers, where sub_080771F0 derives the
 * second with `adds r0, #8` from the first: the difference is that here the
 * value being stored occupies a register of its own, so agbcc has one free for
 * a second address and does not need to reuse the first.
 *
 * The `(int)` cast is what SetVCountInterruptHandler's declared parameter forces. */
void sub_080771C0(void)
{
    REG_BG0VOFS = gUnknown_0300064C;
    REG_BG2VOFS = gUnknown_0300064C;
    SetVCountCompareLine(0);
    SetVCountInterruptHandler((int)sub_080771F0);
}

/* The other half of sub_080771C0's ping-pong: zero the two scroll registers,
 * arm the compare at line 0x80, and hand control back.
 *
 * `adds r0, #8` reaches REG_BG2VOFS from REG_BG0VOFS's address here because the
 * stored value is a constant and leaves a register free; the two are 8 bytes
 * apart, which fits an add-immediate. Not a different addressing decision from
 * sub_080771C0, just a different register pressure. */
void sub_080771F0(void)
{
    REG_BG0VOFS = 0;
    REG_BG2VOFS = 0;
    SetVCountCompareLine(0x80);
    SetVCountInterruptHandler((int)sub_080771C0);
}
