#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0808B31C.
 * sub_0808B31C @ 0x0808B31C, sub_0808B3C0 @ 0x0808B3C0, sub_0808B430 @ 0x0808B430, sub_0808B4B4 @ 0x0808B4B4, sub_0808B540 @ 0x0808B540
 */

#include "hardware.h"

/* Program one sector: erase it, then walk the source buffer a byte at a time
 * through ProgramByte_MX.
 *
 * gUnknown_03005C7C is the loop counter and it is a GLOBAL -- re-loaded at the
 * top of every iteration and stored back after every decrement, which needs no
 * `volatile` because ProgramByte_MX in the body already stops agbcc caching it.
 * The seeding store names the global and the loop goes through a pointer bound
 * just after it: naming the global at all five sites parks the address constant
 * in this unit's own .rodata and reaches it through a second indirection, while
 * binding the pointer before the store loses the ROM's `adds r6, r1, #0`. */
u16 ProgramFlashSector_MX(u16 sectorNum, u8 *src)
{
    u16 buf[0x20];
    u32 sector;
    u16 *remaining;
    u8 *dest;
    u16 result;

    sector = sectorNum;

    if (sector > 0xf)
        return 0x80FF;

    do
    {
        result = EraseFlashSector_MX(sector);

        if (result != 0)
            return result;

        SetReadFlash1(buf);

        REG_WAITCNT = (REG_WAITCNT & ~3) | gUnknown_03005C78->unk10;

        gUnknown_03005C7C = gUnknown_03005C78->unk04;

        dest = (u8 *)(0x0E000000 + ((u16)sector << gUnknown_03005C78->unk08));
    } while (0);

    remaining = &gUnknown_03005C7C;

    while (*remaining != 0)
    {
        result = ProgramByte_MX(src, dest);

        if (result != 0)
            break;

        (*remaining)--;
        src++;
        dest++;
    }

    REG_WAITCNT = (REG_WAITCNT & ~3) | 3;

    return result;
}
asm(".global sub_0808B31C\n.thumb_set sub_0808B31C, ProgramFlashSector_MX\n");

u16 EraseFlashChip_AT(void)
{
    u16 buffer[0x20];
    u16 result;

    SetReadFlash1(buffer);

    REG_WAITCNT = (REG_WAITCNT & 0xfffc) | gUnknown_084856A4[0x12];

    *(volatile u8 *)0x0E005555 = 0xAA;
    *(volatile u8 *)0x0E002AAA = 0x55;
    *(volatile u8 *)0x0E005555 = 0x80;
    *(volatile u8 *)0x0E005555 = 0xAA;
    *(volatile u8 *)0x0E002AAA = 0x55;
    *(volatile u8 *)0x0E005555 = 0x10;

    result = gUnknown_03005C70(3, (u8 *)0x0E000000, 0xFF);

    REG_WAITCNT = (REG_WAITCNT & 0xfffc) | 3;

    return result;
}
asm(".global sub_0808B3C0\n.thumb_set sub_0808B3C0, EraseFlashChip_AT\n");

/* The erase-sector command sequence for one sector, with interrupts masked
 * across it. gUnknown_084856A4 is the ROM flash descriptor: +0x1c is the sector
 * shift and +0x18 the sector's size in bytes -- the same two members
 * gUnknown_03005C78 names unk08 and unk04, reached 0x14 higher up the record.
 *
 * The descriptor's ADDRESS is bound to a local because the literal pool word is
 * `gUnknown_084856A4` with addend 0 and the two loads are displacements off it.
 * Pointer arithmetic on the array instead folds the offsets into the address
 * constant -- measured: `*((const u8 *)g + 0x1c)` emits a pool word relocating
 * to g+0x1c and then reaches +0x18 as `subs rN, #4`.
 *
 * The unlock/command writes must be volatile: 0x0E005555 is written twice with
 * different values and agbcc would otherwise drop one. Same idiom as the
 * matched ProgramByte_MX. The `addr--` after the fill backs the cursor up to the
 * LAST byte written, which is what the poll helper is handed.
 *
 * PARKED at -4 bytes. Every instruction from the REG_IME save onward is
 * byte-exact; the whole residual is that the ROM does NOT coalesce the narrowed
 * parameter with `addr`:
 *
 *     ROM   lsrs r0, r0, #0x10 ; adds r4, r0, #0 ; ldrb r1, [r3, #0x1c] ; lsls r4, r1
 *     ours  lsrs r4, r0, #0x10 ;                   ldrb r0, [r3, #0x1c] ; lsls r4, r0
 *
 * i.e. one `adds r4, r0, #0` short, and the pool then loses its 2-byte
 * alignment pad. See data/parked.json for what was ruled out. */
u16 sub_0808B430(u16 sectorNum)
{
    const u8 *info;
    u32 addr;
    u16 savedIme;
    u16 result;
    u32 n;

    info = (const u8 *)gUnknown_084856A4;

    addr = sectorNum;
    addr <<= info[0x1c];
    addr += 0x0E000000;

    savedIme = REG_IME;
    REG_IME = 0;

    *(volatile u8 *)0x0E005555 = 0xAA;
    *(volatile u8 *)0x0E002AAA = 0x55;
    *(volatile u8 *)0x0E005555 = 0xA0;

    n = *(const u32 *)(info + 0x18);

    while (n != 0)
    {
        *(u8 *)addr = 0xFF;
        addr++;
        n--;
    }

    addr--;

    REG_IME = savedIme;

    result = gUnknown_03005C70(1, (u8 *)addr, 0xFF);

    if (result != 0)
        result = (result & 0xFF00) | 2;

    return result;
}

/* Erase the 32 flash sectors backing save slot `sectorNum`, retrying each one
 * once. The 64-byte stack local is the relocated read routine SetReadFlash1
 * copies out of ROM.
 *
 * NEEDS -O1 -- see data/compiler-overrides.json. The flash library came out of
 * the SDK prebuilt at -O1 and the whole 0x0808A-0x0808B block is built that
 * way. At -O2 this same source is 5 bytes off on register assignment around
 * the two REG_WAITCNT read-modify-writes, which is what parked it. */
u16 EraseFlashSector_AT(u16 sectorNum)
{
    u16 buf[0x20];
    u16 n;
    u16 tries;
    u16 outer;
    u16 result;

    if (sectorNum > 0xf)
        return 0x80FF;

    SetReadFlash1(buf);

    REG_WAITCNT = (REG_WAITCNT & ~3) | gUnknown_084856A4[0x12];

    n = sectorNum << 5;

    for (outer = 0; outer <= 0x1f; outer++)
    {
        tries = 2;

        while (tries != 0)
        {
            result = sub_0808B430(n);

            if (result == 0)
                break;

            tries--;
        }

        n++;

        if (result != 0)
            break;
    }

    REG_WAITCNT = (REG_WAITCNT & ~3) | 3;

    return result;
}
asm(".global sub_0808B4B4\n.thumb_set sub_0808B4B4, EraseFlashSector_AT\n");

/* Program one flash sector from a caller buffer -- the byte-copy twin of
 * sub_0808B430, which stores 0xFF instead. Same descriptor reads (+0x1c the
 * sector shift, +0x18 the sector size in bytes), same interrupt mask, same
 * unlock sequence, and the same back-up of the cursor to the LAST byte written
 * before the poll helper is handed it. Here BOTH cursors back up, because the
 * value the helper waits for has to be read out of the source buffer rather
 * than being the known 0xFF.
 *
 * NEEDS -O1 -- see data/compiler-overrides.json. The flash library came out of
 * the SDK prebuilt at -O1 (wave 47, W47-F); at -O2 agbcc coalesces the
 * narrowed parameter with `addr` and the redundant `adds r4, r0, #0` the ROM
 * has is gone. */
u16 sub_0808B540(u16 sectorNum, const u8 *src)
{
    const u8 *info;
    u32 addr;
    u16 savedIme;
    u32 n;

    info = (const u8 *)gUnknown_084856A4;

    addr = sectorNum;
    addr <<= info[0x1c];
    addr += 0x0E000000;

    savedIme = REG_IME;
    REG_IME = 0;

    *(volatile u8 *)0x0E005555 = 0xAA;
    *(volatile u8 *)0x0E002AAA = 0x55;
    *(volatile u8 *)0x0E005555 = 0xA0;

    n = *(const u32 *)(info + 0x18);

    while (n != 0)
    {
        *(u8 *)addr = *src;
        src++;
        addr++;
        n--;
    }

    addr--;
    src--;

    REG_IME = savedIme;

    return gUnknown_03005C70(1, (u8 *)addr, *src);
}
