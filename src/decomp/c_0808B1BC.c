#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0808B1BC.
 * sub_0808B1BC @ 0x0808B1BC
 */

#include "hardware.h"

/* agb_flash's erase-then-program-a-whole-sector entry point. Four phases:
 *
 *   1. Range-check the sector number and compute the flash address
 *      0x0E000000 + (sector << gUnknown_03005C78->unk08).
 *   2. Relocate sub_0808B2E0 (the "is this sector still blank" scan) into a
 *      stack buffer so it can run out of RAM while the chip is busy. The
 *      length is the BYTE distance to the next function, sub_0808B304, so the
 *      counter steps by 2 per halfword copied.
 *   3. Issue the erase (EraseFlashSector_MX) and check it took, retrying up to 0x51
 *      times; then re-issue it once more, or six more times if it did not take
 *      first try.
 *   4. Relocate the OTHER RAM routine over the same buffer with SetReadFlash1,
 *      set the chip's WAITCNT bits, and walk the sector one byte at a time
 *      through ProgramByte_MX, counting gUnknown_03005C7C down.
 *
 * MATCHES (wave 60, W60-I) -- byte-for-byte, relocations included, at -O1.
 * Verified at -O1 both WITH and WITHOUT `-fforce-addr`: it matches either way,
 * so it puts no constraint on the block's flag. It does NOT match at -O2
 * (89.7%, size exact, flag removed), so the `-O1` override entry is
 * load-bearing.
 *
 * THE PARK NOTE THAT STOOD HERE WAS MEASURED AT -O2 AND ITS DIAGNOSIS WAS
 * WRONG. It reported 91.1% / 26 differing bytes and named three "register-role
 * choices" plus a `-fforce-addr` requirement. At -O1 all of that collapsed to
 * FIVE bytes, and those five were one SOURCE decision, not an allocation the
 * compiler chose:
 *
 *   ONE statement    p = (const u16 *)((u32)sub_0808B2E0 ^ 1);
 *   TWO statements   p = (const u16 *)sub_0808B2E0;
 *                    p = (const u16 *)((u32)p ^ 1);
 *
 * are different code. The one-statement form creates a FRESH pseudo for the
 * XOR result, so the pool value stays live and is COPIED:
 *     ldr r1,=f ; movs r0,#1 ; adds r3,r1,#0 ; eors r3,r0
 * The two-statement form assigns back to the same variable, the XOR clobbers
 * that pseudo, and the address is RE-LOADED from the same pool word at its
 * second use. This ROM wants the copy, so it wants ONE statement -- the
 * opposite of its three neighbours ReadFlash / AF00 / AF74, which want the
 * reload and therefore two. Both axes matter and neither decides it alone; see
 * the "-O1 versus -O2" chapter in docs/agbcc-codegen.md.
 */
u16 ProgramFlashSector_SST(u16 sectorNum, u8 *src)
{
    u16 readFlash1Buffer[0x30];
    const u16 *p;
    u16 *q;
    u16 n;
    u8 *dest;
    u16 result;
    u8 tries;
    u8 count;

    if (sectorNum > 0xf)
        return 0x80ff;

    dest = (u8 *)(0x0e000000 + (sectorNum << gUnknown_03005C78->unk08));

    p = (const u16 *)((u32)sub_0808B2E0 ^ 1);
    q = readFlash1Buffer;
    n = (u8 *)sub_0808B304 - (u8 *)sub_0808B2E0;

    while (n != 0)
    {
        *q++ = *p++;
        n -= 2;
    }

    tries = 0;

    while (1)
    {
        result = EraseFlashSector_MX(sectorNum);

        if (result == 0)
        {
            result = sub_0808B304((int)dest,
                                  (int (*)(int))((u8 *)readFlash1Buffer + 1));

            if (result == 0)
                break;
        }

        tries++;

        if (tries == 0x51)
            return result;
    }

    count = 1;

    if (tries != 0)
        count = 6;

    for (tries = 1; tries <= count; tries++)
        EraseFlashSector_MX(sectorNum);

    SetReadFlash1(readFlash1Buffer);

    REG_WAITCNT = (REG_WAITCNT & 0xfffc) | gUnknown_03005C78->unk10;
    gUnknown_03005C7C = gUnknown_03005C78->unk04;

    while (gUnknown_03005C7C != 0)
    {
        result = ProgramByte_MX(src, dest);

        if (result != 0)
            break;

        gUnknown_03005C7C--;
        src++;
        dest++;
    }

    REG_WAITCNT = (REG_WAITCNT & 0xfffc) | 3;

    return result;
}
asm(".global sub_0808B1BC\n.thumb_set sub_0808B1BC, ProgramFlashSector_SST\n");
