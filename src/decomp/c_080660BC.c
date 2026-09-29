#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080660BC.
 * sub_080660BC @ 0x080660BC
 */

/* gUnknown_0816E140 is a -fforce-addr pool word holding &gUnknown_08580934,
 * not a global of its own -- that is where the `ldr r0,[r1]; ldr r0,[r0]`
 * pair at each read comes from.
 *
 * `step` has to be its own binding: the ROM computes `n - 1` (and `n + 1`)
 * once and keeps it live across the counting loop for the second modulo,
 * where `(x + (n - 1)) % n` written out twice reassociates to `(x - 1) + n`
 * and CSEs nothing.
 *
 * `index = idx` is also deliberate. A wide parameter used directly gets its
 * saved-register copy before the u16/u8 parameter conversions; a second local
 * pseudo emits the copy after both conversions, which is the ROM's prologue. */
void MatchSetupCycleTeam(u16 keys, int idx, u8 sfx)
{
    int index;
    int n;
    s16 step;
    int prev;
    int i;
    int same;
    u8 old;

    index = idx;
    n = gUnknown_08580934->unk08;
    old = gUnknown_08580934->unk11[index];

    if (keys & 0x40)
    {
        step = n - 1;
        gUnknown_08580934->unk11[index] =
            (gUnknown_08580934->unk11[index] + step) % n;

        for (i = 0, same = 0, prev = step; i < n; i++)
            if (gUnknown_08580934->unk11[0] == gUnknown_08580934->unk11[i])
                same++;

        if (same == n)
        {
            gUnknown_08580934->unk11[index] =
                (gUnknown_08580934->unk11[index] + prev) % n;

            if (1)
            {
            }
        }

        if (sfx)
        {
            if (old != gUnknown_08580934->unk11[index])
                PlayMusicOrSfx2(0x64);
            else
                PlayMusicOrSfx2(0x68);
        }
    }

    if (keys & 0x80)
    {
        step = n + 1;
        gUnknown_08580934->unk11[index] =
            (gUnknown_08580934->unk11[index] + step) % n;

        for (i = 0, same = 0, prev = step; i < n; i++)
            if (gUnknown_08580934->unk11[0] == gUnknown_08580934->unk11[i])
                same++;

        if (same == n)
            gUnknown_08580934->unk11[index] =
                (gUnknown_08580934->unk11[index] + prev) % n;

        if (sfx)
        {
            if (old != gUnknown_08580934->unk11[index])
                PlayMusicOrSfx2(0x64);
            else
                PlayMusicOrSfx2(0x68);
        }
    }
}
asm(".global sub_080660BC\n.thumb_set sub_080660BC, MatchSetupCycleTeam\n");
