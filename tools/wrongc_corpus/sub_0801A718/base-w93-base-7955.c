#include "global.h"

/* 68.9%, size exact. gUnknown_0200C618 now has a linker symbol and the tail
 * names it, which removed the extra load described below. The remaining
 * difference is register choice and when the array base is loaded: the
 * original loads &gUnknown_0200C624 after computing the node index and
 * derives the sentinel from that same register; this draft loads it first.
 * The older notes below predate the symbol.
 *
 * PARKED at 59.1% (size exact, 132 B). Wave 41, W41-A.
 *
 * The SHAPE IS SOLVED -- the whole remaining residual is ONE extra `ldr` in
 * the tail statement plus the register shuffle it drags along.
 *
 * What the function is: a sorted insert into the singly-linked list whose
 * nodes are the 0x0c-byte gUnknown_0200C624 records (unk00 payload, unk04
 * next, unk08 the s16 sort key) and whose header is gUnknown_030020A8
 * (unk00 count, unk04 head). Entry [-1] of that array, i.e. 0x0200C618, is
 * the head sentinel; `prev == NULL` is what distinguishes it from a real
 * predecessor. Returns -1 when the pool is full (count > 0x80).
 *
 * WHY IT WILL NOT CLOSE. The ROM's literal pool holds three words:
 *     0x0801A734 -> 0x0808E5CC   (.rodata slot, contains 0x030020A8)
 *     0x0801A75C -> 0x0200C624   (direct address constant)
 *     0x0801A760 -> 0x0808E5D0   (.rodata slot, contains 0x0200C618)
 * The first and third are agbcc's own -fforce-addr address pool -- a PAIR of
 * consecutive .rodata words, exactly as docs/agbcc-codegen.md describes.
 * Values read out of baserom.gba, not inferred.
 *
 * Word 1 is reproduced by naming gUnknown_030020A8 honestly (done below):
 * agbcc emits `.LC0: .word gUnknown_030020A8` and promotion places it at
 * 0x0808E5CC. Word 3 must contain 0x0200C618, so the original source named an
 * OBJECT AT 0x0200C618 -- the sentinel record sitting immediately before the
 * array.
 *
 * That symbol cannot be spelled here:
 *   - aw2bhr.lds jumps 0x00C528 -> 0x00C624, so there is no RAM symbol at
 *     0x0200C618, and inventing one in include/unknown-globals.h fails the
 *     split build with `undefined reference` (the wave-32 lesson).
 *   - `gUnknown_0200C624 - 1` does NOT produce a second address constant:
 *     agbcc under -fforce-addr keeps the base in a register and applies the
 *     -0xc at runtime, and CSE folds the two uses together. Probed: the
 *     .rodata slot then holds 0x0200C624 and the tail becomes
 *     `ldr; sub #8; ldr` -- three instructions where the ROM has two.
 *   - `gUnknown_0808E5D0->unk04`, the pointer-object workaround used for
 *     gUnknown_0808E5C8, is what this draft does. It is right everywhere
 *     EXCEPT that agbcc adds its own .rodata level on top (the load is hoisted
 *     into the loop preheader), so the emitted word holds 0x0808E5D0 instead
 *     of 0x0200C618 and the tail reads
 *     `ldr r0,[r6]; ldr r0,[r0]; ldr r0,[r0,#4]` -- one indirection too many.
 *     Probed separately: OUTSIDE a loop the same spelling emits a direct pool
 *     word and exactly two loads, which is why the workaround holds for
 *     sub_0801A6C0 and not here.
 *
 * TO CLOSE IT: add a RAM symbol at 0x0200C618 (0x0c bytes, one
 * struct Unk0808E5C8) to aw2bhr.lds and name it in the tail statement in
 * place of gUnknown_0808E5D0. That is an edit to the upstream linker script,
 * so it is flagged in the W41-A report rather than done here.
 *
 * WAVE 58 (W58-A) VERIFIED THAT FIX BY PROBE, so it is no longer a proposal.
 * Compiled with `extern struct Unk0808E5C8 gUnknown_0200C618;` declared and the
 * tail written `gUnknown_030020A8.unk04 = gUnknown_0200C618.unk04;`, agbcc emits
 *
 *     .LC2: .word gUnknown_0200C618        (its own .rodata address constant)
 *     ldr r6,=.LC2                         (preheader, as the ROM)
 *     ldr r0,[r6] ; ldr r0,[r0,#4]         (tail -- TWO loads, the ROM's)
 *
 * The extra indirection is gone and the function becomes SIZE-EXACT. So the
 * only thing standing between this draft and a match is the linker symbol, and
 * the exact one-line edit is:
 *
 *     aw2bhr.lds, EWRAM section, between the 0x00C528 and 0x00C624 lines:
 *         . = 0x00C618; gUnknown_0200C618 = .;
 *
 * NOT done here: aw2bhr.lds is upstream's file, tools/gen_lds.py consumes it to
 * build aw2bhr.split.lds, and `try_match` compiles one unit and cannot gate a
 * link error -- so the edit is unverifiable from this seat mid-wave. It needs
 * one full split build to confirm. That is the whole remaining risk; the C is
 * settled.
 *
 * ALSO MEASURED THIS WAVE, and it kills the obvious cheaper alternative:
 * qualifying the pointer-object workaround `const`
 * (`extern struct Unk0808E5C8 *const gUnknown_0808E5D0;`) does NOT remove the
 * force-addr level. It still emits `.LC: .word <ptr>` in .rodata and still
 * reads `ldr r0,[r6]; ldr r0,[r0]; ldr r0,[r0,#4]` -- three loads. Wave 42's
 * `u16 *const` row (which DOES reach the right level count for a scalar in
 * sub_08010EF8) does not generalise to a pointer-to-struct object here. There
 * is no declaration of a ROM word that reaches two loads; only naming the RAM
 * object does.
 *
 * One residual would remain after the symbol exists, and it is small and
 * already localised: spelling `cur = gUnknown_0200C624 - 1` makes agbcc apply
 * the -0xc to the base pseudo in place (`sub r2,r2,#0xc`, ONE instruction)
 * where the ROM copies first (`adds r3,r0,#0; subs r3,#0xc`, TWO -- THUMB's
 * `subs rD,#imm8` is two-operand, so the copy appears exactly when the
 * allocator does NOT coalesce cur with the base). Spelling `cur =
 * &gUnknown_0200C618` instead restores the instruction COUNT but replaces those
 * two with `ldr r0,.LC; ldr r3,[r0]`. One of the two is right; decide it with a
 * try_match once the symbol is in the linker script.
 *
 * Everything else in this draft is verified against the listing: the double
 * `node->unk04 = cur` on the mid-insert path is REAL (the store through
 * `prev` may alias `node`, so agbcc cannot eliminate the second one), and the
 * loop's `b` into the middle with the prev/cur advance laid out above the
 * header is agbcc's own rotation of this `for (;;)`, not source. */
s16 sub_0801A718(struct Unk0200ED20 *a1, s16 key)
{
    struct Unk0808E5C8 *node;
    struct Unk0808E5C8 *prev;
    struct Unk0808E5C8 *cur;
    struct Unk0808E5C8 *next;
    struct Unk0808E5C8 *base;

    if ((s16)gUnknown_030020A8.unk00 > 0x80)
        return -1;

    base = gUnknown_0200C624;
    cur = base - 1;
    node = &base[(s16)gUnknown_030020A8.unk00];
    node->unk00 = (u32)a1;
    node->unk08 = key;

    prev = NULL;

    for (;;)
    {
        if (key <= cur->unk08)
        {
            node->unk04 = cur;
            if (prev != NULL)
            {
                prev->unk04 = node;
                node->unk04 = cur;
                break;
            }
        }
        next = cur->unk04;
        if (next == NULL)
        {
            node->unk04 = next;
            cur->unk04 = node;
            break;
        }
        prev = cur;
        cur = next;
    }

    gUnknown_030020A8.unk00++;
    gUnknown_030020A8.unk04 = gUnknown_0200C618.unk04;
    return 0;
}
