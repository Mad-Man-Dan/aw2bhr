#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080135F4.
 * CopyToPaletteBufferNoSync @ 0x080135F4, ApplyPaletteExt @ 0x08013618, sub_08013640 @ 0x08013640, sub_08013664 @ 0x08013664
 */

#include "hardware.h"

/* MATCHED. ApplyPaletteExt without the trailing EnablePaletteSync call, and one of
 * its four byte-identical siblings. Read ApplyPaletteExt for the `u16` third
 * parameter, which is the whole derivation. */
void CopyToPaletteBufferNoSync(u16 *src, u32 b, u16 n)
{
    CpuCopy16(src, (u8 *)gPal + (b & 0xFFFE), n);
}
asm(".global sub_080135F4\n.thumb_set sub_080135F4, CopyToPaletteBufferNoSync\n");

/* MATCHED. The named palette-upload wrapper, and the last of a family of four
 * byte-identical siblings -- CopyToPaletteBufferNoSync, sub_08013640, sub_08013664 and this
 * one, all found by tools/overlap_screen.py's duplicate digest. The other three
 * are 36-40 bytes of the same two statements.
 *
 * ITS THIRD PARAMETER IS u16, AND THE HEADER SAID u32 FOR SEVENTEEN WAVES.
 * The entry `lsls r2,#0x10; lsrs r2,#0x10` is PROMOTE_MODE on a declared-narrow
 * parameter and therefore sits at the TOP of the function; declared `u32` the
 * same pair still appears -- as the conversion to CpuCopy16's u16 third
 * parameter -- but AFTER the gPal address arithmetic. One instruction pair, one
 * slot out of place, and the only difference between the two spellings. All 12
 * promoted callers pass constants, so the retype cost them nothing; they were
 * re-verified. See include/unknown-functions.h.
 *
 * `(u8 *)gPal + (b & 0xFFFE)` -- a BYTE offset onto a `u16 []`, which is what
 * the ROM's bare `adds r1, r1, r3` says. */
void ApplyPaletteExt(u16 *src, u32 b, u16 n)
{
    CpuCopy16(src, (u8 *)gPal + (b & 0xFFFE), n);
    EnablePaletteSync();
}

/* MATCHED. Byte-for-byte the same function as CopyToPaletteBufferNoSync -- identical
 * instruction stream and identical pool words. One C body, two
 * addresses; read that one for the derivation. */
void sub_08013640(u16 *src, u32 b, u16 n)
{
    CpuCopy16(src, (u8 *)gPal + (b & 0xFFFE), n);
}

/* MATCHED. Byte-for-byte the same function as ApplyPaletteExt -- identical
 * instruction stream and identical pool words. One C body, two
 * addresses; read that one for the derivation. */
void sub_08013664(u16 *src, u32 b, u16 n)
{
    CpuCopy16(src, (u8 *)gPal + (b & 0xFFFE), n);
    EnablePaletteSync();
}
