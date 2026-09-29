#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08033230.
 * sub_08033230 @ 0x08033230
 */

#include "proc.h"
#include "hardware.h"
/* MATCHED byte-for-byte, wave 50 (W50-J).
 *
 * NEEDS ITS .rodata POOL WORDS PLACED: 0x08090D6C, 0x08090D70, 0x08090D74,
 * holding &gUnknown_03003F70, &gUnknown_0849B018 and &gUnknown_0849B060.
 * They are agbcc's own -fforce-addr address constants and the honest spelling
 * reproduces all three, in order, from the loop's register pressure. Note the
 * SPLIT: the same three globals are also read AFTER the loop through a plain
 * `ldr rN,=sym` with no ROM word (gUnknown_03003F70 at 0x080333F4), and agbcc
 * decides that per reference by itself -- do not try to spell either side.
 *
 * THE DEAD `ldrb` BEFORE EVERY `strb` IS THE VOLATILE TELL, NOT A SOURCE
 * FEATURE. struct Unk0849B018's unk09 and unk0a[] are already declared
 * volatile in unknown-globals.h on exactly this evidence, so the plain
 * `&=` / `|=` / `= K` spellings emit it for free. Reading those loads as real
 * code is how this function would have cost a wave.
 *
 * The switch on gUnknown_03003F70[0x18] is a real `switch`, not an else-chain:
 * both `cmp`s sit back-to-back with the DEFAULT body inline behind them and
 * the two case bodies after it. Each case repeats the whole statement; agbcc
 * cross-jumps the common suffix from `movs r2,#0` onwards, which is why only
 * the `movs r1,#K` differs between them. The `(unk1e & 0xe)` else-arm shares
 * its tail with the `> 0xdf` block the same way.
 *
 * `proc->unk64 == 0` really is tested twice -- once as the outer guard and
 * again before Proc_GotoScript. agbcc cannot fold the second test because
 * LinkScreenSetMessage clobbers memory in between, so the reload is not evidence of
 * a different structure.
 *
 * gUnknown_030030E0 is written through `.raw`, not `.bits`: the ROM clears
 * bits 0..4 with one pooled 0xffe0 and ORs 0x1f, and struct BlendCnt declares
 * those five as SEPARATE 1-bit fields, which would be five read-modify-writes.
 * `& ~0x1f` would have pooled 0xffffffe0; the ROM's word is 0x0000ffe0. */

struct Unk33230Proc
{
    /* 0x00 */ u8 filler_00[0x64];
    /* 0x64 */ s16 unk64;
};

void LinkMultiboot_PollClients(struct Unk33230Proc *proc)
{
    int i;

    if (proc->unk64 == 0)
    {
        for (i = 1; i <= 3; i++)
        {
            if (((gUnknown_03003F70[0x1d] >> i) & 1) == 0)
            {
                gUnknown_0849B018->unk09 &= ~(1 << i);
                gUnknown_0849B018->unk0a[i] = 0;
            }
            else if (((gUnknown_03003F70[0x1e] >> i) & 1) == 0)
            {
                gUnknown_0849B018->unk0a[i] = 1;
            }
            else
            {
                gUnknown_0849B018->unk09 |= 1 << i;
                gUnknown_0849B018->unk0a[i] = 3;
            }
        }

        if ((gUnknown_03003F70[0x1e] & 0xe) != 0)
            gUnknown_0849B018->unk09 |= 1;
        else
            gUnknown_0849B018->unk09 &= 0xe;

        if ((gUnknown_03003F70[0x1e] & 0xe) != 0)
        {
            switch (gUnknown_03003F70[0x18])
            {
            case 0:
                gUnknown_0849B060->unk00 =
                    LinkScreenSetMessage(gUnknown_0849B060->unk00, 2, 0);
                break;

            case 0xd1:
                gUnknown_0849B060->unk00 =
                    LinkScreenSetMessage(gUnknown_0849B060->unk00, 0xe, 0);
                break;

            default:
                gUnknown_0849B060->unk00 =
                    LinkScreenSetMessage(gUnknown_0849B060->unk00, 0, 0);
                break;
            }

            if (gUnknown_03003F70[0x18] > 0xdf)
                gUnknown_0849B060->unk00 =
                    LinkScreenSetMessage(gUnknown_0849B060->unk00, 0xe, 0);
        }
        else
        {
            gUnknown_0849B060->unk00 =
                LinkScreenSetMessage(gUnknown_0849B060->unk00, 0, 0);
        }

        if (proc->unk64 == 0 && (gpKeySt->pressed & 2))
            Proc_GotoScript(proc, gUnknown_0849BA68);
    }

    if ((gpKeySt->pressed & 9) && proc->unk64 == 0
        && gUnknown_03003F70[0x18] == 0 && gUnknown_03003F70[0x1e] != 0)
    {
        proc->unk64 = 1;
        Proc_Start(gUnknown_0861429C, PROC_TREE_3);
        gUnknown_030030E0.raw = (gUnknown_030030E0.raw & 0xffe0) | 0x1f;
        Proc_Break(proc);
    }

    if (proc->unk64 != 1)
        MultiBootMain(gUnknown_03003F70);
}
asm(".global sub_08033230\n.thumb_set sub_08033230, LinkMultiboot_PollClients\n");
