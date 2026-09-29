#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08019578.
 * sub_08019578 @ 0x08019578
 */

/* Rewrites two OBJ attribute pairs in the proc's own buffer -- entry 0 and the
 * entry the proc's unk1e selects, in both of two 0x40-byte halves -- and then
 * calls the proc's stored callback.
 *
 * unk18 is a `u16 *`, so the ROM's byte offsets are element indices: 0x40 is
 * [0x20] and the `unk1e * 8` scaling is `unk1e * 4` elements. The `adds r1,
 * #0x40` / `adds r1, #8` chain is addressing, not a member array: 0x40 is past
 * `strh`'s 5-bit displacement, and the second offset is CSEd off the first.
 *
 * unk1e and unk30 are each named TWICE and reloaded both times -- the `strh`
 * in between may alias them, so CSE does not fire and neither should a bound
 * local. The pointer at +0x18 IS loaded once, because nothing writes through
 * anything that could alias it before the second use. */
struct Unk8019578
{
    /* 0x00 */ u8 filler_00[0x18];
    /* 0x18 */ u16 *unk18;
    /* 0x1c */ u8 filler_1c[0x02];
    /* 0x1e */ s16 unk1e;
    /* 0x20 */ u8 filler_20[0x0c];
    /* 0x2c */ void (*unk2c)(void);
    /* 0x30 */ u16 unk30;
};

void ChoiceBox_DrawCursor(void *proc)
{
    struct Unk8019578 *p = (struct Unk8019578 *)proc;

    p->unk18[0] = 0;
    p->unk18[0x20] = 0;
    p->unk18[4] = 0;
    p->unk18[0x24] = 0;

    p->unk18[p->unk1e * 4] = p->unk30 | 0x209;
    p->unk18[p->unk1e * 4 + 0x20] = p->unk30 | 0x20A;

    p->unk2c();
}
asm(".global sub_08019578\n.thumb_set sub_08019578, ChoiceBox_DrawCursor\n");
