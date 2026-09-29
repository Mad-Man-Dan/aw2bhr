#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080195F8.
 * sub_080195F8 @ 0x080195F8
 */

#include "hardware.h"
/* The two-option chooser on the sub_08019578 proc: Up/Down (or Select) moves
 * between entries 0 and 1, A commits, and anything else falls through.
 *
 * THE FIRST TEST IS `!= 0`, NOT `== 0`, and that is the whole difference
 * between this and a candidate that is otherwise instruction-exact. Both arms
 * end in the same commit-and-return block, so agbcc reverses a condition
 * written the natural way round -- the same reordering that decided
 * sub_0802F408 and IsNeotanksUnlocked this wave. Spelling the test to match the
 * ROM's `beq` puts the `== 1` arm inline where the ROM has it.
 *
 * `gUnknown_0808E5A8` is agbcc's -fforce-addr address constant for gpKeySt
 * (baserom.gba holds 0x03002EE0 in that word), so the pointer is named directly
 * and each arm loads its own copy -- the ROM does not CSE it across the branch
 * and neither does naming it three times. */
struct Unk80195F8
{
    /* 0x00 */ u8 filler_00[0x1e];
    /* 0x1e */ s16 unk1e;
    /* 0x20 */ u8 filler_20[0x08];
    /* 0x28 */ u16 *unk28;
    /* 0x2c */ void (*unk2c)(void);
};

void sub_080195F8(void *proc)
{
    struct Unk80195F8 *p = (struct Unk80195F8 *)proc;

    sub_0803670C();

    if (p->unk1e != 0)
    {
        if (p->unk1e == 1 && (gpKeySt->unk0c & DPAD_LEFT))
        {
            p->unk1e = 0;
            sub_08019578(p);
            sub_0803B4DC(0x67);
            return;
        }
    }
    else if (gpKeySt->unk0c & (B_BUTTON | DPAD_RIGHT))
    {
        p->unk1e = 1;
        sub_08019578(p);
        sub_0803B4DC(0x67);
        return;
    }

    if (gpKeySt->unk0c & 1)
    {
        gUnknown_03002EE4 = p->unk1e;
        ClearTilemapRect23x4(p->unk28);
        p->unk2c();
        sub_08015C30(gUnknown_03001FBC);
        sub_0803B4DC(0x65);
    }
}
