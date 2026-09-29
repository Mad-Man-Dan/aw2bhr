#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08048F4C.
 * sub_08048F4C @ 0x08048F4C
 */

#include "hardware.h"

/* Wave 80 (W80-A): MATCHED. Parked across waves 35, 36, 43 and 59 at +4
 * bytes, the whole residual being that the ROM RE-READS unk01e from memory
 * after the decrement (`subs r0,#1 / strh r0,[r2,#0x1e] / ldrh r0,[r2,#0x1e]`)
 * where agbcc forwarded the stored pseudo and paid `lsls/lsrs` to re-narrow
 * it. Ruled out by those waves (do not repeat): `unk01e = unk01e - 1`;
 * `unk020 = unk01e - 1` alone (forwards through the store, `sub r0,r1,#2`);
 * nested ifs; a u16 local; a dummy store; volatile; binding the pointer global
 * to a local; field width u8/u16; operand order inside the `==`; and the
 * kind-4 cross-jump label (W59-D showed the shared `strb` is byte-exact).
 * Seven compiler profiles were swept by the wave-80 orchestrator: none helps.
 *
 * The lever is the one the MATCHED TWIN UnitList_HandleInput (src/decomp/c_08047F70.c)
 * already carries: the comparison's left operand is an ASSIGNMENT to a dead
 * `int` local, `((v = p->unk01e) == p->unk020) && p->unk01e != 0`, and the
 * follow-up store is `p->unk020 = p->unk01e - 1`. With the load bound to a
 * named local the just-stored pseudo is not forwarded into the compare; the
 * value is reloaded (`ldrh r0,[r2,#0x1e]`), and `p->unk01e - 1` is then the
 * ROM's `subs r0,#1` off the reloaded register (cse knows unk01e's reload is
 * in r0). Written `p->unk020--` or with the plain comparison, agbcc forwards
 * the decrement result and the four extra bytes appear. The RIGHT arm needs
 * no such help because its stored value (`unk01e + 1`) was computed BEFORE
 * the guard branch, in a different basic block from the reload.
 *
 * Lesson for the parked-entry axis list: the residual was labelled "the
 * STORE itself"; the measured lever is on the LOAD side of the comparison, and
 * the matched twin had been carrying it since wave 30. Grep src/decomp/ for
 * the twin's spelling before probing (the wave-77 rule). */
void ShopList_HandleInput(void)
{
    struct Unk084C30F8 *p;
    int v;

    p = gUnknown_084C30F8;

    if (p->unk030 == 0 && (gpKeySt->repeated & (DPAD_UP | DPAD_DOWN)) != 0)
    {
        if ((gpKeySt->repeated & DPAD_UP) != 0)
        {
            if (p->unk01e == 0)
                return;

            p->unk01e--;

            if (((v = p->unk01e) == p->unk020) && (p->unk01e != 0))
            {
                p->unk020 = p->unk01e - 1;
                p->unk030 = 2;
            }
        }
        else
        {
            if (gUnknown_02028E1C[p->unk01e + 1] == 0xff)
                return;

            p->unk01e++;

            if (p->unk01e - p->unk020 == 2
                && gUnknown_02028E1C[p->unk020 + 3] != 0xff)
            {
                p->unk020++;
                p->unk030 = 3;
            }
        }

        PlayMusicOrSfx2(0x67);
    }

    ShopList_StepScroll();
}
asm(".global sub_08048F4C\n.thumb_set sub_08048F4C, ShopList_HandleInput\n");
