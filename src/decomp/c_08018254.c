#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08018254.
 * sub_08018254 @ 0x08018254
 */

#include "hardware.h"

/*
 * SetUpCoScreen -- set up the screen that shows a CO, from the current script
 * node.
 *
 * Runs on gUnknown_0200C528 slot `a`, whose .unk04 points at the node that
 * holds the parameters.
 *
 *   1. Rebuild the display shadows: two BG control words (character and
 *      tilemap block), and a DISPCNT word with mode 0, 1D OBJ mapping, BG0 and
 *      BG1 on, BG2 and BG3 off and HBlank interval free. Four more state
 *      globals go to 0.
 *   2. gUnknown_03002F90 takes the node's .unk0c, which then indexes
 *      gUnknown_0848A370 for sub_08012A54.
 *   3. Choose the CO, in gUnknown_03002F08.unk02, from the node's .unk08 read
 *      as a signed halfword: -1 leaves the current one alone. With bit 15 set,
 *      the low 15 bits are a player index and the CO is that player's own,
 *      stepped by 24 for each unit of the node's .unk0a (which counts from 1,
 *      so 0 or 1 means no step). Otherwise .unk08 is the CO index itself.
 *   4. Draw through gUnknown_03002F20 if a callback is installed, otherwise
 *      through TmApplyTsaClipped with a palette built from gUnknown_03002F08.unk00.
 *      RegisterDataMove uploads 0x200 bytes to BG VRAM at 0x0600E000, and
 *      DrawCoPortrait loads the chosen CO's graphics.
 *   5. Arm the wipe: the slot's counter and gUnknown_03001420 both start at
 *      0x2f, QueueVBlankCallback installs sub_08017EEC as the HBlank handler, and the
 *      slot's callback becomes CoScreenWipe_Step, which steps the wipe from there.
 *
 * Why the C looks odd: these spellings do not change what the code does, but
 * the original compiler only produces identical output with them.
 *   - The node's .unk08 goes into `u16 raw` and is sign-extended separately as
 *     `t = (s16)raw`. With one int local, or with the extension folded into the
 *     uses, the halfword is re-read or re-extended instead of being kept.
 *   - The store to gUnknown_03002F90 is spelled out rather than going through
 *     `node`, and `i = t & 0x7fff` is its own statement ahead of the .unk0a
 *     block. Both set the order the constants this function loads are stored
 *     in.
 *   - The bit-15 arm assigns and then adds in two statements, `null` holds NULL
 *     so the callback test compares against a local, and `v` is s16 rather than
 *     u16. All three were needed to make the output match; leave them.
 */
void SetUpCoScreen(s16 a)
{
    struct Unk0200C528Node *node;
    int t;
    int i;
    s16 v;
    u16 raw;
    void *null;

    gUnknown_03001FC8.raw = 0;
    gUnknown_030024E0.raw = 0;
    gUnknown_03001FC8.bits.chr_block = gUnknown_03002B6C.bits.chr_block;
    gUnknown_03001FC8.bits.tm_block = 0xd;
    gUnknown_030024E0.bits.chr_block = gUnknown_030030B4.bits.chr_block;
    gUnknown_030024E0.bits.tm_block = 0x1c;
    gUnknown_03002004.raw = 0;
    gUnknown_03002004.bits.mode = 0;
    gUnknown_03002004.bits.obj_mapping = 1;
    gUnknown_03002004.bits.bg0_enable = 1;
    gUnknown_03002004.bits.bg1_enable = 1;
    gUnknown_03002004.bits.bg2_enable = 0;
    gUnknown_03002004.bits.bg3_enable = 0;
    gUnknown_03002004.bits.hblank_interval_free = 1;
    gUnknown_03002B48 = 0;
    gUnknown_03001FC0 = 0;
    gUnknown_030030D8 = 0;
    gUnknown_0300303C = 0;
    gUnknown_03002F90 = gUnknown_0200C528[a].unk04->unk0c;
    node = gUnknown_0200C528[a].unk04;
    raw = node->unk08;
    t = (s16)raw;
    i = t & 0x7fff;
    v = node->unk0a;
    if ((s16)node->unk0a == 0)
        v = 1;
    v--;
    if (t != -1)
    {
        if (((s16)raw & 0x8000) != 0)
        {
            gUnknown_03002F08.unk02 = gPlayers[i].co;
            gUnknown_03002F08.unk02 += v * 24;
        }
        else
            gUnknown_03002F08.unk02 = raw;
    }
    sub_08012A54(gUnknown_0848A370[(s16)gUnknown_03002F90]);
    null = NULL;
    if (gUnknown_03002F20 != null)
        gUnknown_03002F20();
    else
        TmApplyTsaClipped(gUnknown_0849958C, 0, 0, gUnknown_080D445C,
                     (u16)((gUnknown_03002F08.unk00 << 12) | 0x360));
    RegisterDataMove(gUnknown_0849958C, (void *)0x0600E000, 0x200);
    sub_080179AC();
    DrawCoPortrait(gUnknown_03002F08.unk02);
    gUnknown_0200C528[a].unk0e = 0x2f;
    gUnknown_03001420 = 0x2f;
    QueueVBlankCallback((void *)sub_08017EEC);
    gUnknown_0200C528[a].unk08 = (void *)CoScreenWipe_Step;
}
asm(".global sub_08018254\n.thumb_set sub_08018254, SetUpCoScreen\n");
