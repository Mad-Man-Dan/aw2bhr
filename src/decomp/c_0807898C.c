#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0807898C.
 * sub_0807898C @ 0x0807898C
 */

#include "hardware.h"

/* The screen-mode reset for this subsystem: blank DISPCNT down to "mode 0, all
 * four BGs and OBJ on, no windows", give each BG shadow its priority / char
 * base / tilemap base, zero the eight scroll shadows, then run the four
 * palette-and-tile inits and turn 1-D OBJ mapping back on.
 *
 * Every mask in the ROM falls out of hardware.h's bitfield structs, so nothing
 * here needed deriving -- but three of them read as raw constants and are worth
 * naming, because each is a field assignment and not a hand-written mask:
 *
 *  - `movs #0x21; rsbs` is -0x21 = ~0x20, i.e. `win0_enable = 0`, and the
 *    `subs r1, #0x20` that follows RE-USES it to build ~0x40 for
 *    `win1_enable = 0`. c_080688E4.c records the same pair for
 *    `.bits.wrap = 0`.
 *  - `movs #0x7f; ands` is not a 7-bit mask; it is `objwin_enable = 0` on
 *    DISPCNT byte 1 and `color_depth = 0` on each BgCnt byte 0. agbcc prefers
 *    `& 0x7f` to `& ~0x80` because 0x7f is a `movs` immediate and 0xFFFFFF7F is
 *    not. Likewise `& 0x3f` is `size = 0`.
 *  - `movs #0xd; rsbs` is ~0x0C = `chr_block = 0`.
 *
 * Statement ORDER is read off the interleave, and it is not per-BG-block. The
 * four `priority` writes come first as a group -- all four `ldrb`s and their
 * `& ~3` happen before any other field -- and only then does each shadow get
 * chr_block, tm_block, color_depth, size. Within one shadow the two bytes are
 * separate read-modify-writes that agbcc merges per byte, so the emitted
 * interleave (chr_block, tm_block, color_depth, size) is literal source order:
 * byte 0 keeps chr_block before color_depth and byte 1 keeps tm_block before
 * size, and tm_block sits between them.
 *
 * MATCHED first draft.
 *
 * WAVE 53, W53-D: the parameter was added here, not discovered here -- this
 * body never reads r0 and is byte-identical either way. See the note on the
 * declaration in include/unknown-functions.h: sub_0808A6CC reloads its spilled
 * proc into r0 immediately before the `bl`, which only a callee with an
 * argument can explain. Re-verified byte-for-byte after the change. */
void SetupMenuScreenBgs(ProcPtr proc)
{
    gDispIo.disp_ct.mode = 0;
    SetDispEnable(1, 1, 1, 1, 1);
    SetWinEnable(0, 0, 0);

    gUnknown_03002B6C.bits.priority = 0;
    gUnknown_03001FE8.bits.priority = 2;
    gUnknown_030030B4.bits.priority = 1;
    gUnknown_0300251C.bits.priority = 3;

    gUnknown_03002B6C.bits.chr_block = 0;
    gUnknown_03002B6C.bits.tm_block = 14;
    gUnknown_03002B6C.bits.color_depth = 0;
    gUnknown_03002B6C.bits.size = 0;

    gUnknown_03001FE8.bits.chr_block = 0;
    gUnknown_03001FE8.bits.tm_block = 30;
    gUnknown_03001FE8.bits.color_depth = 0;
    gUnknown_03001FE8.bits.size = 0;

    gUnknown_030030B4.bits.chr_block = 2;
    gUnknown_030030B4.bits.tm_block = 15;
    gUnknown_030030B4.bits.color_depth = 0;
    gUnknown_030030B4.bits.size = 0;

    gUnknown_0300251C.bits.chr_block = 2;
    gUnknown_0300251C.bits.tm_block = 31;
    gUnknown_0300251C.bits.color_depth = 0;
    gUnknown_0300251C.bits.size = 0;

    gUnknown_03001FF8 = 0;
    gUnknown_03001418 = 0;
    gUnknown_03002B34 = 0;
    gUnknown_03002F18 = 0;
    gUnknown_030030A0 = 0;
    gUnknown_03001400 = 0;
    gUnknown_0300200C = 0;
    gUnknown_03002000 = 0;

    ClearBg0Tilemap();
    ClearBg1Tilemap();
    ClearBg2Tilemap();
    ClearBg3TilemapBuffer();

    gDispIo.disp_ct.obj_mapping = 1;
}
asm(".global sub_0807898C\n.thumb_set sub_0807898C, SetupMenuScreenBgs\n");
