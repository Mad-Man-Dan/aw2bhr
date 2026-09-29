#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08049D88.
 * sub_08049D88 @ 0x08049D88, sub_08049DE4 @ 0x08049DE4
 */

/* A frame-scheduled cutscene step: on frame 10 start four copies of the same
 * script blob, on frame 50 stop one slot, and tick.
 *
 * A `switch` and NOT an `if`/`else if` chain: the ROM tests both constants back
 * to back (`cmp #0xa; beq A; cmp #0x32; beq B; b C`) with the bodies out of
 * line, which is case dispatch. An `if`/`else if` interleaves the second
 * compare after the first body. The counter is read `ldrsh` for the dispatch
 * and `ldrh` for the increment, which is one `s16` field, not two. */
struct Unk08049DE4Proc
{
    /* 0x00 */ u8 filler_00[0x1e];
    /* 0x1e */ s16 unk1e;
};

/* Screen setup for the 0x08049 sequence: reset the slot counter, decompress
 * sub_0801F494's blob into gUnknown_0200FC50 and register its four quadrants,
 * then load the palette.
 *
 * `Decompress(sub_0801F494(), ...)` is nested and not two statements: r0 is
 * already the returned pointer at the `bl`, and the destination's pool word is
 * materialised between the calls, which is the argument-setup order for a
 * nested call rather than a spill. */
void DefeatFlow_BannerInit(void)
{
    gUnknown_02028E3D = 0;

    Decompress(sub_0801F494(), gUnknown_0200FC50);

    CopyDayGlyphTiles(gUnknown_0200FC50, 0, 0);
    CopyDayGlyphTiles(gUnknown_0200FC50, 1, 1);
    CopyDayGlyphTiles(gUnknown_0200FC50, 2, 2);
    CopyDayGlyphTiles(gUnknown_0200FC50, 3, 3);

    ApplyPaletteExt(gUnknown_081268B8, 0x260, 0x20);
}
asm(".global sub_08049D88\n.thumb_set sub_08049D88, DefeatFlow_BannerInit\n");

void DefeatFlow_BannerSpawner_Step(struct Unk08049DE4Proc *proc)
{
    switch (proc->unk1e)
    {
    case 0xa:
        sub_080152EC(gUnknown_084C3244, 0);
        sub_080152EC(gUnknown_084C3244, 0);
        sub_080152EC(gUnknown_084C3244, 0);
        sub_080152EC(gUnknown_084C3244, 0);
        break;

    case 0x32:
        ClearSlotScriptCallback(gUnknown_03001FBC);
        break;
    }

    proc->unk1e++;
}
asm(".global sub_08049DE4\n.thumb_set sub_08049DE4, DefeatFlow_BannerSpawner_Step\n");
