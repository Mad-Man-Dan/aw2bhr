#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08064A44.
 * sub_08064A44 @ 0x08064A44
 */

/* MATCHED (292/292 bytes, one try_match, zero compile probes). Re-verified
 * after this wave's header edits.
 *
 * PROMOTION NOTE -- this match needs its .rodata pool words PLACED. Add to this
 * function's data/promoted.json entry:
 *     "rodata": ["0x0816E0EC", "0x0816E0F0"]
 * then re-run tools/split_rodata.py and tools/gen_lds.py before building.
 * Those two words are agbcc's own -fforce-addr copies of gUnknown_085809B4 and
 * gUnknown_085809D0; naming the targets, as below, is the wave-18 spelling.
 *
 * Derived from the matched exemplar src/decomp/c_0806D268.c, which answered the
 * seven-iteration walk over unk54[] verbatim. Three things differed: the blob
 * (gUnknown_0858096C), the two stepped members (unk28/unk38 here where the
 * exemplar has unk2c/unk2a -- and the exemplar's unk2c is a WORD, so this is a
 * different pair, not a widening), and the two added tail blocks.
 *
 * The guarded decrement reloads gUnknown_08580934 rather than reusing the `g`
 * the final statement binds, and the ROM says so: `ldr r1, [r1, #0x6c]` clobbers
 * the pointer's register inside the `if`, so it cannot be live at the merge. */

void MatchSetupSpawnRuleOptions(void)
{
    struct Unk08580934 *g;
    struct Unk08580934_Obj *o;
    u8 *p;
    int i;
    int a;
    int b;

    a = 7;
    b = 0x54;

    for (i = 0; i <= 6; i++)
    {
        o = (struct Unk08580934_Obj *)sub_080152EC(gUnknown_0858096C, 3);
        gUnknown_08580934->unk54[i] = o;
        o->unk46 = 0;
        o->unk28 = a;
        o->unk38 = b;
        a += 0x20;
        b -= 8;
        o->unk24 = 6 - i;
        o->unk3e = 0x6000 + gUnknown_0816E0D0[i];
        o->unk40 = 0x6000 + gUnknown_0816E0DE[i];
        o->unk4b = gUnknown_085809A4[i];
        p = &gUnknown_08580934->unk84;
        o->unk48 = p[i];
        o->unk4c = gUnknown_085809B4[i];
        o->unk47 = gUnknown_085809D0[i];
    }

    if (gUnknown_08580934->unk24 == 1)
        gUnknown_08580934->unk54[6]->unk4b--;

    g = gUnknown_08580934;
    g->unk54[4]->unk4b = g->unk16 - g->unk15 + 2;

    StartHeaderBanner(gUnknown_085802AC, gUnknown_0200FC50, 0x230, 0xF, 1, 1, 3);
    LinkRestartKeySync();
}
asm(".global sub_08064A44\n.thumb_set sub_08064A44, MatchSetupSpawnRuleOptions\n");
