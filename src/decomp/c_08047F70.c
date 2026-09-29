#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08047F70.
 * sub_08047F70 @ 0x08047F70
 */

#define UnitList_HandleInput sub_08047F70_decl
#undef UnitList_HandleInput
#include "hardware.h"
/* MATCHED (wave 77, W77-I), by decomp-permuter. Was parked at 98.8% since
 * wave 36 on SIX bytes -- three instances of one instruction, `mov r1, r8`
 * emitted as `mov r3, r8`:
 *
 *   ROM        mov r1, r8 / ldr r0,[r1] / ldr r0,[r0] / ldrh r1,[r0, #2]
 *   candidate  mov r3, r8 / ldr r0,[r3] / ldr r0,[r0] / ldrh r1,[r0, #2]
 *
 * r8 holds the -fforce-addr address constant &gpKeySt for the whole function,
 * read six times, once at the head of each `gpKeySt->` test after the first.
 * Three of the six already used r1; the differing three were the 2nd, 4th and
 * 6th -- a strict alternation, which is a least-recently-used pick inside
 * reload with no operand, order or shape component the source can name
 * directly. Wave 36 spent four hand attempts on it and correctly concluded
 * that this is the one documented case for the permuter rather than a fifth
 * spelling.
 *
 * THE WHOLE FIX IS THE `(v = p->unk1f)` BINDING IN THE FIRST ARM'S CONDITION.
 * The assignment is dead -- `v` is overwritten in the 0x80 arm before it is
 * ever read -- and it emits no instruction. What it does is extend `v`'s live
 * range across the first arm, which changes the pressure reload sees at each
 * of the six r8 reads and flips the alternating pick onto r1 throughout. This
 * is the same lever W77-C recorded (a narrow binding in an `if` condition is a
 * register lever, and the permuter is what finds it); it is not something a
 * hand rewrite was ever likely to reach, because the binding is semantically
 * pointless and only its live range matters.
 *
 * DO NOT "TIDY" THAT BINDING OUT. It looks like an artefact and it is the
 * match. Re-run trymatch after any edit here, cosmetic ones included -- the
 * body below is the permuter's result with whitespace and brace style restored
 * to the file's own conventions and nothing else changed (re-verified: exit 0).
 *
 * WHAT WAS FIXED BY HAND ON THE WAY, all measured in wave 36 and still load-
 * bearing:
 *  - The 0x200 arm's if/else is written with the `<= 5` (zero) arm FIRST. The
 *    ROM branches `bgt` past the fall-through to the `-6` arm, which is what
 *    gcc emits when the arm reached by FALLING THROUGH is the one written
 *    first. Writing it `> 5` first swapped both arms in the output.
 *  - The 0x80 arm's else needs `v = p->unk20 + 1;` bound to its own local.
 *    Written inline as `p->unk1f - (p->unk20 + 1) > 4`, fold reassociates it
 *    to `(p->unk1f - 1) - p->unk20` and emits `subs r0, r5, #1`, where the ROM
 *    materialises `unk20 + 1` once and STORES that same register.
 *  - `limit` (0x80 arm) and `d` (0x200 and 0x100 arms) must be ONE s16 local,
 *    not two. As two locals the whole function's callee-saved assignment
 *    shifted by one and twelve further instructions differed.
 *  - Local declaration order is byte-neutral (measured, wave 36).
 *
 * TOOLING NOTE: `python tools/permute.py UnitList_HandleInput` aborts at setup with
 * `conflicting types for 'UnitList_HandleInput'`. It seeds from best.c by default, and
 * this function's best.c is an older hand file lacking the `#define` shadow
 * above, so after cpp the unit carries two conflicting declarations. Run it as
 * `python tools/permute.py UnitList_HandleInput --current` to seed from this file.
 *
 * gUnknown_0812A13C is the -fforce-addr word holding &gpKeySt (dereferenced in
 * baserom.gba: 0x0812A13C -> 0x03002EE0, and aw2bhr.lds puts gpKeySt exactly
 * there). Do NOT reintroduce a `gUnknown_0812A13C` object.
 *
 * ON PROMOTION this function needs:  "rodata": ["0x0812A13C"]
 */

struct Unk08047B98
{
    /* 0x00 */ u8 filler_00[0x1e];
    /* 0x1e */ u8 unk1e;
    /* 0x1f */ u8 unk1f;
    /* 0x20 */ u8 unk20;
    /* 0x21 */ u8 unk21;
    /* 0x22 */ u8 unk22;
};
void RefreshUnitList(struct Unk08047B98 *);

void UnitList_HandleInput(struct Unk08047B98 *p)
{
    int changed;
    s16 d;
    int v;

    changed = 0;

    if (gpKeySt->repeated & DPAD_UP)
    {
        if (p->unk1f == 0)
            return;

        p->unk1f--;
        changed = 1;

        if (((v = p->unk1f) == p->unk20) && (p->unk1f != 0))
            p->unk20 = p->unk1f - 1;
        else if (p->unk1f == (p->unk20 - 1))
            p->unk20 = p->unk1f;

        PlayMusicOrSfx2(0x67);
    }

    if (gpKeySt->repeated & DPAD_DOWN)
    {
        if (p->unk1f >= (p->unk21 - 1))
            return;

        p->unk1f++;
        changed = 1;

        if (p->unk21 <= 5)
            d = 0;
        else
            d = p->unk21 - 6;

        if (((p->unk1f - p->unk20) == 5) && (p->unk20 < d))
        {
            p->unk20++;
        }
        else
        {
            v = p->unk20 + 1;
            if ((p->unk1f - v) > 4)
                p->unk20 = v;
        }

        PlayMusicOrSfx2(0x67);
    }

    if (gpKeySt->repeated & L_BUTTON)
    {
        if (p->unk20 == 0)
            return;

        changed = 1;
        d = p->unk1f - p->unk20;

        if ((p->unk1f - d) <= 5)
        {
            p->unk1f = d;
            p->unk20 = 0;
        }
        else
        {
            p->unk1f -= 6;
            p->unk20 -= 6;
        }

        PlayMusicOrSfx2(0x67);
    }

    if (gpKeySt->repeated & R_BUTTON)
    {
        if (p->unk20 >= (p->unk21 - 6))
            return;

        changed = 1;
        d = p->unk1f - p->unk20;

        if (((p->unk1f - d) + 0xc) > p->unk21)
        {
            p->unk20 = p->unk21 - 6;
            p->unk1f = p->unk20 + d;
        }
        else
        {
            p->unk1f += 6;
            p->unk20 += 6;
        }

        PlayMusicOrSfx2(0x67);
    }

    if (gpKeySt->repeated & DPAD_LEFT)
    {
        if (p->unk1e == 0)
            p->unk1e = 3;
        else
            p->unk1e--;

        changed = 1;
        PlayMusicOrSfx2(0x67);
    }

    if (gpKeySt->repeated & DPAD_RIGHT)
    {
        if (p->unk1e > 2)
            p->unk1e = 0;
        else
            p->unk1e++;

        changed = 1;
        PlayMusicOrSfx2(0x67);
    }

    if (gpKeySt->pressed & (SELECT_BUTTON | START_BUTTON))
    {
        if (p->unk21 == 0)
            return;

        gUnknown_02028E18 = (gUnknown_02028E18 + 1) & 1;
        changed = 1;
        PlayMusicOrSfx2(0x67);
    }

    if (changed == 1)
        RefreshUnitList(p);
}
asm(".global sub_08047F70\n.thumb_set sub_08047F70, UnitList_HandleInput\n");
