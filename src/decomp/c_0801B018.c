#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801B018.
 * sub_0801B018 @ 0x0801B018, sub_0801B09C @ 0x0801B09C
 */

/* The validator for the 0x1000-byte flash staging buffer gUnknown_02002000:
 * a four-byte magic ("2ars"), a 0x55/0xaa pair that must be complementary
 * across the two ends of the block, a byte checksum with its own complement,
 * and a constant 0xf. Returns 0 only when all of them hold, so the sense is
 * "rejected", not "ok".
 *
 * The header is a CAST VIEW of the byte array rather than the array's declared
 * type, because the same object is also summed a byte at a time below. That is
 * not a workaround for anything: 0x0808EF60, which asm/ shows in this pool as
 * `gUnknown_0808EF60`, is a word in data/rodata.s containing 0x02002000 --
 * agbcc's own -fforce-addr address-constant slot for this array -- so
 * `ldr r0, =...; ldr r3, [r0]` is just how &gUnknown_02002000 is materialised
 * here, and the second `ldr r3, [r5]` after the loop is that slot being re-read
 * once the loop has clobbered r3. Declaring gUnknown_0808EF60 as a pointer
 * VARIABLE instead adds a third load at both points; see the note in
 * include/unknown-globals.h.
 *
 * The promotion needs "rodata": ["0x0808EF60"].
 *
 * `sum` is `u8`, which is what the `lsls #0x18; lsrs #0x18` in the loop body
 * is -- the accumulator truncating, not a cast at the use. */

struct Unk02002000
{
    /* 0x0000 */ u32 magic;
    /* 0x0004 */ u8 unk04;
    /* 0x0005 */ u8 unk05;
    /* 0x0006 */ u8 unk06;
    /* 0x0007 */ u8 unk07;
    /* 0x0008 */ u8 filler_08[0xff7];
    /* 0x0fff */ u8 unk0fff;
};
#define HDR ((struct Unk02002000 *)gUnknown_02002000)

/* Load slot `a` from flash into the staging buffer and validate it, retrying up
 * to four times. Three distinct results: 4 when the transfer succeeded but
 * IsSaveSectorBufferInvalid rejected the contents, 0 when it accepted them, and 1 when all
 * four transfer attempts failed.
 *
 * Only ONE member of gUnknown_0200CC38 is touched, unk20, even though asm/
 * prints two different symbols for it. The retry loop's accesses are hoisted by
 * LICM (`a` is loop-invariant), which folds the base and the member offset into
 * the single address constant 0x0200CC58; the access after the loop is not in a
 * loop, so -fforce-addr leaves `base + 0x20` as a runtime add. Same member, two
 * materialisations -- see the struct's note in include/unknown-globals.h.
 *
 * The two casts are deliberate and byte-neutral. ReadFlashIfPresent and VerifyFlashSectorIfPresent
 * are promoted with `int` where this passes a buffer, and SetFlashTimerIntrIfPresent with
 * `void (**)(void)` where gUnknown_0200CC34 is declared `int`; a promoted
 * definition wins over anything decided at a call site, so the call sites
 * convert rather than the declarations moving. Both prototypes look like they
 * WANT retyping -- ReadFlashIfPresent's third parameter is a pointer at every known
 * call, and gUnknown_0200CC34 is what SetFlashTimerIntr writes a callback through --
 * but that is a separate change with its own re-verification and it was not
 * made here. */
int ReadAndValidateSaveSector(u16 a)
{
    int i;

    SetFlashTimerIntrIfPresent(gUnknown_0200CC30, (void (**)(void))gUnknown_0200CC34);

    for (i = 0; i < 4; i++)
    {
        ReadFlashIfPresent(a, 0, (int)gUnknown_02002000, 0x1000);

        if (VerifyFlashSectorIfPresent(a, (int)gUnknown_02002000) == 0)
        {
            if (IsSaveSectorBufferInvalid() != 0)
            {
                gUnknown_0200CC38.unk20[a] |= 4;
                return 4;
            }

            gUnknown_0200CC38.unk20[a] &= 0xfa;
            return 0;
        }
    }

    gUnknown_0200CC38.unk20[a] |= 1;
    return 1;
}
asm(".global sub_0801B018\n.thumb_set sub_0801B018, ReadAndValidateSaveSector\n");

int IsSaveSectorBufferInvalid(void)
{
    u8 sum;
    int i;

    if (HDR->magic != 0x73726132)
        return 1;

    if (HDR->unk04 == 0x55)
    {
        if (HDR->unk0fff != 0xaa)
            return 1;
    }
    else if (HDR->unk04 == 0xaa)
    {
        if (HDR->unk0fff != 0x55)
            return 1;
    }
    else
    {
        return 1;
    }

    sum = 0;

    for (i = 0; i < 0x1000; i++)
        sum += gUnknown_02002000[i];

    if (HDR->unk06 != sum)
        return 1;

    if (HDR->unk07 != (u8)~sum)
        return 1;

    if (HDR->unk05 != 0xf)
        return 1;

    return 0;
}
asm(".global sub_0801B09C\n.thumb_set sub_0801B09C, IsSaveSectorBufferInvalid\n");
