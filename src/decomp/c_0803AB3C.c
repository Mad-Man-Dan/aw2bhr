#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803AB3C.
 * sub_0803AB3C @ 0x0803AB3C
 */

/* MATCHED, wave 43 (W43-I), first attempt.
 *
 * Walks the byte script gUnknown_084997C8 selects with
 * gUnknown_0849D89C->unk04->unk00 and draws one UnitInfoPanel_DrawIconPair row per entry
 * pair. 0x07 terminates; 0x06 is a break that resets the row counter to 4 and
 * advances by ONE byte instead of two.
 *
 * SUB_0803AAC0 TAKES FOUR ARGUMENTS, NOT THREE, and this function is the
 * evidence. unknown-functions.h had `(u8, u8, u8)` since wave 35, whose own
 * note says the arity was read off the argument setup at the call sites -- but
 * sub_0803AB3C was unmatched then and it is the call site that sets r3
 * (`lsls r3,#0x18; asrs r3,#0x18`, an s8 conversion, immediately before the
 * `bl`). Both the prototype and src/decomp/c_0803AAC0.c were given the fourth
 * `s8` parameter; that file is byte-identical after the change (re-verified),
 * because a trailing parameter the body never reads costs no instructions.
 *
 * `e` is `s8` holding 0, 0xd and -1. The ROM materialises -1 as `movs r3,#0xff`
 * and pays the sign extension at the CALL, not at the assignment -- PROMOTE_MODE
 * keeps the pseudo zero-extended and converts at the use.
 *
 * `c` and `d` ARE LOCALS, not `p[i]` / `p[i + 1]` spelled at the call: the ROM
 * loads both at the top of the else-arm, before `e = 0`, and parks `d` in the
 * callee-saved r5 across the whole if/else. Spelled at the call they would load
 * beside the `bl`. The leading `p[i] == 6` test is a SEPARATE read -- the ROM
 * has two `ldrb`s off the same `p + i` -- so `c` is assigned inside the else.
 *
 * `& ~1` and not `& 0xfe`: the ROM builds the mask `movs r0,#2; rsbs r0,r0,#0`,
 * which is the int constant -2. `0xfe` would be a bare `movs r0,#0xfe`. */
void sub_0803AB3C(void)
{
    u8 *p;
    u8 i;
    u8 k;
    u8 c;
    u8 d;
    s8 e;

    p = gUnknown_084997C8[gUnknown_0849D89C->unk04->type];

    i = 0;
    k = 0;

    while (p[i] != 7)
    {
        if (p[i] == 6)
        {
            k = 4;
            i++;
        }
        else
        {
            c = p[i];
            d = p[i + 1];
            e = 0;

            if ((c & 1) == 0)
            {
                if (c == (p[i + 2] & ~1))
                    e = 0xd;
            }
            else
            {
                if (i > 1 && p[i - 1] != 6 && (c & ~1) == p[i - 2])
                    e = -1;
            }

            UnitInfoPanel_DrawIconPair(k, c, d, e);
            i += 2;
            k++;
        }
    }
}
