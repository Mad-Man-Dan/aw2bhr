#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08047920.
 * sub_08047920 @ 0x08047920
 */

/* Draw the six-row unit list of the 0x08047xxx panel: one row per live
 * gUnknown_02028DD8 entry from p->unk20 onward, stopping at the 0xff
 * terminator, at six rows maximum.
 *
 * `pal[2] = {0, 1}` is a real local array, not a pool word. agbcc has no
 * movstrsi for THUMB, so a 4-byte aggregate initialiser becomes a .rodata
 * template plus a memcpy call -- which is exactly the
 * `sub_0808B6E8(sp + 0xc, 0x0812A12C, 4)` the ROM opens with. The word at
 * 0x0812A12C is 0x00010000, i.e. those two u16s, and NOT an address constant.
 *
 * The other three words this unit carves ARE agbcc -fforce-addr address
 * constants: 0x0812A130 holds &gUnknown_02028DD8, 0x0812A134 holds
 * &gBG0TilemapBuffer and 0x0812A138 holds &gUnknown_085D5ABC. Naming the real
 * globals is what reproduces the double load.
 *
 * `(s16)(i * 2 + 7)` is spelled at every use rather than bound to a local: the
 * ROM keeps `i * 2` in a callee-saved register and recomputes the `+ 7` and the
 * extension on both sides of the Div merge. An `s16 y` local instead parks the
 * zero-extended row in its own pseudo and costs five instructions.
 *
 * The unk0b test reads `== 0`: the ROM branches AWAY to the unk0b arm, so the
 * nested unk11 test is the source's THEN arm and the unk0b arm is the ELSE.
 *
 * The unk01 bit-0 test respells the whole subscript instead of using `u`
 * because the intervening sub_0804769C call kills the address expression's CSE
 * class -- gUnits and gUnknown_03003F2C are both plain (non-const)
 * memory. The later field reads keep using `u`, which is what proves `u` is a
 * real local and that one access is a separate spelling.
 *
 * Wave 56, W56-K: the residual was 7 bytes, size exact, and it was NOT an
 * instruction-order problem -- it was the
 * tied operand of the `unk01 & (m = 1)` and, three spellings of which W56-G
 * already measured as worse (see the `SHARED-CONSTANT LEVER` chapter, and
 * W32-B for the mirror image with eleven more). `size: match` rules out a
 * pool-length error and nothing else; do not read it as "the order is wrong".
 * three instructions plus the pool-base register they displace.
 *
 * MATCHED, wave 71.  Bind the field byte first, then use a scratch fixed to r0:
 * `k = 1; m = k; k &= f;`.  This preserves `m` in r8 for the later mask while
 * forcing the tied operation to be `ands r0, r1`, closing all seven bytes.
 * The configured verdict is exact; the three .rodata names resolve to the
 * original address-constant words. */

void DrawUnitListRows(void *arg)
{
    u16 pal[2];
    u16 i;
    u16 n;
    u16 c;
    u8 id;
    u8 b;
    int m;
    int f;
    register int k asm("r0");
    int v;
    struct Unit *u;

    sub_0808B6E8(pal, gUnknown_0812A12C, 4);

    for (i = 0;
         i <= 5 && gUnknown_02028DD8[i + ((struct Unk0804769C *)arg)->unk20] != 0xff;
         i++)
    {
        id = gUnknown_02028DD8[i + ((struct Unk0804769C *)arg)->unk20];
        u = &gUnits[gUnknown_03003F2C + id];
        if (u->type == 0)
            break;

        n = sub_0804769C(arg, id) + 1;

        f = gUnits[id + gUnknown_03003F2C].flags;
        k = 1;
        m = k;
        k &= f;
        if (k)
            c = 0x100;
        else
            c = gUnknown_03003F2C;

        WriteUnitTileQuadAt(gBG0TilemapBuffer, n + 3, i * 2 + 7, u->type, c, u->unk07,
                     u->unk05_3);
        b = (u->flags >> 3) & m;
        PutTextTableEntryImmediate(8, (s16)(i * 2 + 7), gBG0TilemapBuffer,
                     gUnknown_085D5ABC[u->type].unk00, 0x8000, pal[b]);

        if (u->hp != 0)
            v = Div(u->hp - 1, 10) + 1;
        else
            v = 0;

        DrawTallNumberRightAligned(0x10, (s16)(i * 2 + 7), gBG0TilemapBuffer, v, 0x8000,
                     pal[b]);
        DrawTallNumberRightAligned(0x13, (s16)(i * 2 + 7), gBG0TilemapBuffer, u->fuel,
                     0x8000, pal[b]);
        PutTextScriptImmediate(0x14, (s16)(i * 2 + 7), gBG0TilemapBuffer,
                     gUnknown_084C3F38, 0x8000, pal[b]);
        DrawTallNumberRightAligned(0x16, (s16)(i * 2 + 7), gBG0TilemapBuffer,
                     gUnknown_085D5ABC[u->type].maxFuel, 0x8000, pal[b]);

        if (gUnknown_085D5ABC[u->type].maxAmmo == 0)
        {
            if (gUnknown_085D5ABC[u->type].unk11 != 0)
                PutTextTableEntryImmediate(0x18, (s16)(i * 2 + 7), gBG0TilemapBuffer, 0x962,
                             0x8000, pal[b]);
            else
                PutTextTableEntryImmediate(0x18, (s16)(i * 2 + 7), gBG0TilemapBuffer, 0x966,
                             0x8000, pal[b]);
        }
        else
        {
            DrawTallNumberRightAligned(0x19, (s16)(i * 2 + 7), gBG0TilemapBuffer, u->ammo,
                         0x8000, pal[b]);
            PutTextScriptImmediate(0x1a, (s16)(i * 2 + 7), gBG0TilemapBuffer,
                         gUnknown_084C3F3C, 0x8000, pal[b]);
            DrawTallNumberRightAligned(0x1b, (s16)(i * 2 + 7), gBG0TilemapBuffer,
                         gUnknown_085D5ABC[u->type].maxAmmo, 0x8000, pal[b]);
        }
    }
}
asm(".global sub_08047920\n.thumb_set sub_08047920, DrawUnitListRows\n");
