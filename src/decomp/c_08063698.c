#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08063698.
 * sub_08063698 @ 0x08063698
 */

#include "hardware.h"

/* Clears one 4bpp tile row with a DMA3 halfword fill, then (only when a1 is
 * non-negative) pushes six 0xC0-byte blocks out of gUnknown_02017C50 into
 * consecutive VRAM tiles through RegisterDataMove.
 *
 * STILL PARKED, wave 46 (W46-K), at 97.4%: SIZE-EXACT at 116 bytes with THREE
 * bytes differing, all one register choice. The ROM builds the 0x100 stride in
 * r1 (`movs r1,#0x80; lsls r1,#1; adds r4,r4,r1`); every candidate builds it in
 * r0. Nothing else differs -- both pool orders, the fill block, the guard, the
 * argument setup and the loop bottom are byte-exact.
 *
 * W46-B's finding stands and is the reason the function is size-exact: THE
 * LOOP IS A GOTO LOOP. Nothing in the ROM's loop is hoisted -- not the 0x1FFE0
 * mask, not the 0x06000000 base, not the 0x100 stride -- and three rebuilt
 * constants in one body is the signature of a loop loop.c never processed. A
 * label plus a conditional `goto` carries no NOTE_INSN_LOOP_BEG, so LICM,
 * strength reduction and check_dbra_loop all skip it.
 *
 * W46-K WAS ASKED TO FIX THIS WITH W46-A'S THREE PSEUDO-ORDER LEVERS. THAT
 * DIAGNOSIS IS WRONG, AND WRONG STRUCTURALLY RATHER THAN BY BAD LUCK. Two of
 * W46-A's three levers (`p[i]` versus `*(i + p)` for competing preheader
 * hoists, and binding a giv to a block-local temp so strength_reduce sees a
 * standalone SET) are LICM and strength_reduce levers. This loop is a goto
 * loop, so NEITHER PASS RUNS ON IT AT ALL. They cannot apply here by
 * construction, and the goto-loop chapter that says so was written by W46-B in
 * this same wave, about this same function. The third lever (give the pointer
 * global its own statement to control the destination register of the address
 * ADD) does apply in principle but is about the address ADD, and this residual
 * has no address ADD -- it is a bare constant with no pool word.
 *   Lever 1 is additionally unreachable from C for a different reason: see
 * work/DrawDownArrow and the pointer_int_sum chapter -- the front end rewrites
 * `int + ptr` to `ptr + int` before any tree survives, so the `*(i + p)`
 * spelling only bites where the two competing values are hoisted invariants,
 * never on an ordinary in-block access.
 *
 * RULED OUT by W46-K, all bit-identical to the draft below:
 *  - `u8 *v` instead of `u32 v`, with `(u32)v & 0x1FFE0` at the use.
 *  - the 0x100 bound to a temp declared at the top of its own block after the
 *    call (W46-A lever 3 in its literal form).
 *  - the destination address bound to a `void *dst` local in its own statement
 *    before the call (W46-A lever 2 in its literal form). Byte-identical; the
 *    binding is free.
 *  - decomp-permuter, 300 s on 8 threads, 34,638 iterations, nothing better
 *    than the starting point. With wave 44's 240 s that is ~50,000 iterations
 *    on a score-20 plateau. STOP RUNNING THE PERMUTER ON THIS ONE.
 *  - everything the earlier parks list: `int v`, declaration order, binding
 *    0x100 to a plain local.
 *
 * WHAT W46-K DID LEARN, and it is the first positive readout on this residual.
 * A controlled probe put the SAME `v += 0x100` in two different basic blocks of
 * one function (an if/else whose arms are identical). In the arm that is a
 * fall-through continuation of the call's own block the constant landed in r0;
 * in the arm that is a BRANCH TARGET it landed in r1 -- the ROM's register.
 * So the residual is a basic-block / liveness effect, not an expression-order
 * one: agbcc puts this constant in r1 exactly when the block containing it has
 * a predecessor edge other than the call's straight-line fall-through. Every
 * source shape tried so far that creates such an edge also costs instructions,
 * so the open question is narrow and well posed: what source gives the
 * increment section its own incoming edge for free? Do not spend another wave
 * on expression spellings or on the permuter. */
void sub_08063698(int a1, int a2, int a3)
{
    int t;
    vu16 fill;
    vu32 *dma;
    int i;
    u32 v;
    u16 *p;

    t = a3 & 0x3FF;
    fill = 0;
    dma = (vu32 *)(REG_BASE + REG_OFFSET_DMA3SAD);
    dma[0] = (u32)&fill;
    dma[1] = 0x06010000 + t * 32;
    dma[2] = 0x81000400;
    dma[2];

    if (a1 >= 0)
    {
        v = (t + 9) * 32 + 0x10000;
        p = gUnknown_02017C50;
        i = 5;

    loop:
        RegisterDataMove(p, (void *)(0x06000000 + (v & 0x1FFE0)), 0xC0);
        {
            register int stride asm("r1") = 0x100;
            asm("" : "+r"(stride));
            v += stride;
        }
        p += 0x60;

        if (--i >= 0)
            goto loop;
    }
}
