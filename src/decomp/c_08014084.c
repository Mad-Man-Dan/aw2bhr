#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08014084.
 * sub_08014084 @ 0x08014084
 */

/*
 * TextWriterStepCommand -- run one command of the text script at p->unk20 and say what
 * the caller should do next.
 *
 * p is the text writer's state. unk20 is the read position in the script, unk28
 * the tilemap being written, unk2c the tile attribute bits, unk2e the colour,
 * unk30/unk31 the left and top margins in tiles, unk32/unk33 the position now,
 * unk34 the VRAM tile index, unk3a the per-character delay and unk3c a routine
 * that flags the background for copying. dst is the tilemap entry the next
 * character goes to.
 *
 * The answer is 0 for "the script is finished", 1 for "this byte is an ordinary
 * character, draw it", 2 for "stop until the next frame" and 3 for "nothing to
 * draw, ask again straight away".
 *
 * Bytes 0x80 to 0x83 are consumed first, in a loop, each one setting the colour
 * in unk2e. The byte after them is the command:
 *
 *   0          end of script: if unk24 holds a continuation, switch to it and
 *              start again, otherwise answer 0.
 *   9          write the four tilemap entries of a 2 x 2 glyph, their tile
 *              numbers worked out from the next byte.
 *   10         draw a unit: the next byte picks which of gUnknown_030040D8's two
 *              units, and its hp is rounded up to tens for WriteUnitTileQuad.
 *   11         set the speed from the next byte. 0x80 switches the delay off
 *              (TextWriterDisableDelay); 0x81 to 0x89 store that byte plus 0x80 in unk3a
 *              and restart the counter; anything else is ignored.
 *   12         go back to the margins and hand the text to ClearTilemapRect23x4.
 *   13         next line: two rows down and back to the left margin.
 *   14         pause, unless the delay is off, in which case carry straight on.
 *   15         new page: blank the first entry, set the one at 0x21 from unk2c,
 *              go back to the margins and start the gUnknown_0848A398 helper.
 *   20, 22, 23 start the gUnknown_0848A3C4 helper at the current position, with
 *              its unk1e set to 1 when the next script byte is 0x17.
 *
 * Any other byte answers 1 and is drawn by the caller.
 *
 * Why the C looks odd: these spellings do not change what the code does, but
 * the original compiler only produces identical output with them.
 *   - The 0x80-to-0x83 scan is a `goto` loop and its lower bound is the local
 *     `lo`, not the literal. It also needs two separate comparisons with their
 *     own exits: `c >= 0x80 && c <= 0x83` is folded into one unsigned compare
 *     and the original compares twice. Case 11 tests its byte against the same
 *     `lo`, through its own signed local.
 *   - Case 20's false arm assigns through `zero`, a local pinned to r1, which is
 *     what leaves the hoisted zero where the original has it.
 *   - Case 10's two arms are near-identical and each carries its own copy of the
 *     ending. With the common `p->unk20 += 2; return 3;` moved below the `if`,
 *     the compiler merges the two arms into one block; the original has both in
 *     full. Case 10 also reaches unk32 through the separate `s2` pointer.
 *   - The cases are written in the order the original's blocks appear in -- 0,
 *     20/22/23, 13, 15, 12, 14, 11, 10, 9 -- and not in label order. The
 *     compiler lays case bodies out in source order, so reordering them moves
 *     every block in the function.
 */

/* sub_08013D64 is defined in src/decomp/c_08013D4C.c against a file-local
 * struct that describes this same record, with the same types at the same
 * offsets. It is declared here rather than in include/unknown-functions.h so
 * that the two files need not agree on one tag; merging the two views is a
 * separate job. */
u16 *sub_08013D64(struct Unk08014074 *);
struct Unk8014084Slot
{
    /* 0x00 */ u8 filler_00[0x18];
    /* 0x18 */ u16 *unk18;
    /* 0x1c */ u8 filler_1c[0x02];
    /* 0x1e */ s16 unk1e;
    /* 0x20 */ u8 filler_20[0x08];
    /* 0x28 */ u16 *unk28;
    /* 0x2c */ void (*unk2c)(void);
    /* 0x30 */ u16 unk30;
};

s16 TextWriterStepCommand(struct Unk08014074 *p, u16 *dst)
{
    u8 *s;
    u8 *s2;
    u16 n;
    int c;
    int c2;
    int lo;
    u16 q30;
    register int zero asm("r1");
    struct Unit *e;
    struct Unk8014084Slot *q;

entry_loop:
    c = *p->unk20;
    s = p->unk20;
    lo = 0x80;
    if (c > 0x83)
        goto entry_done;
    if (c < lo)
        goto entry_done;
    p->unk2e = c - 0x80;
    p->unk20 = s + 1;
    goto entry_loop;
entry_done:

    switch (s[0])
    {
    case 0:
        if (p->unk24 == 0)
            return 0;
        p->unk20 = (u8 *)p->unk24;
        p->unk24 = 0;
        return TextWriterStepCommand(p, dst);

    case 20:
    case 22:
    case 23:
        q = (struct Unk8014084Slot *)sub_080152EC(gUnknown_0848A3C4, 0);
        q->unk18 = dst + 1;
        q->unk28 = sub_08013D64(p);
        q->unk2c = p->unk3c;
        q30 = p->unk2c;
        zero = 0;
        q->unk30 = q30;
        if (p->unk20[0] == 0x17)
            q->unk1e = 1;
        else
            q->unk1e = zero;
        p->unk20 += 1;
        p->unk32 += 8;
        return 2;

    case 13:
        p->unk33 += 2;
        p->unk32 = p->unk30;
        p->unk34 += 2;
        p->unk40 = 0;
        p->unk20 += 1;
        return 3;

    case 15:
        dst[1] = 0;
        dst[0x21] = p->unk2c | 0x208;
        p->unk20 = s + 1;
        p->unk32 = p->unk30;
        p->unk33 = p->unk31;
        p->unk34 = p->unk36;
        p->unk40 = 0;
        q = (struct Unk8014084Slot *)sub_080152EC(gUnknown_0848A398, 0);
        q->unk18 = dst + 1;
        q->unk28 = sub_08013D64(p);
        q->unk2c = p->unk3c;
        sub_0803670C();
        return 2;

    case 12:
        p->unk20 = s + 1;
        p->unk32 = p->unk30;
        p->unk33 = p->unk31;
        p->unk34 = p->unk36;
        p->unk40 = 0;
        ClearTilemapRect23x4(sub_08013D64(p));
        p->unk3c();
        return 2;

    case 14:
        p->unk20 = s + 1;
        if (p->unk3a == 0)
            return 3;
        sub_080152EC(gUnknown_08489518, 0)->unk1e = 0xa;
        return 2;

    case 11:
        p->unk20 = s + 1;
        c2 = s[1];
        if (c2 == 0x80)
        {
            TextWriterDisableDelay(p);
        }
        else
        {
            if (c2 < lo)
                return 3;
            if (c2 > 0x89)
                return 3;
            p->unk3a = s[1] + 0x80;
            p->unk39 = 0;
        }
        p->unk20 += 1;
        return 3;

    case 10:
        if (s[1] == 0x80)
        {
            e = &gUnits[gUnknown_030040D8->unk07[0]];
            if (e->hp != 0)
                n = Div(e->hp - 1, 10) + 1;
            else
                n = 0;
            s2 = (u8 *)p + 0x32;
            WriteUnitTileQuad(p->unk28 + s2[0] + p->unk33 * 32, e->type,
                         gUnknown_03003F2C, e->unk07, 0, n, 0, 0);
            s2[0] += 2;
            p->unk20 += 2;
            return 3;
        }
        e = &gUnits[gUnknown_030040D8->unk07[1]];
        if (e->hp != 0)
            n = Div(e->hp - 1, 10) + 1;
        else
            n = 0;
        s2 = (u8 *)p + 0x32;
        WriteUnitTileQuad(p->unk28 + s2[0] + p->unk33 * 32, e->type,
                     gUnknown_03003F2C, e->unk07, 0, n, 0, 0);
        s2[0] += 2;
        p->unk20 += 2;
        return 3;

    case 9:
        dst[0] = s[1] * 4 - 0x604C;
        dst[1] = s[1] * 4 - 0x604B;
        dst[0x20] = s[1] * 4 - 0x604A;
        dst[0x21] = s[1] * 4 - 0x6049;
        p->unk32 += 2;
        p->unk20 += 2;
        return 3;
    }

    return 1;
}
asm(".global sub_08014084\n.thumb_set sub_08014084, TextWriterStepCommand\n");
