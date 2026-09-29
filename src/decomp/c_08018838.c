#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08018838.
 * sub_08018838 @ 0x08018838
 */

/* A gUnknown_0200C528 list-script handler: sets the palette-fade selector to
 * 0xf and repaints the current army's palette, then steps the script pointer.
 *
 * The second argument is `gUnknown_03002F08.unk00`, not a literal -- CSE
 * rematerialises the 0xf it stored one statement earlier rather than keeping a
 * register alive, so `movs r1, #0xf` is what the global read compiles to here.
 * EventOp_ApplyFramePaletteForArmy, the same call with no preceding store, reloads it `ldrb`. */
bool8 EventOp_SetFramePaletteSlot15(s16 a)
{
    gUnknown_03002F08.unk00 = 0xf;
    ApplyWindowFramePalette(gPlayers[gUnknown_030033EC].teamColor - 1, gUnknown_03002F08.unk00);
    gUnknown_0200C528[a].unk04++;
    return TRUE;
}
asm(".global sub_08018838\n.thumb_set sub_08018838, EventOp_SetFramePaletteSlot15\n");
