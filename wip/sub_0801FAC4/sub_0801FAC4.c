#include "global.h"

/* PARKED at 536/540 bytes (-4).
 * The switch must be written in source order 1,0,2,3 and `s = base + 1; s -=
 * k;` must remain two statements. The ROM uses two stack slots and spills the
 * loop-invariant bound in cases 0/2/3 while retaining the stepped value; this
 * draft uses one slot and spills the stepped value. Net residual: two missing
 * register copies/four bytes. Wave 61 tested zero-trip wrappers around the
 * case loops; they did not change the 536-byte residual. A scoped explicit
 * bound-local experiment was invalidated by historical permuter-mutated
 * best.c and was not evidence. Wave 65 repeated it cleanly with a distinct
 * scoped bound in cases 0, 2 and 3: the frame reaches the ROM's eight bytes,
 * but code grows to 548/540 (+8) and allocation diverges across those cases.
 * The one-slot 536-byte draft remains active. Prior 25,513-iteration permuter
 * and loop-variable coalescing probes were negative. Wave 71 isolated case 0:
 * a case-local or function-scope `bound = a2 + a4` still leaves a four-byte
 * frame and reallocates the arm, so case 0 alone is not the second-slot lever. */
void sub_0801FAC4(u16 a1, u16 a2, u16 a3, u16 a4, u8 a5)
{
    int k;
    int x;
    int y;
    int s;
    int e;
    int w;
    volatile int bound;

    k = 1;
    switch (a3) {
    case 1:
        for (y = a2; y > a2 - a4 && y >= 0; k++, y--) {
            s = a1 + 1;
            s -= k;
            w = k * 2 - 1;
            if (s < 0) {
                w += s;
                s = 0;
            }
            e = s + w;
            if (e > *(u16 *)gUnknown_08499590)
                e = *(u16 *)gUnknown_08499590;
            for (x = s; x < e; x++)
                gUnknown_03003340[y][x] = a5;
        }
        break;
    case 0:
        bound = a2 + a4;
        for (y = a2; y < bound && y < *(u16 *)(gUnknown_08499590 + 2); k++, y++) {
            s = a1 + 1;
            s -= k;
            w = k * 2 - 1;
            if (s < 0) {
                w += s;
                s = 0;
            }
            e = s + w;
            if (e > *(u16 *)gUnknown_08499590)
                e = *(u16 *)gUnknown_08499590;
            for (x = s; x < e; x++)
                gUnknown_03003340[y][x] = a5;
        }
        break;
    case 2:
        for (x = a1; x > a1 - a4 && x >= 0; k++, x--) {
            s = a2 + 1;
            s -= k;
            w = k * 2 - 1;
            if (s < 0) {
                w += s;
                s = 0;
            }
            e = s + w;
            if (e > *(u16 *)(gUnknown_08499590 + 2))
                e = *(u16 *)(gUnknown_08499590 + 2);
            for (y = s; y < e; y++)
                gUnknown_03003340[y][x] = a5;
        }
        break;
    case 3:
        for (x = a1; x < a1 + a4 && x < *(u16 *)gUnknown_08499590; k++, x++) {
            s = a2 + 1;
            s -= k;
            w = k * 2 - 1;
            if (s < 0) {
                w += s;
                s = 0;
            }
            e = s + w;
            if (e > *(u16 *)(gUnknown_08499590 + 2))
                e = *(u16 *)(gUnknown_08499590 + 2);
            for (y = s; y < e; y++)
                gUnknown_03003340[y][x] = a5;
        }
        break;
    }
}




