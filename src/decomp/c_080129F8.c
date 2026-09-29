#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080129F8.
 * sub_080129F8 @ 0x080129F8
 */

/*
 * RollPercentChance -- roll a percentage chance; 1 means it succeeded.
 *
 * `a` is a whole percentage. The next random number is reduced modulo 10000 and
 * compared with a * 100, which puts the two on the same scale. Both sides are
 * unsigned, which follows from GetNextRandomNumber returning u32.
 *
 * Why the C looks odd: these spellings do not change what the code does, but
 * the original compiler only produces identical output with them.
 *   - The two answers are returned from separate statements rather than by
 *     returning the comparison. Returning the comparison presets the false value
 *     and copies it into place, which the original does not do.
 *   - The return type is u8 because the only caller narrows the result to a byte
 *     before testing it, which an `int` return would not produce.
 */
u8 RollPercentChance(u16 a)
{
    if (GetNextRandomNumber() % 10000 < a * 100)
        return 1;

    return 0;
}
asm(".global sub_080129F8\n.thumb_set sub_080129F8, RollPercentChance\n");
