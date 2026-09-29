#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08017880.
 * sub_08017880 @ 0x08017880
 */

#include "hardware.h"

/*
 * CoScreenHBlankHandler -- the HBlank handler: reprogram display registers part-way
 * down the frame.
 *
 * Reads the current scanline out of REG_VCOUNT and acts on three of them:
 *   - 0x2f minus gUnknown_030030A8: load the BG0 control word, both horizontal
 *     scroll registers, and the blend control, blend weights and fade level,
 *     each from its RAM shadow.
 *   - 0x2c minus gUnknown_030030A8: load the display control word.
 *   - 0xe2, a fixed line: load a second display control word and BG0 control
 *     word, point both vertical scroll registers at gUnknown_030030A8, turn
 *     blending off and reset both horizontal scrolls to 0.
 * gUnknown_030030A8 is a vertical scroll amount, so the first two triggers
 * follow the scrolled region down the screen.
 *
 * Why the C looks odd: the two halves of REG_BLDALPHA are read through `vu16`
 * casts. Without them the compiler folds each address into its own load and
 * shares one register between the two reads. Swapping the two operands does not
 * help either; it reverses the order the two addresses are stored in.
 * gUnknown_030030A8 is already declared volatile, and these two are its
 * neighbours in the same set of HBlank shadow registers.
 */
void CoScreenHBlankHandler(void)
{
    int v;

    v = REG_VCOUNT & 0xff;

    if (v == 0x2f - (s16)gUnknown_030030A8)
    {
        *(vu32 *)(REG_BASE + REG_OFFSET_BG0CNT) = gUnknown_03002010;
        *(vu32 *)(REG_BASE + REG_OFFSET_BG0HOFS) = gUnknown_03003030;
        *(vu32 *)(REG_BASE + REG_OFFSET_BG1HOFS) = gUnknown_03002B3C;
        REG_BLDCNT = gUnknown_03002014;
        REG_BLDALPHA = *(vu16 *)&gUnknown_030030C0 + (*(vu16 *)&gUnknown_03001FEC << 8);
        REG_BLDY = gUnknown_03001FB4;
    }

    if (v == 0x2c - (s16)gUnknown_030030A8)
        REG_DISPCNT = gUnknown_03002F38;

    if (v == 0xe2)
    {
        REG_DISPCNT = gUnknown_03002EDC;
        *(vu32 *)(REG_BASE + REG_OFFSET_BG0CNT) = gUnknown_03002030;
        REG_BG0VOFS = gUnknown_030030A8;
        REG_BG1VOFS = gUnknown_030030A8;
        *(vu32 *)(REG_BASE + REG_OFFSET_BLDCNT) = 0;
        REG_BLDY = gUnknown_03001424;
        REG_BG0HOFS = 0;
        REG_BG1HOFS = 0;
    }
}
asm(".global sub_08017880\n.thumb_set sub_08017880, CoScreenHBlankHandler\n");
