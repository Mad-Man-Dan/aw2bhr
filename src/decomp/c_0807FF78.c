#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0807FF78.
 * sub_0807FF78 @ 0x0807FF78, sub_0807FFF0 @ 0x0807FFF0
 */

#include "hardware.h"
#include "proc.h"

/* MATCHED wave 34, W34-E. Parked at 53.3% since wave 33 on what its note called
 * a pure register-allocation residual; it was a source-structure fact, and it
 * needed THREE locals where the draft had one.
 *
 * The ROM materialises &gUnknown_081D937C in a scratch, dereferences it, reads
 * the halfword, and only THEN copies the address to the register that survives:
 *
 *   ldr  r0, =gUnknown_081D937C   <- p  = gUnknown_081D937C
 *   ldr  r2, [r0]                 <-      (the pointer VALUE, live into the body)
 *   ldrh r1, [r2]                 <- v  = *p
 *   adds r3, r0, #0               <- pp = &gUnknown_081D937C
 *
 * Each local buys exactly one of those, and the ORDER of the three statements
 * is what places the copy. Binding only `pp` (wave 33's draft) collapses the
 * address into one pseudo that agbcc puts straight in r3, so the copy never
 * appears at all -- 53.3%. Adding `p` produces the copy but emits it BEFORE the
 * `ldrh` -- 95.8%, size-exact, five bytes out. Adding `v` is what pushes it
 * past the `ldrh`, because the address constant is then not wanted again until
 * the `pp` binding. Wave 33's "one local where the original had N" diagnostic
 * was pointing the right way; only the count was wrong.
 *
 * Wave 33's other measurement stands as far as it went: naming
 * gUnknown_081D937C directly at the use sites is +4 bytes, because
 * -fforce-addr parks the address in this unit's own .rodata (a `.LC` word) and
 * reaches it through a second `ldr` at BOTH sites. The `pp` binding removes
 * that .LC. Both halves of the wave-33 note were correct about single changes;
 * neither outcome was reachable without the other two locals, which is why four
 * attempts on one axis found nothing.
 *
 * The pointee's `volatile` is settled and required -- see the note on the
 * declaration in unknown-globals.h. It is what makes the body re-read through
 * `p` instead of forwarding `v` into `adds r0, r2, #1`.
 *
 * Its mirror CoPowerScreenBlend_FadeOutLoop closed on the identical three-local change. */
void CoPowerScreenBlend_FadeInLoop(void)
{
    volatile u16 **pp;
    volatile u16 *p;
    u16 v;

    p = gUnknown_081D937C;
    v = *p;
    pp = &gUnknown_081D937C;

    if (v < 0x10 && (gGameClock & 1))
    {
        (*p)++;
        gUnknown_03002B28--;
    }

    if (**pp != 0)
        gUnknown_030030E0.raw = (gUnknown_030030E0.raw & 0xE0FF) | 0x1E00;
    else
        gUnknown_030030E0.raw = (gUnknown_030030E0.raw & 0xE0FF) | 0x0E00;

    gUnknown_03001FF8 += 0x18;
}
asm(".global sub_0807FF78\n.thumb_set sub_0807FF78, CoPowerScreenBlend_FadeInLoop\n");

/* MATCHED wave 34, W34-E. The mirror of CoPowerScreenBlend_FadeInLoop -- read that file's header
 * for why the address of gUnknown_081D9380, its dereferenced value and the
 * halfword read each need their own local, and in that order. The identical
 * three-local change closed both functions unmodified, which is the evidence
 * that the lever is the source structure rather than either function's control
 * flow: this one ends in a Proc_Break arm and a call, that one in a plain
 * if/else, and both wanted exactly the same three statements at the top.
 *
 * Wave 33 parked this 4 bytes SHORT at 120 against the ROM's 124, missing
 * precisely the `adds r3, r0, #0` those locals produce. */
void CoPowerScreenBlend_FadeOutLoop(ProcPtr proc)
{
    volatile u16 **pp;
    volatile u16 *p;
    u16 v;

    p = gUnknown_081D9380;
    v = *p;
    pp = &gUnknown_081D9380;

    if (v != 0 && (gGameClock & 1))
    {
        (*p)--;
        gUnknown_03002B28++;
    }

    if (**pp == 0)
    {
        ClearBg0Tilemap();
        BG_EnableSyncBG0();
        Proc_Break(proc);
    }
    else
    {
        gUnknown_030030E0.raw = (gUnknown_030030E0.raw & 0xE0FF) | 0x1E00;
    }

    gUnknown_03001FF8 += 0x18;
}
asm(".global sub_0807FFF0\n.thumb_set sub_0807FFF0, CoPowerScreenBlend_FadeOutLoop\n");
