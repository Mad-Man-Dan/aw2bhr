#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801E2A4.
 * sub_0801E2A4 @ 0x0801E2A4
 */

/* MATCHED (wave 40, W40-J), re-verified. Links to identical bytes; the
 * promotion must carry
 *     "rodata": ["0x0808F0B4"]
 * because gUnknown_0808F0B4 IS NOT A GLOBAL. It is agbcc's own -fforce-addr
 * address constant holding &gUnknown_0200F720 -- dereference baserom.gba at
 * 0x8F0B4 and it reads 0x0200F720. Declaring it as a `struct Unk0200F720 *` and
 * reading through it costs one extra `ldr` per iteration and cannot match;
 * naming the real object is the honest spelling and the one the ROM used.
 *
 * The per-frame interpolation sweep over the 32 records: each axis steps its
 * current value one Nth of the way to its target and decrements N, and any
 * record that moved gets UpdateObjAffineRecord called on its index.
 *
 * THE MEMBER IS READ TWICE PER TEST AND THAT IS THE WHOLE TYPE ARGUMENT.
 * `ldrh r5,[r4,#6]` is a plain HImode move into the local that supplies the
 * `- 1`; the `movs r1,#6; ldrsh r0,[r4,r1]` immediately after is the test,
 * which needs SImode and becomes a second, sign-extending load. cse does not
 * unify them because `(zero_extend (mem:HI))` and `(sign_extend (mem:HI))` are
 * different expressions. That is why the source has to name a local -- written
 * inline as `p->unk06 - 1` the subtraction reads through `ldrsh` and the load
 * moves inside the `if`.
 *
 * The pointer `p` is bound once per iteration rather than repeating
 * `gUnknown_0200F720[i].member`: the `strh` stores in the first arm may alias
 * the object, so the repeated form re-derives the base for the second arm and
 * costs a callee-saved register. */
void StepObjAffineTweens(void)
{
    int i;
    int flag;
    u16 n;
    struct Unk0200F720 *p;

    for (i = 0; i <= 0x1f; i++)
    {
        flag = 0;
        p = &gUnknown_0200F720[i];
        n = p->unk06;
        if ((s16)p->unk06 != 0)
        {
            p->unk00 += ((s16)p->unk08 - (s16)p->unk00) / (s16)p->unk06;
            p->unk02 += ((s16)p->unk0a - (s16)p->unk02) / (s16)p->unk06;
            p->unk06 = n - 1;
            flag = 1;
        }
        n = p->unk0c;
        if ((s16)p->unk0c != 0)
        {
            p->unk04 += ((s16)p->unk0e - (s16)p->unk04) / (s16)p->unk0c;
            p->unk0c = n - 1;
            flag = 1;
        }
        if (flag != 0)
            UpdateObjAffineRecord(i);
    }
}
asm(".global sub_0801E2A4\n.thumb_set sub_0801E2A4, StepObjAffineTweens\n");
