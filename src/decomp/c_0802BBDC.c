#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802BBDC.
 * sub_0802BBDC @ 0x0802BBDC
 */

/* The two DrawSpriteNumberFont2 arguments carry an explicit (u16) cast and the two
 * PutOamHi ones do not -- the ROM truncates the first pair with
 * `lsls #0x10; lsrs #0x10` (fused into the `0x8c << 0xf` add on the first,
 * which reuses the a1 << 16 still live in r0) and leaves the second pair
 * alone. DrawSpriteNumberFont2's first parameter is declared int; the cast reproduces
 * the ROM without retyping a prototype other callers share. Dropping it also
 * costs the register assignment: a1 moves to r5 and gUnknown_0849A3B8 to r4,
 * the reverse of the ROM. */
void DrawDayAndFundsBar(s16 a1)
{
    DrawSpriteNumberFont2((u16)(a1 + 0x46), 5, gPlayers[gUnknown_030033EC].funds);
    DrawSpriteNumberFont2((u16)(a1 + 0xe), 5, gUnknown_03004080);
    PutOamHi(a1 + 0x16, 5, gUnknown_0849A3B8, 0x700A);
    PutOamHi(a1 + 0x4e, 6, gUnknown_0849A3B8, 0x700B);
    gUnknown_030033F0 = 1;
}
asm(".global sub_0802BBDC\n.thumb_set sub_0802BBDC, DrawDayAndFundsBar\n");
