#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0806279C.
 * sub_0806279C @ 0x0806279C
 */

/* AiClearInfluenceGrid @ 0x0806279C, 88 bytes, THUMB.  MATCHED (wave 48, W48-D).
 * Clears the whole [10][12] grid of 0x2c-byte records at gUnknown_0202DAD8.
 *
 * The ten words at offset 0 are ONE array, not two of five: the k-loop stores
 * unk00[k + 5] and unk00[k] off a SINGLE induction variable, which is what
 * `str r4,[r0,#0x14]; stm r0!,{r4}` is. Spelling the record with two five-word
 * members folds the 0x14 into the address constant (a hoisted
 * `gUnknown_0202DAD8 + 0x14`), needs a second iv and a stack slot, and does
 * not match.
 *
 * No binding local for `gUnknown_0202DAD8[i][j]`: the ROM recomputes the row
 * address into a scratch for the two `strh`s after the k-loop instead of
 * parking it in a callee-saved register, which is the tell for the address
 * expression being written out at each statement. */
void AiClearInfluenceGrid(void)
{
    int i;
    int j;
    int k;

    for (i = 0; i <= 9; i++)
        for (j = 0; j <= 0xb; j++) {
            for (k = 0; k <= 4; k++) {
                gUnknown_0202DAD8[i][j].unk00[k + 5] = 0;
                gUnknown_0202DAD8[i][j].unk00[k] = 0;
            }
            gUnknown_0202DAD8[i][j].unk28 = 0;
            gUnknown_0202DAD8[i][j].unk2a = 0;
        }
}
asm(".global sub_0806279C\n.thumb_set sub_0806279C, AiClearInfluenceGrid\n");
