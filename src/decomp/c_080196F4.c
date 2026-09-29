#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080196F4.
 * sub_080196F4 @ 0x080196F4
 */

/* Apply a 0xFF-terminated table of 12-byte unit-spawn commands: 0xFE resets
 * gUnknown_030033EC (and the 0x40-scaled shadow gUnknown_03003F2C) to the
 * command's byte 1, anything else builds a unit through CreateUnitAtNoRefresh and
 * stamps the command's bytes into it, clamped against that unit type's caps in
 * gUnknown_085D5ABC. The entry value of gUnknown_030033EC is saved in sl and
 * restored on the way out, so the whole table runs "as" whichever army each
 * 0xFE selects.
 *
 * The `if (u == NULL)` exit is a BREAK, not a return: the ROM branches to the
 * same block the loop guard does, and that block still runs RebuildMapUnitLayers and
 * the restore.
 *
 * gUnknown_030033EC and gUnknown_03003F2C each get an agbcc `-fforce-addr`
 * `.rodata` word (the ROM's 0x0808E5AC and 0x0808E5B0), parked in r8/sb and
 * re-loaded from memory at every use -- five and four references respectively.
 * gUnknown_085D5ABC, by contrast, is CSEd to a single reference and gets an
 * ordinary inline pool word, which is why there is no local for the table
 * element: writing `t = &gUnknown_085D5ABC[u->type];` as its own statement
 * computes the element address BEFORE `u->fuel`, and the ROM computes the
 * bitfield extract first.
 *
 * THE `do { } while (0)` IS LOAD-BEARING -- it is worth 7 bytes and it is the
 * wave-17 zero-trip-block register lever, found by decomp-permuter after four
 * hand spellings failed. Without it the `movs r0, #0` feeding the unk09/unk0a
 * stores is allocated r1 instead of r0 and lands one slot early, between
 * `ldrb r0,[r5,#3]` and its `strb r0,[r4,#1]`; the extra block boundary forces
 * the constant to reuse the register the unk01 store just freed. Neither
 * spelling from the "where a shared constant's register is CREATED" rule in
 * docs/agbcc-codegen.md moved it (`u->unk0a = u->unk09 = 0;` and a
 * `u->unk09 = v = 0;` local were both probed and both came out unchanged), so
 * that rule does not cover a constant shared by two ADJACENT byte stores. */
struct Unk080196F4Cmd /* 0x0c */
{
    /* 0x00 */ u8 unk00;
    /* 0x01 */ u8 unk01;
    /* 0x02 */ u8 unk02;
    /* 0x03 */ u8 unk03;
    /* 0x04 */ u8 unk04;
    /* 0x05 */ u8 unk05;
    /* 0x06 */ u8 unk06;
    /* 0x07 */ u8 filler_07[0x02];
    /* 0x09 */ u8 unk09;
    /* 0x0a */ u8 filler_0a[0x02];
};

void ApplyUnitSpawnTable(void *arg)
{
    struct Unk080196F4Cmd *p;
    struct Unit *u;
    u16 saved;

    p = arg;
    saved = gUnknown_030033EC;

    while (p->unk00 != 0xFF)
    {
        if (p->unk00 == 0xFE)
        {
            gUnknown_030033EC = p->unk01;
            gUnknown_03003F2C = (gUnknown_030033EC - 1) * 0x40;
        }
        else
        {
            u = CreateUnitAtNoRefresh(p->unk00, p->unk01, p->unk02);
            if (u == NULL)
                break;
            u->hp = p->unk04;
            u->fuel = p->unk06;
            do
            {
                if (p->unk05 == 0x63)
                    u->ammo = 0xF;
                else
                    u->ammo = p->unk05;
                u->flags = p->unk03;
            } while (0);
            u->unk09 = 0;
            u->unk0a = 0;
            u->unk0b = p->unk09;
            if (u->fuel > gUnknown_085D5ABC[u->type].maxFuel)
                u->fuel = gUnknown_085D5ABC[u->type].maxFuel;
            if (u->ammo > gUnknown_085D5ABC[u->type].maxAmmo)
                u->ammo = gUnknown_085D5ABC[u->type].maxAmmo;
        }
        p++;
    }

    RebuildMapUnitLayers();
    gUnknown_030033EC = saved;
    gUnknown_03003F2C = (gUnknown_030033EC - 1) * 0x40;
}
asm(".global sub_080196F4\n.thumb_set sub_080196F4, ApplyUnitSpawnTable\n");
