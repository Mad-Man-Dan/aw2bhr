#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08032950.
 * sub_08032950 @ 0x08032950, sub_08032A00 @ 0x08032A00
 */

#include "proc.h"
#include "hardware.h"

/* LinkPlayerCursor_HandleInput's wrap-around twin: same left/right slot stepper, but the index
 * wraps with `& 3` instead of an explicit bounds pair, it records the direction
 * in unk10, and a changed slot restarts the gUnknown_0849B688 proc.
 *
 * Two read-outs earned this one:
 *
 * - The guard is `Proc_Exists(...) == TRUE`, NOT `Proc_Find(...) != NULL`. The
 *   `== TRUE` forces the macro's `?:` through do_store_flag as a VALUE, giving
 *   the branchless `rsbs r1,r0,#0; orrs r1,r0` whose sign combine then tests
 *   with `cmp #0; blt`. Every direct spelling (`!= 0`, `== 0`, `!`, and bare
 *   `Proc_Exists(...)`) folds to a plain `cmp r0,#0; bne` and is 4 bytes short
 *   -- measured, all five in one probe.
 *
 * - `i` is u16, not s16. `ldrh` fills it and `(i +/- 1) & 3` is provably 0..3,
 *   so nothing re-extends it; s16 costs `lsls #0x10; asrs #0x10` on every step.
 *
 * The final test compares the two MEMBERS (`unk06 != unk04`), not member
 * against `i` -- that is why both sides get `lsls #0x10` where LinkPlayerCursor_HandleInput,
 * comparing against its own local, shifts only one. */

void LinkMapPick_HandleInput(void)
{
    u16 i;

    i = gUnknown_0849B060->unk04;

    if (Proc_Exists(gUnknown_0849B688) == TRUE)
        return;

    if (gpKeySt->pressed & DPAD_LEFT)
    {
        do
            i = (i - 1) & 3;
        while (gUnknown_0300449C[i] == 0);

        gUnknown_0849B060->unk10 = 1;
    }

    if (gpKeySt->pressed & DPAD_RIGHT)
    {
        do
            i = (i + 1) & 3;
        while (gUnknown_0300449C[i] == 0);

        gUnknown_0849B060->unk10 = 2;
    }

    gUnknown_0849B060->unk06 = gUnknown_0849B060->unk04;
    gUnknown_0849B060->unk04 = i;

    if (gUnknown_0849B060->unk06 != gUnknown_0849B060->unk04)
    {
        Proc_EndEach(gUnknown_0849B688);
        Proc_EndEach(gUnknown_0849B670);
        Proc_Start(gUnknown_0849B688, 0);
        PlayMusicOrSfx2(0x67);
    }
}
asm(".global sub_08032950\n.thumb_set sub_08032950, LinkMapPick_HandleInput\n");

/* The local `u8 *v[3]` initialiser makes agbcc emit FOUR .rodata objects: the
 * three one-character string literals and then the 12-byte template it block-
 * copies to the stack (`ldm r0!,{r2,r3,r4}; stm r1!,{r2,r3,r4}`). The layout is
 * strings-first, so the template lands at .rodata + 0xc -- exactly the ROM's
 * layout: the literals are at 0x08090D3C/40/44 (words 0x31, 0x32, 0x33, i.e.
 * "1", "2", "3") and the template at 0x08090D48.
 *
 * PROMOTION NEEDS the whole six-word run, starting at 0x08090D3C and not at
 * the 0x08090D48 the .text pool word names:
 *     "rodata": ["0x08090D3C", "0x08090D40", "0x08090D44",
 *                "0x08090D48", "0x08090D4C", "0x08090D50"]
 *
 * The stack block-copy is what proves the array is a NON-STATIC LOCAL with an
 * initialiser: a file-scope `static const char *const[]` would be indexed in
 * place, with no template and no `sub sp` for it.
 *
 * This function sat at 99.6% across several waves for a HARNESS reason, not a
 * decompilation one: the template's three words relocate against the local
 * `.rodata` section symbol (they point at the literals beside them), and
 * trymatch.py's pool_word_equivalent() resolved every relocation through
 * sym_addr(), which cannot name a section. It now anchors a `.rodata`
 * self-reference on the section base it has already computed. The C below is
 * unchanged from the draft that was reported as a near-miss.
 */
void LinkMapPick_Draw(void)
{
    u8 *v[3] = { (u8 *)"1", (u8 *)"2", (u8 *)"3" };

    if (gUnknown_0849B060->unk0c > 1)
    {
        DrawOamObject(0x45, 0x58 - gUnknown_0849B0C0[((u32)gGameClock >> 3) & 3], 0x4c, 0, 0);
        DrawOamObject(0x46, gUnknown_0849B0C0[((u32)gGameClock >> 3) & 3] + 0xd8, 0x4c, 0, 0);
    }

    if (gUnknown_0849B060->unk10 != 0)
    {
        FillTilemapRect(gBG0TilemapBuffer, 0x11, 2, 0xc, 2, 0);
        PutTextScriptImmediate(0x11, 2, gBG0TilemapBuffer, v[gUnknown_0849B060->unk04], 0x8000, 0);
        PutTextScriptImmediate(0x13, 2, gBG0TilemapBuffer,
            &gUnknown_02027C2C[gUnknown_0849B060->unk04 * 19], 0x8000, 0);
        BG_EnableSyncBG0();
    }

    gUnknown_03002F18 = 0xFFD4;
    gUnknown_03002B34 = gUnknown_0849B060->unk0e - 0x60;
}
asm(".global sub_08032A00\n.thumb_set sub_08032A00, LinkMapPick_Draw\n");
