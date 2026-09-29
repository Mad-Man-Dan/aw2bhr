#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08078440.
 * sub_08078440 @ 0x08078440, sub_08078454 @ 0x08078454
 */

#include "proc.h"

/* Family F006, exemplar src/decomp/c_08014BB4.c. `adds r1, r0, #0` moves the
 * incoming argument to the SECOND parameter and the pool word is the first, so
 * this is `f(ProcPtr parent)` and not a one-argument call to a two-argument
 * function.
 *
 * gUnknown_08615984 is a 0xFF-terminated u8 id list ({8, 9, 0xa, 0xff} in the
 * ROM); see the comment on it in include/unknown-globals.h for why it and its
 * neighbour four bytes later are two symbols rather than one array, and for
 * what that reading does and does not prove. StartWorldMapMarkerReveal's first parameter was
 * retyped from `s32` to `const u8 *` on the same evidence. */

void WorldMapReturn_RevealMissions08To0A(ProcPtr parent)
{
    StartWorldMapMarkerReveal(gUnknown_08615984, parent);
}
asm(".global sub_08078440\n.thumb_set sub_08078440, WorldMapReturn_RevealMissions08To0A\n");

/* Family F006, the twin of WorldMapReturn_RevealMissions08To0A one list along. gUnknown_08615988 is
 * {0x10, 0x11, 0x12, 0xff} in the ROM. */

void WorldMapReturn_RevealMissions10To12(ProcPtr parent)
{
    StartWorldMapMarkerReveal(gUnknown_08615988, parent);
}
asm(".global sub_08078454\n.thumb_set sub_08078454, WorldMapReturn_RevealMissions10To12\n");
