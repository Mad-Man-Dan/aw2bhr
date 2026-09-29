#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08074484.
 * sub_08074484 @ 0x08074484
 */

/* MATCHED (wave 38, W38-E), FIRST ATTEMPT, 236 bytes. Never attempted before.
 *
 * The interpreter for an 8-byte-stride condition script: it walks records until
 * the leading opcode byte is 8, and for each one either accepts the record
 * (fall through to the next) or rejects it, in which case SkipToMapEventFireRecord skips
 * ahead to the next opcode-7-or-8 record. Only opcode 7 does any work --
 * running the +0x04 script through StartOrQueueEventScript and reporting `1` back to the
 * caller.
 *
 * THE RECORD IS READ THROUGH `u8 *` WITH EXPLICIT CASTS, not through a struct.
 * That is forced from two sides and is worth stating because it looks wrong:
 * `u8 RunMapEventRecords(u8 *, struct Unk030040D8 *, int)` is the declared prototype,
 * and the already-promoted SkipToMapEventFireRecord (c_08074570.c) walks the same array as
 * a bare `u8 *` with `p += 8`. Both halves of the original agreed on `u8 *`, so
 * `*(u16 *)(p + 2)` and `*(const u8 **)(p + 4)` are the original's own spelling.
 *
 * CASE ORDER IN THE SOURCE IS 0, 7, 5, 6, 1, 4, 2, 3 -- read straight off the
 * ROM's block order, since agbcc emits case bodies in SOURCE order while the
 * jump table at _080744A4 is indexed by value and says nothing about it. Any
 * other order changes the byte stream. `_080744A4` is that jump table and NOT a
 * global: it is a local label the index lists as a data_ref, and the eight
 * `.4byte` entries are the case targets.
 *
 * The single shared `bl SkipToMapEventFireRecord` block at _08074556 is cross-jumping
 * merging eight identical `p = SkipToMapEventFireRecord(p);` tails -- it does NOT need a
 * goto, a flag variable, or a shared exit written in the source. Each case
 * simply ends with its own copy. Same for the `cmp r0, #0; beq` that case 3 and
 * case 6 share.
 *
 * Argument counts and widths that are readable rather than guessed:
 *   - `bl _call_via_r0` is a NULLARY indirect call (the register index counts
 *     the arguments), so +0x04 is `u8 (*)(void)` in cases 5 and 6.
 *   - Case 5's `lsls #0x18; lsrs #0x18` before `cmp #1` is agbcc re-narrowing a
 *     u8-returning callee's result; case 6's BARE `lsls #0x18` is the same u8
 *     return used as a truth test. The pair pins the return type at u8 twice
 *     over, from the two different uses.
 *   - Case 7's +0x04 is the `const u8 *` script pointer StartOrQueueEventScript declares,
 *     which is the same object cases 5 and 6 call through -- the union is real
 *     in the data, not a typing error here.
 *
 * Case 0's two guards are ONE `||` of two `&&` pairs, not two separate `if`s:
 * `(p[1] != 0 && p[1] != gUnknown_030033EC) || (u16 != 0 && u16 != g03004080)`.
 * The short-circuit structure reads straight out of the branch targets -- each
 * `&&`'s first test jumps FORWARD to the next disjunct on failure, each second
 * test jumps to the reject block on success.
 */

u8 RunMapEventRecords(u8 *p, struct Unk030040D8 *a2, int a3)
{
    u8 ret;

    ret = 0;

    while (p[0] != 8)
    {
        switch (p[0])
        {
        case 0:
            if ((p[1] != 0 && p[1] != gUnknown_030033EC)
             || (*(u16 *)(p + 2) != 0 && *(u16 *)(p + 2) != gUnknown_03004080))
                p = SkipToMapEventFireRecord(p);
            break;

        case 7:
            if (p[1] != 0xFF && IsCampaignCompletionFlagSet(p[1]) != 0)
            {
                p = SkipToMapEventFireRecord(p);
                break;
            }

            if (*(const u8 **)(p + 4) != 0)
            {
                StartOrQueueEventScript(*(const u8 **)(p + 4));
                ret = 1;
            }

            if (p[1] != 0xFF)
                SetCampaignCompletionFlag(p[1], 1);
            break;

        case 5:
            if ((*(u8 (**)(void))(p + 4))() != 1)
                p = SkipToMapEventFireRecord(p);
            break;

        case 6:
            if ((*(u8 (**)(void))(p + 4))())
                p = SkipToMapEventFireRecord(p);
            break;

        case 1:
            if (a2->unk00 != p[1])
                p = SkipToMapEventFireRecord(p);
            break;

        case 4:
            if (p[1] != a3)
                p = SkipToMapEventFireRecord(p);
            break;

        case 2:
            if (IsCampaignCompletionFlagSet(p[1]) == 0)
                p = SkipToMapEventFireRecord(p);
            break;

        case 3:
            if (IsCampaignCompletionFlagSet(p[1]) != 0)
                p = SkipToMapEventFireRecord(p);
            break;
        }

        p += 8;
    }

    return ret;
}
asm(".global sub_08074484\n.thumb_set sub_08074484, RunMapEventRecords\n");
