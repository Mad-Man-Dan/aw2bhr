#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08043CA0.
 * sub_08043CA0 @ 0x08043CA0, sub_08043D00 @ 0x08043D00
 */

/* MATCHED (wave 37, W37-Q4), one attempt.
 * Needs "rodata": ["0x08091378", "0x0809137C"] in data/promoted.json.
 *
 * Filters the 19-entry CO display order at gUnknown_084A077C down to the
 * unlocked ones (IsCoUnlocked), packs them into gUnknown_020288A0, writes the
 * 0xff terminator and returns the count.
 *
 * Both globals are reached through the file's `-fforce-addr` .rodata block at
 * 0x08091350-0x0809138C, i.e. `ldr rN,[pool] (= 0x08091378); ldr rM,[rN]`.
 * That is one plain subscript in the source, not two dereferences -- the
 * honest spelling emits the .rodata word itself and trymatch reports
 * `name different symbols that resolve to the same address`, which is a MATCH.
 * The words were identified by dereferencing 0x08091378/0x0809137C in
 * baserom.gba (-> 0x084A077C and 0x020288A0).
 *
 * `u8 n` and `u8 i` are both hard: every increment is `adds #1; lsls #0x18;
 * lsrs #0x18`, and the loop bound reads `cmp r5,#0x12; bls` (unsigned <= 18).
 */
u8 BuildUnlockedCoList(void)
{
    u8 n;
    u8 i;

    n = 0;
    for (i = 0; i < 19; i++)
    {
        if ((u8)IsCoUnlocked(gUnknown_084A077C[i]))
            gUnknown_020288A0[n++] = gUnknown_084A077C[i];
    }
    gUnknown_020288A0[n] = 0xff;
    return n;
}
asm(".global sub_08043CA0\n.thumb_set sub_08043CA0, BuildUnlockedCoList\n");

/* MATCHED (wave 37, W37-Q4), one attempt.
 * Needs "rodata": ["0x08091380"] in data/promoted.json (the force-addr word
 * holding &gUnknown_020288A0 -- same block as BuildUnlockedCoList's two).
 *
 * Takes BuildUnlockedCoList's unlocked-CO list and DOUBLES it in place until it is
 * longer than six, so the carousel always has enough entries to scroll.
 *
 * `(u8)(n - 1) <= 5` is gcc's range test for `n != 0 && n <= 6` on an
 * unsigned char: the ROM computes `(n << 24) - 0x01000000` and reuses the
 * `n << 24` it already had for the narrowing, then `lsrs #0x18; cmp #5; bhi`.
 *
 * `n += k` and not `n *= 2`: the ROM adds the inner loop's counter register
 * (`adds r0, r3, r2`), which is only equal to n because the guard keeps n
 * non-zero.  A doubling would have been `lsls r0, r3, #1`.
 */
void RepeatUnlockedCoList(void)
{
    u8 n;
    u8 k;

    n = BuildUnlockedCoList();
    while ((u8)(n - 1) <= 5)
    {
        for (k = 0; k < n; k++)
            gUnknown_020288A0[n + k] = gUnknown_020288A0[k];
        n += k;
    }
    gUnknown_020288A0[n] = 0xff;
}
asm(".global sub_08043D00\n.thumb_set sub_08043D00, RepeatUnlockedCoList\n");
