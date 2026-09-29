#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08044AB8.
 * sub_08044AB8 @ 0x08044AB8
 */

/* Matched wave 43 (W43-J), after eight waves parked at 88.8% with every
 * register one lower than the ROM's. Two independent statement splits, both
 * needed, neither reachable by the permuter (it permutes existing allocnos):
 *  - `m = (j - 1) * 5;` on its own line keeps that addend emitted FIRST while
 *    moving the sum's accumulator onto `i * 17`, which is what the ROM does.
 *    W35 measured the whole-expression swap at 82.5% and concluded the written
 *    order was right -- the emission order was, the accumulator was not.
 *  - `p = (const u8 *)gUnknown_084A0090;` as its own statement AFTER both byte
 *    reads, then `p += ...`. W43-H had measured the same binding BEFORE the
 *    reads (pool ldr moves to the top) and inlined (loaded late); this is the
 *    third position and the one that wins the base callee-saved r4.
 * See the W43-J chapter in docs/agbcc-codegen.md. */

void StartCoPowerAnimation(int a)
{
    const u8 *p;
    int i;
    int j;
    int m;
    int b0;
    int b1;

    i = gPlayers[a].co;
    j = gPlayers[a].coActivationMode;
    p = (const u8 *)gUnknown_084A0090;
    m = (j - 1) * 5;
    p += (i * 17 + m) * 4;
    b0 = p[0x1c];
    b1 = p[0x1d];

    LoadSparkleGraphics(GetCoPowerAnimation(b0), GetCoPowerAnimationPalette(b1), 0x1ca, 5);
}
asm(".global sub_08044AB8\n.thumb_set sub_08044AB8, StartCoPowerAnimation\n");
