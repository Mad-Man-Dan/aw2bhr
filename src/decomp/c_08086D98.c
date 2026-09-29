#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08086D98.
 * sub_08086D98 @ 0x08086D98, sub_08086DB4 @ 0x08086DB4, sub_08086DD4 @ 0x08086DD4, sub_08086DF4 @ 0x08086DF4, sub_08086E2C @ 0x08086E2C
 *
 * Named per src/aw2e-names.s (proc-table labels auto-generated from
 * AW2E.lua). The old sub_XXXXXXXX symbols are kept as linker aliases
 * below so every other unit keeps resolving them unchanged.
 */

/* One table lookup handed straight on: the s16 at +0x66 of the proc indexes a
 * byte array four bytes into gUnknown_02027F74.
 *
 * The +4 is a separate `adds` on the base and NOT folded into either the pool
 * word or the `ldrb` displacement, which is what pins the spelling down: a
 * member array or `(base + 4)[i]` folds it into the address constant, and
 * `*(i + (base + 4))` folds it into the load. Only a pointer local that is
 * bumped in a statement of its own keeps it, and the index has to be bound
 * first or the bump lands ahead of the `ldrsh`. */
struct Unk86D98Proc
{
    /* 00 */ u8 filler_00[0x66];
    /* 66 */ s16 unk66;
};
#include "hardware.h"

void MapSelectPreview_LoadMap(struct Unk86D98Proc *proc)
{
    u8 *p;
    int i;

    p = (u8 *)&gUnknown_02027F74;
    i = proc->unk66;
    p += 4;
    LoadMapData(p[i]);
}

/* `0x06004000 + chr_block * 0x4000` is the tile block BG1 is pointed at.
 *
 * The `ldr` on a two-byte union is not a mistake in the type model: agbcc's
 * get_best_mode widens a bitfield extract to the largest mode the OBJECT's
 * alignment allows, and gUnknown_03001FE8 is word-aligned, so a 2-bit field at
 * bit 2 comes out as `ldr` plus `lsls #0x1c; lsrs #0x1e`. Reading it as a mask
 * and a shift instead would be three instructions.
 *
 * MapSelectPreview_RunOverlayRoutine4 and MapSelectPreview_LoadPlaceholderPicture repeat the same address expression
 * with a different consumer. */
void MapSelectPreview_RunOverlayRoutine3(void)
{
    sub_0801B6EC((void *)(0x06004000 + gUnknown_03001FE8.bits.chr_block * 0x4000));
}

/* MapSelectPreview_RunOverlayRoutine3 with sub_0801B6FC instead of sub_0801B6EC, and
 * nothing else -- diffed against it rather than derived from it. See that
 * file for why the bitfield read is a word load. */
void MapSelectPreview_RunOverlayRoutine4(void)
{
    sub_0801B6FC((void *)(0x06004000 + gUnknown_03001FE8.bits.chr_block * 0x4000));
}

/* Two byte fields widened into two word globals, between a tilemap push and a
 * flush. The `ldrb` / `str` pair on each is the tell that the SOURCE is byte
 * and the DESTINATION is word -- neither side needs a cast.
 *
 * gUnknown_03003F68 is declared `void *`, so the two reads spell out the byte
 * view explicitly; 0x6200 is `movs #0xc4; lsls #7`. */
void MapSelectPreview_FillTilemap(void)
{
    FillMapPreviewTilemap(gBG1TilemapBuffer, 0x6200);
    gUnknown_03005918 = ((u8 *)gUnknown_03003F68)[0];
    gUnknown_030058F4 = ((u8 *)gUnknown_03003F68)[1];
    BG_EnableSyncBG1();
}

/* The third user of MapSelectPreview_RunOverlayRoutine3's address expression: unpack a
 * blob into the tile block BG1 currently points at. Decompress takes the
 * destination second, so the address arithmetic lands in r1 here rather than
 * r0. */
void MapSelectPreview_LoadPlaceholderPicture(void)
{
    Decompress(gUnknown_0823FD7C,
               (void *)(0x06004000 + gUnknown_03001FE8.bits.chr_block * 0x4000));
}
asm(".global sub_08086E2C\n.thumb_set sub_08086E2C, MapSelectPreview_LoadPlaceholderPicture\n");

asm(".global sub_08086D98\n.thumb_set sub_08086D98, MapSelectPreview_LoadMap\n"
    ".global sub_08086DB4\n.thumb_set sub_08086DB4, MapSelectPreview_RunOverlayRoutine3\n"
    ".global sub_08086DD4\n.thumb_set sub_08086DD4, MapSelectPreview_RunOverlayRoutine4\n"
    ".global sub_08086DF4\n.thumb_set sub_08086DF4, MapSelectPreview_FillTilemap\n");
