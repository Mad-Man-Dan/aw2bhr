#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08039A5C.
 * sub_08039A5C @ 0x08039A5C, sub_08039ACC @ 0x08039ACC
 */

#include "proc.h"
/* Starts one of two banner scripts and seeds it, after advancing the global
 * two-frame animation toggle. The sound it plays is picked from the current
 * map's terrain record, the caller's variant `d`, and the toggle.
 *
 * gUnknown_030043F8 is named honestly: the ROM reaches it through a
 * -fforce-addr .rodata word (0x08090F6C, whose ROM content is 0x030043F8)
 * because it is read on both sides of the wrap's control-flow merge. */
struct Unk39ACCProc
{
    /* 0x00 */ PROC_HEADER;
    /* 0x29 */ u8 unk29;
    /* 0x2a */ u8 unk2a;
    /* 0x2b */ STRUCT_PAD(0x2b, 0x2c);
    /* 0x2c */ u16 unk2c;
    /* 0x2e */ u16 unk2e;
    /* 0x30 */ u16 unk30;
};

/* The general form behind CoPowerPanelNoOp: two tile blobs into VRAM at 0x06010000
 * at a caller-chosen tile index, plus one palette, resetting the frame toggle.
 *
 * Parameters 3 and 4 ARE u16 -- both are narrowed at ENTRY, before the
 * gUnknown_030043F8 store, which is PROMOTE_MODE and not a cast at a use. The
 * header declared them `int`; see the note there for why retyping is safe.
 *
 * `lsls #0x15; lsrs #0x10` on the palette slot is a NET LEFT SHIFT OF FIVE with
 * an 11-bit truncation, i.e. `(u16)((b + 0x10) * 0x20)` -- a multiply, not a
 * mask plus a shift, and the `+ 0x10` happens BEFORE the scaling. Both 0x3FF
 * and 0x06010000 are CSEd into callee-saved registers across the two
 * Decompress calls, which is what forces r8 to be pushed. */
void LoadSparkleGraphics(void *src, void *pal, u16 a, u16 b)
{
    gUnknown_030043F8 = 0;

    Decompress(src, (void *)((a & 0x3ff) * 0x20 + 0x06010000));
    ApplyPaletteExt(pal, (u16)((b + 0x10) * 0x20), 0x20);
    Decompress(gUnknown_080A534C, (void *)(((a + 0xc0) & 0x3ff) * 0x20 + 0x06010000));
}
asm(".global sub_08039A5C\n.thumb_set sub_08039A5C, LoadSparkleGraphics\n");

void StartSparkleAtTile(u16 a, u16 b, u16 c, int d)
{
    struct Unk39ACCProc *proc;

    gUnknown_030043F8++;

    if (gUnknown_030043F8 > 1)
        gUnknown_030043F8 = 0;

    if (d != 0)
        proc = Proc_Start(gUnknown_0849D874, PROC_TREE_3);
    else
        proc = Proc_Start(gUnknown_0849D84C, PROC_TREE_3);

    PlayMusicOrSfx2(gUnknown_084A0090[gPlayers[gUnknown_030033EC].co]
                     .power[d].sound[gUnknown_030043F8]);

    proc->unk29 = a;
    proc->unk2a = b;
    proc->unk2c = c;
    proc->unk2e = 0;
    proc->unk30 = 0;
}
asm(".global sub_08039ACC\n.thumb_set sub_08039ACC, StartSparkleAtTile\n");
