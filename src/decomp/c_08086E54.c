#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08086E54.
 * sub_08086E54 @ 0x08086E54, sub_08086EB0 @ 0x08086EB0
 */

#include "proc.h"
/* Swaps between two proc scripts according to the byte at index `a1` of the
 * candidate list four bytes into gUnknown_02027F74.
 *
 * The address arithmetic is src/decomp/c_08086D98.c's idiom and for the same
 * reason: `(u8 *)&gUnknown_02027F74` bumped by 4 in a statement of its own
 * keeps the `adds r0, #4` at runtime, where gUnknown_02027F78 (the ROM's own
 * name for that address) or a member array would fold it into the pool word.
 *
 * `(u8)IsCampaignMapUnlocked(...)` is the caller's own narrowing -- src/decomp is
 * promoted with an `int` return, and include/unknown-functions.h records why
 * the `lsls #0x18` belongs here. No argument setup appears because the byte is
 * already in r0 from the `ldrb` that the `<= 0xb3` test just read.
 *
 * The `else if` re-tests the byte rather than reusing the first comparison, and
 * the ROM's opening `bhi` jumps straight into that arm -- jump threading, since
 * a byte above 0xb3 fails the `&&` and satisfies the `else if` unconditionally.
 * That duplicated test is the shape; a plain `else` emits one branch fewer. */

struct Unk86EB0Proc
{
    /* 0x00 */ u8 filler_00[0x66];
    /* 0x66 */ s16 unk66;
};

/* Fills the first ten entries of each of the first eight 32-halfword rows of
 * *gBG1TilemapBuffer with consecutive tile ids from 0x6200 up, then sets the
 * two window-size globals sub_08086DF4 reads out of gUnknown_03003F68 and
 * flushes.
 *
 * Two readings the assembly forces:
 *   - The stored value is `(n + 0x200) | 0x6000`, not `n + 0x6200`. Both
 *     constants are LICM-hoisted into their own registers (r7 = 0x200 via
 *     `movs #0x80; lsls #2`, r6 = 0x6000 via `movs #0xc0; lsls #7`) and the ROM
 *     spends an `adds` AND an `orrs`; the single-constant spelling folds them
 *     into one `adds` against one hoisted word. 0x6000 is the palette bits.
 *   - `n` is a running counter of its own, not `i * 10 + j`. It is bumped once
 *     per inner trip with no reset and no per-row init in the inner preheader,
 *     which is what the giv for `i * 10 + j` would have to place there.
 * The pointer global is re-read inside the inner loop (`mov r0, ip; ldr r1,
 * [r0]`) because it is named directly and is not const -- the same tell
 * src/decomp/c_080858C0.c documents. */

void MapSelectPreview_FillPlaceholderTilemap(void)
{
    int i;
    int j;
    int n;

    n = 0;
    for (i = 0; i <= 7; i++)
        for (j = 0; j <= 9; j++)
            gBG1TilemapBuffer[i * 32 + j] = (n++ + 0x200) | 0x6000;

    gUnknown_03005918 = 0x14;
    gUnknown_030058F4 = 0x10;
    BG_EnableSyncBG1();
}
asm(".global sub_08086E54\n.thumb_set sub_08086E54, MapSelectPreview_FillPlaceholderTilemap\n");

void StartMapSelectPreview(int a1)
{
    struct Unk86EB0Proc *proc;
    u8 *p;

    ClearBg1Tilemap();
    p = (u8 *)&gUnknown_02027F74;
    p += 4;

    if (p[a1] <= 0xb3 && (u8)IsCampaignMapUnlocked(p[a1]))
    {
        Proc_EndEach(gUnknown_08616D1C);
        Proc_EndEach(gUnknown_08616D6C);
        proc = Proc_Start(gUnknown_08616D1C, PROC_TREE_3);
        proc->unk66 = a1;
    }
    else if (p[a1] > 0xb3)
    {
        if (Proc_Find(gUnknown_08616D1C))
            Proc_EndEach(gUnknown_08616D1C);
        if (Proc_Find(gUnknown_08616D6C))
            Proc_EndEach(gUnknown_08616D6C);
        Proc_Start(gUnknown_08616D6C, PROC_TREE_3);
    }
    BG_EnableSyncBG1();
}
asm(".global sub_08086EB0\n.thumb_set sub_08086EB0, StartMapSelectPreview\n");
