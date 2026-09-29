#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08029AF8.
 * RepairUnit @ 0x08029AF8
 */

/* MATCHED in wave 73 (W73-H), 248/248 bytes. Previously parked at 240/248 (-8)
 * across waves 59 (W59-B), 60 (W60-D) and 73 (W73-C).
 *
 * THE ANSWER: write the SUBTRACTION in the shifted domain, not the exit test.
 *
 *     t = (int)(((u32)a2 << 16) - 0x10000);  a2 = (u32)t >> 16;
 *     } while (t != 0);
 *
 * Nineteen previously-measured spellings all kept the subtract NARROW (`a2 - 1`,
 * `--a2`, `a2 -= 1`, `a2 += 0xFFFF`, ascending) and varied only where the shift
 * and the test went, including W73-C's near miss below, which asks `combine` to
 * DISTRIBUTE the `<< 16` over the `- 1`. combine declines every time. Written
 * already-shifted, nothing has to be distributed: expand emits the shift and
 * final output materialises -65536 as `ldr rN,=0xFFFF0000` inside the loop,
 * which is exactly the ROM. The a2/v register swap was downstream of this and
 * needed no work. The notes below are kept as a record of the negatives; their
 * CONCLUSION (that the form is unreachable from C) is refuted.
 *
 * WAVE 73 (W73-C) REFUTED THE STANDING DIAGNOSIS. Read this before probing.
 *
 * Waves 59/60 concluded the lever was WHICH VALUE THE EXIT TEST READS: the ROM
 * tests the PRE-NARROWED temp, which was said to be what lets `combine`
 * distribute the `<< 16` over the `- 1` and produce `+ 0xFFFF0000`. That is
 * wrong. The exit test has now been moved onto the pre-narrowed temp, by
 * introducing an explicit `int t` between the decrement and the store, and the
 * distribution STILL does not happen:
 *
 *     t = (a2 - 1) << 16;  a2 = (u32)t >> 16;  } while (t != 0);
 *        ->  sub r0,r7,#1 ; lsl r0,r0,#0x10 ; lsr r7,r0,#0x10 ; cmp r0,#0 ; bne
 *
 * That candidate tests r0, the shifted temp, exactly as the ROM does, and
 * combine still emits `sub` before `lsl` rather than `lsl` before an add of
 * 0xFFFF0000. So the exit test's operand is NOT the lever, and the brief's rule
 * ("a non-coalesced `lsr rTMP,rV,#k ; mov rV,rTMP` means the exit test reads the
 * temp") is a correct READ-OUT of the ROM but does not identify what to write.
 *
 * WAVE 60'S "USEFUL NEGATIVE" IS ALSO WRONG AND SHOULD NOT STOP ANYONE. It
 * argued combine cannot have produced the ROM's form because "combine has no way
 * to emit a pool load", and that the `ldr` sitting inside the loop rather than
 * hoisted proves the constant was created after loop.c and so is unreachable
 * from C. Both halves are mistaken: `ldr rN,=0xFFFF0000` is not an RTL memory
 * load at all, it is how final output MATERIALISES any SImode immediate that is
 * not an 8-bit THUMB constant, so combine producing `(plus (ashift x 16)
 * (const_int -65536))` yields exactly those two instructions -- and a constant
 * created after loop.c is precisely what LICM cannot have hoisted. The residual
 * is an ordinary combine decision, and the axis is open, not closed.
 *
 * WHAT THIS WAVE'S SHAPE DID BUY, and why it is the new fixpoint: introducing
 * the `int t` temp fixes the COUNTER REGISTER SWAP that waves 59/60 recorded as
 * a separate unexplained difference. The old `} while (--a2 != 0);` draft put
 * a2 in r6 and v in r7; the ROM has a2 in r7 and v in r6, and so does this. The
 * swap was downstream of the decrement's shape, as suspected -- one fact, not
 * two.
 *
 * REMAINING RESIDUAL, unchanged at -8 bytes: the ROM computes the decrement in
 * the SHIFTED domain and this does not.
 *     ROM    lsls r0,r7,#0x10 ; ldr r1,=0xFFFF0000 ; adds r0,r0,r1
 *            lsrs r7,r0,#0x10 ; cmp r0,#0 ; bne <top>
 *     draft  subs r0,r7,#1 ; lsls r0,r0,#0x10 ; lsrs r7,r0,#0x10
 *            cmp r0,#0 ; bne <top>
 * Exactly 8 bytes: 2 for the extra instruction, 4 for the pool word, 2 for the
 * `.short 0` alignment the ROM's odd pool-word count needs.
 *
 * THE ONE CHANGE THAT GOT THE FUNCTION HERE (wave 59): `goto _done` INSTEAD OF
 * `break` for the loop's LEADING exit test -- see "Loop rotation is decided by
 * the LEADING exit test" in docs/agbcc-codegen.md. With `break`, expand_end_loop
 * rolls the first conditional exit down to the bottom and enters with a `b`; the
 * ROM does not roll it. A user `goto` to a label after the loop is not a jump to
 * the loop's own end_label, so the roll does not fire. The top guard, the LICM
 * hoist of the bitfield mask (`movs r4,#0x7f` in the preheader) and hence the
 * THIRD saved hi register all followed from that single edit.
 *
 * DECREMENT SPELLINGS ARE EXHAUSTED -- ELEVEN across waves 59 and 60, all
 * emitting the identical narrow-domain tail. DO NOT SWEEP THEM AGAIN:
 *   `} while (--a2);` / `a2--;` + `} while (a2 != 0);` /
 *   `while (a2 != 0) { ... a2 = a2 - 1; }` / `for (; a2 != 0; a2--)` /
 *   a `u16 n = a2;` copy (identical tail AND an extra `add r4,r1,#0`) /
 *   `while (a2-- != 0)` (pre-decrements before the loop) /
 *   ASCENDING `for (i = 0; i < a2; i++)` with both `u16 i` and `int i` -- this
 *   REFUTES the check_dbra_loop-reversed-an-ascending-loop theory here: it burns
 *   a register on i, moves a2 to r8 and loses the force-addr word entirely /
 *   the decrement as an assignment expression in the condition, all four of
 *   `while ((a2 = a2 - 1) != 0)`, `while ((a2 -= 1) != 0)`,
 *   `while ((u16)(a2 = a2 - 1) != 0)`, `while ((a2 = a2 - 1))` /
 *   `a2 += 0xFFFF` (LICM hoists the constant into a preheader register) /
 *   the exit test written in the shifted domain on the STORED counter,
 *   `} while ((a2 = a2 - 1) << 16);` and `a2 = a2 - 1; } while (a2 << 16);`
 *   (combine folds the truth test back to `cmp rN,#0`).
 * Wave 73 adds the two int-temp forms above: `t = a2 - 1; a2 = t;` (this draft)
 * and `t = (a2 - 1) << 16; a2 = (u32)t >> 16;`. Same tail, same -8.
 *
 * NEXT LEVER, and it is NOT the counter. Everything that decides the decrement
 * has been swept. What has never been varied is the loop's other content: the
 * distribution is a combine COST decision, so the thing to change is what else
 * is live across the bottom of the loop, not how the counter is written.
 *
 * NOT TRIED: decomp-permuter. Wave 37 lists a one-extra-instruction residual as
 * the case it cannot reach, but the wave-60 caveat stands -- it closed
 * TurnStartRepair_Loop and did not move StartCameraScroll -- so a chained run is worth the
 * budget if anyone has it.
 *
 * SETTLED, keep as-is: `gPlayers` is a POINTER to an array of 60-byte
 * elements (`ldr r1,[r0]` then index*60) and `.unk00` is a 32-bit member.
 * `[r5,#4]` is a 7-bit count plus a preserved top bit (the header's `unk04_0`)
 * -- the `!= 0` test compiles to `movs #0x7f ; ands` while the VALUE read
 * compiles to `lsls #0x19 ; lsrs #0x19`, and agbcc does not spell one `& 0x7f`
 * two different ways, so the bitfield model is right. `Div`'s divisor is 10,
 * `acc` is int in r8, `v` is u16.
 */

int RepairUnit(struct Unit *p, u16 a2, u8 a3)
{
    int acc;
    u16 v;
    int t;

    acc = 0;
    v = GetUnitCostWithCoBonus(gUnknown_030033EC, p->type);

    if (a2 != 0)
    {
        do
        {
            if (p->hp != 0 && Div(p->hp - 1, 10) == 9)
                goto _done;

            if (a3)
            {
                if (gPlayers[gUnknown_030033EC].funds < v)
                    goto _done;

                SubtractPlayerFunds(gUnknown_030033EC, v);
            }

            acc += v;
            p->hp += 10;

            if (p->hp > 100)
                p->hp = 100;

            t = (int)(((u32)a2 << 16) - 0x10000);
            a2 = (u32)t >> 16;
        } while (t != 0);
    }

_done:
    p->hp = p->hp != 0 ? (Div(p->hp - 1, 10) + 1) * 10 : 0;

    return acc;
}

asm(".global sub_08029AF8\n.thumb_set sub_08029AF8, RepairUnit\n");
