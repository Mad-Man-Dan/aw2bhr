#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080872D0.
 * sub_080872D0 @ 0x080872D0
 */

/* Wave 66 park, size exact (580), 99.3%, four allocation bytes remain.
 *
 * The A4 guard form is retained: binding `unk37` to `m` before binding `g - 1`
 * to `k` makes all seven guard instructions and their registers exact. A clean
 * uninterrupted 300-second permuter run from that source searched 12,558
 * iterations. Its best legal source added the dead assignment `i = 0x608F` in
 * the final PutSprite argument, improving the configured result from 95.9% to
 * 99.3%. The only remaining code difference is the literal load immediately
 * before that call: ROM uses `ldr r0; str r0, [sp]`, while agbcc uses r4 for
 * both instructions. A block-scoped fixed-r0 palette local regressed to 94.5%
 * by perturbing the earlier sprite-loop allocation.
 *
 * Wave 74 remeasured this draft with the configured matcher and retained it:
 * exact 580/580, four differing allocation bytes. Two additional clean
 * 120-second `--current` runs (4,188 and 4,520 iterations) found no better
 * source. Fresh block locals of several integer widths produce the desired
 * final r0 pair but rotate r4/r5 in both earlier sprite loops; coordinate
 * factoring and comma/first-reference forms do not repair that rotation.
 *
 * The two `.rodata` relocs at +0x17c/+0x184 are the -fforce-addr blob words
 * 0x081D9448 / 0x081D944C, which dereference to &gUnknown_02027F74 and
 * &gUnknown_0848B688 -- what this draft names directly. Per the wave-55
 * correction the reloc line is not the defect; size is exact, so the pool
 * LENGTH is right. Promotion must carry "rodata": ["0x081D9448","0x081D944C"].
 *
 * The two `.rodata` relocs are equivalent address-constant pool words at
 * 0x081D9448 and 0x081D944C. They resolve to gUnknown_02027F74 and
 * gUnknown_0848B688 and are not part of the residual. */

/* WAVE 77 (W77-M) CORRECTION to the wave-74 note above.  The dead
 * `i = 0x608F` assignment is NOT fixing the final PutSprite call.  Remove it
 * and the draft is 24 differing bytes, all of them a systematic r4/r5/r0
 * rotation in the two EARLIER PutSprite loops, in both arms of the outer if:
 * the ROM keeps the counter `i` in r4 and the strength-reduced `i * 8 + 0x28`
 * giv in r5, and the candidate reverses them.  The dead assignment adds one
 * more reference to `i` at the far end of the function, flips that global tie
 * the right way, and then routes the constant through i's home r4 -- which is
 * the 2 real bytes that remain (the other 2 of the reported 4 are addend bytes
 * of the equivalent .rodata alias at +0x70 and +0x184).  The open problem is
 * what puts `i` in r4 honestly.
 *
 * Measured from the honest 24-byte draft: `i == 0 ? 0x6088 : 0x6089` is 27
 * (30 on both loops); inlining the m/k temporaries into the if is 33.  Any
 * other dead local is worse or is deleted: k 24, m 29, a1 506, w 340, a fresh
 * `int n` 24; the same assignment on BOTH arms is 6. */


/* Wave 80 (W80-F). What puts `i` in r4 honestly: the ROM's j-loops keep their
 * counter in r4 -- the SAME register as `i`, with no copy between them (the
 * W80-B "same register in different blocks = one reused variable" tell). The
 * original reuses `i` as the counter of all four DivRem loops. That gives the
 * one pseudo `i` the refs of six loops, it wins r4 against the strength-reduced
 * giv on priority rather than on a dead reference, and the final PutSprite
 * constant is no longer routed through r4. The dead `i = 0x608F` is gone. */

void MapSelectList_DrawScrollbar(int a1)
{
    int i;
    int v;
    int w;
    int k;
    int m;

    if (gPlaySt.gameMode == 2)
    {
        w = 0x70;
        v = Div(a1 * 0x70, gUnknown_02027F74.unk37 + 1);
        if (gUnknown_02027F74.unk37 != 0)
            w = Div(gUnknown_03005928 * 0x70, gUnknown_02027F74.unk37);

        for (i = 0; i <= 0xd; i++)
            PutSprite(5, 0x58, i * 8 + 0x28, (u16 *)gUnknown_0848B688,
                      i != 0 ? 0x6089 : 0x6088);

        if (a1 > gUnknown_02027F74.unk37 - (int)gUnknown_03005928)
        {
            for (i = 0; i < w - 8; i++)
                if (DivRem(i, 8) == 0 || i + 1 == w - 8)
                    PutSprite(4, 0x58, i + 0x29 + v, (u16 *)gUnknown_0848B688, 0x608F);
        }
        else
        {
            for (i = 0; i < w - 8; i++)
                if (DivRem(i, 8) == 0 || i + 1 == w - 8)
                    PutSprite(4, 0x58, i + 0x29 + v, (u16 *)gUnknown_0848B688, 0x608F);
        }
    }
    else
    {
        w = 0x50;
        v = Div(a1 * 0x50, gUnknown_02027F74.unk37 + 1);
        if (gUnknown_02027F74.unk37 != 0)
            w = Div(gUnknown_03005928 * 0x50, gUnknown_02027F74.unk37 + 1);

        if (gUnknown_02027F74.unk37 < (int)gUnknown_03005928)
            return;

        for (i = 0; i <= 9; i++)
            PutSprite(5, 0x48, i * 8 + 0x48, (u16 *)gUnknown_0848B688,
                      i != 0 ? 0x6089 : 0x6088);

        m = gUnknown_02027F74.unk37;
        k = (int)gUnknown_03005928 - 1;
        if (a1 > m - k)
        {
            for (i = 0; i < w - 8; i++)
                if (DivRem(i, 8) == 0 || i + 1 == w - 8)
                    PutSprite(4, 0x48, i + 0x48 + v, (u16 *)gUnknown_0848B688, 0x608F);
        }
        else
        {
            for (i = 0; i < w - 8; i++)
                if (DivRem(i, 8) == 0 || i + 1 == w - 8)
                    PutSprite(4, 0x48, i + 0x49 + v, (u16 *)gUnknown_0848B688, 0x608F);
        }
    }
}
asm(".global sub_080872D0\n.thumb_set sub_080872D0, MapSelectList_DrawScrollbar\n");
