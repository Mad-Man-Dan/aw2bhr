#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0806BED8.
 * sub_0806BED8 @ 0x0806BED8, sub_0806BF40 @ 0x0806BF40
 */

#include "proc.h"
/* The string loader for the CreditsNameLine_Loop proc (src/decomp/c_0806BD84.c): starts
 * it, seeds the row it will spin in at +0x58, and converts a NUL-terminated
 * byte string into the +0x2a halfword table of sprite tiles, leaving the length
 * in +0x54 -- which is exactly the bound CreditsNameLine_Loop counts +0x5c up to. The
 * two functions agree on struct Unk6BE7CParent independently, so the layout is
 * a producer/consumer pair rather than one function's guess.
 *
 * `lsls #0x11; lsrs #0x10` is a net LEFT shift of one under a u16 truncation,
 * i.e. `(u16)((c - 0x41) * 2)` -- two tiles per letter. Per the wave-16 rule it
 * has to reach the compiler as a MULTIPLY, not `<< 1`: shorten_binary_op folds a
 * MULT into the narrow mode where a shift needs a third instruction. Assigning
 * to the `u16` local supplies the truncation, so no explicit cast is needed. */

struct Unk6BE7CParent
{
    /* 0x00 */ u8 filler_00[0x2a];
    /* 0x2a */ u16 unk2a[0x15];
    /* 0x54 */ int unk54;
    /* 0x58 */ int unk58;
    /* 0x5c */ int unk5c;
    /* 0x60 */ int unk60;
};
/* Draws one screen's worth of rows from the gUnknown_0858265C entry the proc's
 * +0x38 selects: six 8-byte records, each a tag plus a string. Tag 1 renders
 * the string, tags 3/4/5 blit one of three fixed blobs. If nothing at all was
 * drawn the proc jumps to label 1; otherwise the cursor is armed and +0x30 is
 * cleared.
 *
 * Four independent `if`s, NOT a switch and not an else-chain: the ROM re-loads
 * the tag before each test and every arm falls through into the next.
 *
 * THE ONE LEVER, and it is worth 8 bytes plus the whole register assignment:
 * the row cursor must be written `gBG0TilemapBuffer + (i * 0x60 + 0x29)`, with
 * the integer arithmetic PARENTHESISED, not `gBG0TilemapBuffer + i * 0x60 +
 * 0x29`. The latter parses as `((p + i * 0x60) + 0x29)`, which gives
 * strength_reduce a giv of `i * 0xc0` and leaves an `adds r0, #0x52` at each of
 * the two use sites; grouping the index folds the constant INTO the giv's init,
 * so the ROM's two separate cursors start at 0x52 and 0x92 and are each bumped
 * by 0xc0 at the loop bottom. That extra register pressure is also what spills
 * the proc to the stack -- with one giv it stays in sl and there is no
 * `sub sp, #4`, so the frame and the address arithmetic are ONE fact here, not
 * two. Same family as the documented `p[X + C]` vs `*(p + X + C)` rule. */

struct Unk6BF40Proc
{
    /* 0x00 */ u8 filler_00[0x30];
    /* 0x30 */ int unk30;
    /* 0x34 */ u8 filler_34[0x04];
    /* 0x38 */ int unk38;
};

void StartCreditsNameLine(int row, u8 *src, ProcPtr parent)
{
    struct Unk6BE7CParent *proc;
    int i;
    u16 t;

    i = 0;
    proc = Proc_Start(gUnknown_08581A34, parent);
    proc->unk58 = row * 24 + 8;
    proc->unk5c = 0;
    proc->unk60 = 0;

    while (src[0] != 0)
    {
        if (src[0] == 0x2d)
            t = 0x34;
        else if (src[0] == 0x20)
            t = 0x38;
        else if (src[0] == 0x7e)
            t = 0x3a;
        else
            t = (src[0] - 0x41) * 2;

        proc->unk2a[i] = t + 0xa0;
        src++;
        i++;
    }

    proc->unk54 = i;
}
asm(".global sub_0806BED8\n.thumb_set sub_0806BED8, StartCreditsNameLine\n");

void CreditsPage_Init(ProcPtr a1)
{
    struct Unk6BF40Proc *proc = a1;
    struct Unk0858265C *t;
    int i;
    int count;

    count = 0;
    t = gUnknown_0858265C[proc->unk38];

    for (i = 0; i <= 5; i++)
    {
        if (t->unk00[i].unk00 == 1)
        {
            DrawCreditsHeadingText(gBG0TilemapBuffer + (i * 0x60 + 0x29), t->unk00[i].unk04);
            count++;
        }

        if (t->unk00[i].unk00 == 3)
        {
            sub_080718F8(gBG0TilemapBuffer + (i * 0x60 + 0x49), gUnknown_081B9BC8, 0xe0);
            BG_EnableSyncBG0();
            count++;
        }

        if (t->unk00[i].unk00 == 4)
        {
            sub_080718F8(gBG0TilemapBuffer + (i * 0x60 + 0x49), gUnknown_081B9C3C, 0xe0);
            BG_EnableSyncBG0();
            count++;
        }

        if (t->unk00[i].unk00 == 5)
        {
            sub_080718F8(gBG0TilemapBuffer + (i * 0x60 + 0x29), gUnknown_081B9CC0, 0xe0);
            BG_EnableSyncBG0();
            count++;
        }
    }

    if (count == 0)
        Proc_Goto(proc, 1);
    else
    {
        SetCreditsWindow1Rect(0x48, 0, 0xf0, 0xa0);
        proc->unk30 = 0;
    }
}
asm(".global sub_0806BF40\n.thumb_set sub_0806BF40, CreditsPage_Init\n");
