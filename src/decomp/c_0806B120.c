#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0806B120.
 * sub_0806B120 @ 0x0806B120
 */

/* MATCHED in wave 73 (W73-C). The wave-48 park was correct that the two writes
 * at +0x02 are `store_bit_field` output and genuinely want a bitfield
 * declaration -- and correct that gUnknown_0202F214's SHARED declaration must
 * not be reshaped, because StartCreditsMissionLine (promoted, matched wave 48) reads the
 * same halfword and a bitfield READ widens to the enclosing word (`ldr`), which
 * that ROM does not have.
 *
 * WHAT THE PARK MISSED, and it is the general lesson: the two constraints are
 * not actually in conflict, because a bitfield view can be LOCAL TO THE WRITER.
 * `struct Unk0202F214Rec` below is declared in this .c and reached by a cast.
 * The shared `struct Unk0202F214 { u16 unk00; u16 unk02; }` is UNCHANGED, so
 * StartCreditsMissionLine is untouched and still matches (re-verified this wave). The
 * wave-48 ruled-out list only covered a UNION carrying a bitfield struct --
 * which does inflate the element from 4 to 8 bytes -- and a unilateral reshape
 * of the shared struct. A local view type is neither.
 *
 * This is the brief's own rule read forwards: when your diff demands a
 * different width for a member someone else already typed, that is evidence
 * about YOUR ACCESS, not about the struct. Here the access is a bitfield view;
 * elsewhere it is a cast or a different member at the same offset.
 *
 * WHY THE BITFIELDS FIX MORE THAN THE TWO STORES. A plain masked read-modify-
 * write through a `u8 *` view gets both stores' WIDTHS right (`ldrb`/`strb` for
 * the 2-bit field, `ldrh`/`strh` for the 14-bit one) but is 24 bytes SHORT, and
 * for two separate reasons that are really one:
 *   - `store_bit_field` builds its inverse mask as a full-word constant, so
 *     ~3 is emitted as `movs r3,#4; rsbs r3,r3,#0; adds r1,r3,#0` (three
 *     instructions) where a QImode `& ~3` narrows to a single `movs #0xfc`.
 *   - that extra live constant is exactly the register pressure that stops
 *     `strength_reduce` from making the element address a pointer giv. With the
 *     bitfields, agbcc keeps `i * 4` as the giv (r6), re-loads the base from the
 *     force-addr pool word every iteration (`mov r3,r8; ldr r4,[r3]`) and needs
 *     r8, which is the ROM's `mov r7,r8; push {r7}` prologue. Without them it
 *     hoists the base into a pointer giv and drops r8 entirely.
 * So the 4-byte mask sequence and the 20-byte allocation difference are ONE
 * fact, not two -- the same "a misplaced load and a swapped register are one
 * fact" mechanism, seen through register pressure.
 *
 * THE TAIL IS THE OTHER HALF, and it is why unk00 is written through the plain
 * shared type rather than the view. Both spellings give the same six tail
 * instructions, but through the cast the index shift is emitted BEFORE the base
 * load; through the array symbol the base load comes first, which is the ROM's
 * order (`mov r1,r8; ldr r0,[r1]; lsls r1,r7,#2; adds r1,r1,r0`). Use the view
 * only where a bitfield is actually needed.
 *
 * gUnknown_0816E16C IS NOT A GLOBAL. The word at 0x0816E16C holds 0x0202F214,
 * so it is an ordinary `-fforce-addr` address constant that gen_lds.py invented
 * a symbol for; `gUnknown_0202F214` is written honestly and the build places the
 * pool word. (0x0816E170 holds 0x08499578 and is the same thing; 0x0816E174
 * holds 0x80000001 and IS real data.)
 *
 * `ldm r5!, {r1}` is strength_reduce fusing the pointer bump with the unk00_08
 * read -- do not author it. The three source-field reads confirm struct
 * Unk0200C078Rec: bits 0..7 fill QImode so `ldrb` with no shifts, bits 8..19
 * span bytes 1..2 so `ldr` plus `lsl #0xc; lsr #0x14`, bits 20..31 sit in the
 * high halfword so `ldrh [.,#2]; lsrs #4`. */

/* Local VIEW of gUnknown_0202F214's element, for the two +0x02 writes only.
 * The shared declaration in unknown-globals.h stays `u16 unk02` because
 * StartCreditsMissionLine reads it as a byte and a halfword. Same 4-byte size and
 * alignment either way, so the `lsls #2` index is unaffected. */
struct Unk0202F214Rec
{
    /* 0x00 */ u16 unk00;
    /* 0x02 */ u16 unk02_0 : 2;
    /* 0x02 */ u16 unk02_2 : 14;
};

void BuildCreditsMissionList(void)
{
    int i;

    i = 0;

    while (gUnknown_0200C420.unk38[i].unk00_08 != 0)
    {
        gUnknown_0202F214[i].unk00 = gUnknown_0200C420.unk38[i].unk00_00 + 0x8a;

        ((struct Unk0202F214Rec *)gUnknown_0202F214)[i].unk02_0 =
            5 - GetRankFromScore(gUnknown_0200C420.unk38[i].unk00_14);

        ((struct Unk0202F214Rec *)gUnknown_0202F214)[i].unk02_2 =
            gUnknown_0200C420.unk38[i].unk00_08;

        i++;
    }

    gUnknown_0202F214[i].unk00 = 0;
}
asm(".global sub_0806B120\n.thumb_set sub_0806B120, BuildCreditsMissionList\n");
