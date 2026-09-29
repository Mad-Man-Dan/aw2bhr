#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803CD2C.
 * sub_0803CD2C @ 0x0803CD2C
 */

#include "hardware.h"

/* PARKED at 144/144 bytes, 93.8% identical (9 bytes). Wave 35, W35-E.
 *
 * SETTLED HERE, and the first two are prototype corrections now in
 * include/unknown-functions.h:
 *   - the first parameter is `u16`, not `int`. The ROM narrows it
 *     `lsls #0x10; lsrs #0x10` BEFORE it touches the second parameter, which is
 *     entry-order parameter conversion; an `int` with a `u16` local assigned as
 *     the first statement puts the narrowing after `lsls r1,r1,#0x18` instead.
 *     Its one caller (src/decomp/c_080328EC.c) passes the constant 0x200.
 *   - SetLoadedMapBlob takes `void *`. It was declared `void (void)` while its own
 *     promoted definition in src/decomp/c_08037B84.c has taken a pointer since
 *     it landed; this function and DrawDesignRoomSlotPreview / DrawDesignRoomMapPreview all materialise
 *     the argument. Fixing it is what let DrawDesignRoomSlotPreview and DrawDesignRoomMapPreview match.
 *   - `p = gUnknown_02000000;` bound before the guard, not named at each call:
 *     the ROM loads that pool word ABOVE the `bne`, which no in-expression
 *     spelling reproduces. The same lever matched DrawDesignRoomSlotPreview and DrawDesignRoomMapPreview.
 *
 * THE WHOLE REMAINING DIFF IS THE SAVE/RESTORE PAIR AROUND DrawMapPreviewToBg, and it
 * is which of the two values gets the high register:
 *
 *   ROM:  ldr r0,=gUnknown_03001418 ; mov r8, r0 ; ldrh r5,[r0]
 *         ldr r6,=gUnknown_03001FF8 ; ldrh r4,[r6]
 *         ... mov r0, r8 ; strh r5,[r0] ; strh r4,[r6]
 *   here: ldr r6,=gUnknown_03001418 ; ldrh r0,[r6] ; mov r8, r0
 *         ldr r5,=gUnknown_03001FF8 ; ldrh r4,[r5]
 *         ... mov r0, r8 ; strh r0,[r6] ; strh r4,[r5]
 *
 * The ROM parks the ADDRESS of gUnknown_03001418 in r8 and its saved VALUE in
 * r5; agbcc parks the value in r8 and the address in r6. Identical instruction
 * count and order, four live values either way.
 *
 * RULED OUT with try_match and compile_probe:
 *   - declaring the locals in reverse order (u8 *p; u16 s2; u16 s1;). This is
 *     the lever that fixed UnitClassInfo_DrawSelectionBrackets's register pair this wave, and it does
 *     nothing here -- so it moves LOCALS, not compiler-created address pseudos.
 *   - u16 *g1 = &gUnknown_03001418; s1 = *g1; ... *g1 = s1;  is byte-identical
 *     to this draft, so the address pseudo already exists and the choice is
 *     downstream of naming it.
 *
 * NEXT AXIS: gUnknown_03001FF8 is `volatile u16` and gUnknown_03001418 is not,
 * yet the ROM treats the NON-volatile one as the address-in-a-register case,
 * which is the opposite of the intuition. Try `volatile` on gUnknown_03001418 --
 * and check its other readers before believing a match that comes out of it,
 * since a wrong `volatile` is byte-neutral almost everywhere else.
 */
u8 DrawDesignRoomSlotPreviewBg1(u16 a1, u8 a2)
{
    u16 s1;
    u16 s2;
    u8 *p;

    p = gUnknown_02000000;
    if (IsSaveSlotInvalid(a2 + 5) != 0)
        return 0;
    ReadSaveSlot(a2 + 5, p);
    SetLoadedMapBlob(p);
    SnapshotTeamColorsFromRecord((struct Unk3D6FC *)p);
    s1 = gUnknown_03001418;
    s2 = gUnknown_03001FF8;
    DrawMapPreviewToBg((void *)(0x06000000 + gUnknown_03001FE8.bits.chr_block * 0x4000),
                 a1, 1, 0, 0, 5);
    gUnknown_03001418 = s1;
    gUnknown_03001FF8 = s2;
    return 1;
}
asm(".global sub_0803CD2C\n.thumb_set sub_0803CD2C, DrawDesignRoomSlotPreviewBg1\n");
