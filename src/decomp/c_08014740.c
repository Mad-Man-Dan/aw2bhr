#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08014740.
 * sub_08014740 @ 0x08014740, sub_080147B4 @ 0x080147B4, sub_08014824 @ 0x08014824
 */

/*
 * StartTextBox -- start a text box as its own process and return it.
 *
 * sub_080152EC starts the gUnknown_08489530 process and InitTextWriter then fills
 * that process's record, so the record and the process are one object. The
 * pointer is returned. StartTextBoxViaRecord and sub_080146D4 do the same job the other
 * way round: they fill gUnknown_0200C020 first and start the script afterwards.
 *
 * a and b are the left and top margins in tiles, c the tilemap to write into, d
 * the entry of gTextTable to read the script from, e the tile attribute bits and
 * f the VRAM tile index. Only a and b are known to be signed; the other narrow
 * parameters have nothing here that exercises their sign.
 */
struct Unk03001470 *StartTextBox(s16 a, s16 b, u16 *c, u16 d, u16 e, u16 f)
{
    struct Unk03001470 *p;

    gUnknown_03002514 = 0;
    p = sub_080152EC(gUnknown_08489530, 0);
    InitTextWriter((struct Unk08014074 *)p, a, b, c, d, e, f);

    return p;
}
asm(".global sub_08014740\n.thumb_set sub_08014740, StartTextBox\n");

/*
 * InitTextWriter -- fill a text writer's record from its parameters.
 *
 * The script is gTextTable[a5]; a4 is the tilemap to write into, a6 the tile
 * attribute bits, a7 the VRAM tile index, and a2/a3 the left and top margins,
 * which are also the starting position. The delay starts at two frames with its
 * counter at -1, and unk3c is set to the routine that flags BG0 for copying.
 *
 * Why the C looks odd: this spelling does not change what the code does, but
 * the original compiler only produces identical output with it.
 *   - `s->unk34 = s->unk36 = a7;` is one chained assignment, so one narrowed
 *     value feeds both stores and unk36 is written first.
 */
void InitTextWriter(struct Unk08014074 *s, s16 a2, s16 a3, u16 *a4, u16 a5, u16 a6, u16 a7)
{
    s->unk20 = gTextTable[a5];
    s->unk24 = 0;
    s->unk28 = a4;
    s->unk2c = a6;
    s->unk2e = 0;
    s->unk34 = s->unk36 = a7;
    s->unk38 = 0;
    s->unk30 = a2;
    s->unk31 = a3;
    s->unk32 = a2;
    s->unk33 = a3;
    s->unk39 = -1;
    s->unk3a = 2;
    s->unk3c = BG_EnableSyncBG0;
    s->unk40 = 0;
}
asm(".global sub_080147B4\n.thumb_set sub_080147B4, InitTextWriter\n");

/*
 * sub_08014824 -- is any of the three text-box processes still running?
 *
 * FindSlotScript answers -1 for a process that is not there. The three asked about
 * are gUnknown_08489530, gUnknown_08489548 and gUnknown_08489568, and the result
 * is 1 when at least one of them answered anything else.
 *
 * Why the C looks odd: this spelling does not change what the code does, but
 * the original compiler only produces identical output with it.
 *   - `n` is initialised from the first comparison itself and the other two are
 *     `if`s that increment it. The original works the first one out without a
 *     branch and branches on the other two, which is what this gives.
 */
int sub_08014824(void)
{
    int n;

    n = FindSlotScript((s32)gUnknown_08489530) != -1;
    if (FindSlotScript((s32)gUnknown_08489548) != -1)
        n++;
    if (FindSlotScript((s32)gUnknown_08489568) != -1)
        n++;

    return n > 0;
}
