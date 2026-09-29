#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801258C.
 * sub_0801258C @ 0x0801258C
 */

#include "hardware.h"

/* The display-state snapshot: every shadow that FlushDisplayRegisters pushes to the
 * hardware, copied into a parallel set of symbols. 824 bytes, 43 statements,
 * no branches and no calls -- the emitted store order IS the source order,
 * because none of the 88 addresses is reachable by arithmetic from any other
 * (they are all distinct relocations, so unlike FlushDisplayRegisters there is no
 * absolute-address chain to read the ordering off, and none is needed).
 *
 * Six of the copies pack two shadows into one word as `lo | (hi << 16)`, and
 * the four window bounds go out together as a single 64-bit OR. That last one
 * is what makes the function want three high registers: the four operands are
 * volatile, so combine cannot fold each `ldrh` into the DImode lowpart and
 * every term leaves a `adds rN, r4, #0` copy behind. See the notes on
 * gUnknown_030020B0 and gUnknown_03001FF8 in unknown-globals.h -- the volatile
 * qualifiers here are load-bearing and were each read off the codegen.
 */

void sub_0801258C(void)
{
    gUnknown_03002F38 = *(u16 *)&gDispIo.disp_ct;
    gUnknown_03001FCC = gUnknown_030020B4.raw;
    gUnknown_03003030 = gUnknown_03001FF8 | (gUnknown_03001418 << 16);
    gUnknown_03002B3C = gUnknown_03002B34 | (gUnknown_03002F18 << 16);
    gUnknown_03003090 = gUnknown_030030A0;
    gUnknown_03001414 = gUnknown_03001400;
    gUnknown_03002F10 = gUnknown_0300200C;
    gUnknown_03002518 = gUnknown_03002000;
    gUnknown_03002F14 = gUnknown_030030C4;
    gUnknown_03003044 = gUnknown_030030A4.raw;
    gUnknown_03002008 = gUnknown_030030DC.raw;
    gUnknown_03001FD8 = gUnknown_03002B40;
    gUnknown_03002B70 = gUnknown_03002EFC;
    gUnknown_03002EE8 = gUnknown_03002B4C;
    gUnknown_03002F34 = gUnknown_03002B44;
    gUnknown_030030AC = gUnknown_03002B68;
    gUnknown_03002EEC = gUnknown_03002B30;
    gUnknown_030020A4 = gUnknown_030024E4;
    gUnknown_03003094 = gUnknown_030020B8;
    gUnknown_03002010 = gUnknown_03002B6C.raw | (gUnknown_03001FE8.raw << 16);
    gUnknown_03002EF4 = gUnknown_03002B6C.raw;
    gUnknown_03002B58 = gUnknown_03001FE8.raw;
    gUnknown_03003098 = gUnknown_030030B4.raw;
    gUnknown_030030E4 = gUnknown_0300251C.raw;
    gUnknown_03002014 = gUnknown_030030E0.raw;
    gUnknown_030030C0 = gUnknown_03002020;
    gUnknown_03001FEC = gUnknown_03002B28;
    gUnknown_03001FB4 = gUnknown_03001FFC;
    gUnknown_0300140C = gUnknown_03002B48;
    gUnknown_03001FC4 = gUnknown_03001FC0;
    gUnknown_03003038 = gUnknown_030030D8;
    gUnknown_03001424 = gUnknown_0300303C;
    gUnknown_03002030 = gUnknown_03001FC8.raw | (gUnknown_030024E0.raw << 16);
    gUnknown_03002F40 = gUnknown_03001FC8.raw;
    gUnknown_03002018 = gUnknown_03002034;
    gUnknown_03002EDC = gUnknown_03002004.raw;
    gUnknown_030030B8 = (u64)gUnknown_030020B0 | ((u64)gUnknown_0300309C << 16)
                      | ((u64)gUnknown_03002B60 << 32) | ((u64)gUnknown_03002028 << 48);
    gUnknown_03002024 = gUnknown_030020B0 | (gUnknown_0300309C << 16);
    gUnknown_03001FB0 = gUnknown_03002B60 | (gUnknown_03002028 << 16);
    gUnknown_03002B50 = gUnknown_03001FB8;
    gUnknown_03002F28 = gUnknown_030024C4;
    gUnknown_0300201C = gUnknown_030024CC;
    gUnknown_03002B64 = gUnknown_03001410;
}
