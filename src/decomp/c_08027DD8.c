#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08027DD8.
 * DayStartScreen_Loop @ 0x08027DD8
 */

#include "hardware.h"
#include "proc.h"
/* Not a proc: +0x1e and +0x20 are halfwords, which would sit inside
 * proc_next / proc_prev, and +0x08 is written as a word. */
struct Unk27DD8
{
    /* 0x00 */ u8 filler_00[0x08];
    /* 0x08 */ int unk08;
    /* 0x0c */ u8 filler_0c[0x12];
    /* 0x1e */ s16 unk1e;
    /* 0x20 */ s16 unk20;
};
/* The two identical `StartDayStartGlyph` bodies are NOT a transcription slip: the
 * ROM reaches one copy from case 25 with an explicit `b`, over the top of
 * case 15's test, which a single shared body could not produce. gcc's
 * cross-jumping merges the two copies back into one, so the duplicate costs
 * nothing and is the only shape that emits that branch.
 */

/* The two identical `StartDayStartGlyph` bodies are NOT a transcription slip: the
 * ROM reaches one copy from case 25 with an explicit `b`, over the top of
 * case 15's test, which a single shared body could not produce. gcc's
 * cross-jumping merges the two copies back into one, so the duplicate costs
 * nothing and is the only shape that emits that branch.
 */
void DayStartScreen_Loop(struct Unk27DD8 *e)
{
    if (gpKeySt->pressed & (A_BUTTON | B_BUTTON | START_BUTTON))
    {
        Proc_EndEach(ProcScr_DayStart);
        sub_0801537C(gUnknown_08499D90);
        gUnknown_03002F1C = 1;
        e->unk08 = 0;
        return;
    }

    switch (e->unk1e)
    {
    case 20:
        e->unk20 += 0x10;
        if (gUnknown_03004080 <= 0x63)
            break;
        /* fall through */
    case 25:
        if (gUnknown_03004080 <= 9)
            break;
        StartDayStartGlyph(e->unk20, 0x50, (s16)(e->unk1e / 5) * 16, (s16)(e->unk1e / 5),
                     PROC_TREE_3);
        e->unk20 += 0x18;
        break;

    case 15:
        if (GetDayWordGlyphCount() != (void *)4)
            break;
        /* fall through */
    case 0:
    case 5:
    case 10:
    case 30:
        StartDayStartGlyph(e->unk20, 0x50, (s16)(e->unk1e / 5) * 16, (s16)(e->unk1e / 5),
                     PROC_TREE_3);
        e->unk20 += 0x18;
        break;

    case 75:
        e->unk08 = 0;
        break;
    }

    e->unk1e++;
}

asm(".global sub_08027DD8\n.thumb_set sub_08027DD8, DayStartScreen_Loop\n");
