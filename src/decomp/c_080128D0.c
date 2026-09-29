#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080128D0.
 * sub_080128D0 @ 0x080128D0
 */

#include "hardware.h"
void FlushDisplayRegisters(void);
void sub_0801258C(void);

/* FlushLCDControl @ 0x080128D0, 140 bytes, THUMB.
 * Signature below is DECLARED in include/unknown-functions.h -- it is authoritative.
 * The compiler sees that header too, so a definition that
 * disagrees will not compile.
 *
 * Pushes the two IWRAM WinCnt shadows and the eight window-bound bytes
 * straight to hardware (0x04000040-0x0400004B: WIN0H/WIN1H/WIN0V/WIN1V
 * as byte pairs, then WININ/WINOUT as halfwords) after two setup calls.
 */
void FlushLCDControl(void)
{
    FlushDisplayRegisters();
    sub_0801258C();
    *(vu8 *)(REG_BASE + REG_OFFSET_WIN0H)     = gUnknown_03002B4C;
    *(vu8 *)(REG_BASE + REG_OFFSET_WIN0H + 1) = gUnknown_03002B40;
    *(vu8 *)(REG_BASE + REG_OFFSET_WIN0V)     = gUnknown_03002B44;
    *(vu8 *)(REG_BASE + REG_OFFSET_WIN0V + 1) = gUnknown_03002EFC;
    *(vu8 *)(REG_BASE + REG_OFFSET_WIN1H)     = gUnknown_030024E4;
    *(vu8 *)(REG_BASE + REG_OFFSET_WIN1H + 1) = gUnknown_03002B68;
    *(vu8 *)(REG_BASE + REG_OFFSET_WIN1V)     = gUnknown_030020B8;
    *(vu8 *)(REG_BASE + REG_OFFSET_WIN1V + 1) = gUnknown_03002B30;
    REG_WININ  = gUnknown_030030A4.raw;
    REG_WINOUT = gUnknown_030030DC.raw;
}
asm(".global sub_080128D0\n.thumb_set sub_080128D0, FlushLCDControl\n");
