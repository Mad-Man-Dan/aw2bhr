#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0808AF00.
 * sub_0808AF00 @ 0x0808AF00, sub_0808AF74 @ 0x0808AF74
 */

#include "hardware.h"

/* The relocating trampoline for VerifyFlashSector_Core (the SRAM verify), as
 * ReadFlash is the one for ReadFlash_Core (the SRAM read). It copies a
 * SIBLING FUNCTION'S MACHINE CODE onto the stack and calls it from there so
 * the SRAM access runs out of RAM.
 *
 * MATCHED in Wave 61 -- byte-for-byte, relocations included.
 *
 * REQUIRED BUILD CONFIGURATION (recorded in data/compiler-overrides.json):
 *
 *     cflags_remove: ["-O2", "-fforce-addr"]      cflags_add: ["-O1"]
 *
 * Wave 60 measured only the -O1 half, which gets to 92.2% (9 of 116
 * bytes, size exact) and no further. BOTH halves are needed, and the same pair
 * matches VerifyFlashSectorNBytes and ProgramFlashSector_SST, so the block takes one uniform entry.
 *
 * WHY, precisely -- this is the same residual c_0808AD6C.c documents, and that
 * file's conclusion "no source spelling can [reach the re-load]: the flag
 * decides it" is HALF right. Two independent axes decide it, and the flag alone
 * is not sufficient:
 *
 *   ONE statement    s = (const u16 *)((u32)VerifyFlashSector_Core ^ 1);   -> COPY
 *   TWO statements   s = (const u16 *)VerifyFlashSector_Core;              -> RE-LOAD,
 *                    s = (const u16 *)((u32)s ^ 1);                 but only
 *                                                                   with the
 *                                                                   flag OFF
 *
 * With `-fforce-addr` ON, both spellings emit the copy
 * (`ldr r1,=f; movs r0,#1; adds r3,r1,#0; eors r3,r0`). With it OFF, the
 * two-statement form assigns back to the same variable, the XOR clobbers that
 * pseudo, CSE has nothing live at the second reference, and the address is
 * re-loaded from the SAME pool word -- which is what this ROM does:
 *     ldr r3,=f ; movs r0,#1 ; eors r3,r0 ... ldr r1,=f ; subs r0,r0,r1
 * The one-statement form creates a fresh pseudo, keeps the value live, and
 * emits the copy under either flag. ProgramFlashSector_SST is the same idiom wanting the
 * OTHER answer, and it is one statement there.
 *
 * MEASURED, do not re-run: at -O1, removing `-fforce-addr` changes NOTHING for
 * the one-statement spelling -- the flag is only visible once the source has
 * killed the pseudo. -O2 with the flag removed is 23.3%.
 *
 * ALSO SETTLED: `n = ((u32)A - (u32)B) / 2` and `n = (u16 *)A - (u16 *)B` are
 * byte-identical here (probe, wave 60), so the counter's spelling is not a
 * lever in this family.
 *
 * NOTE the prototype in include/unknown-functions.h was widened from one
 * argument to two in wave 44 on the callee's own evidence (`adds r5, r1, #0`
 * feeding the relocated function's `src`), and src/decomp/c_0801B598.c was
 * updated to forward its second parameter explicitly and re-verified
 * byte-for-byte. That edit is settled and is not part of this note.
 */
int VerifyFlashSector(u16 a1, int a2)
{
    u16 buf[0x80];
    const u16 *s;
    u16 *d;
    u16 n;

    REG_WAITCNT = (REG_WAITCNT & 0xFFFC) | 3;

    s = (const u16 *)VerifyFlashSector_Core;
    s = (const u16 *)((u32)s ^ 1);
    d = buf;
    n = ((u32)VerifyFlashSector - (u32)VerifyFlashSector_Core) / 2;

    while (n != 0)
    {
        *d++ = *s++;
        n--;
    }

    return (int)((u8 *(*)(const u8 *, u8 *, int))((int)buf + 1))(
        (const u8 *)a2,
        (u8 *)((a1 << gUnknown_08485550.unk1c) + 0x0E000000),
        gUnknown_08485550.unk18);
}
asm(".global sub_0808AF00\n.thumb_set sub_0808AF00, VerifyFlashSector\n");

/* VerifyFlashSector with a caller-supplied length instead of
 * gUnknown_08485550.unk18. Same relocating-trampoline idiom.
 *
 * MATCHED in Wave 61 -- byte-for-byte, relocations included.
 *
 * REQUIRED BUILD CONFIGURATION, the same uniform entry recorded in
 * data/compiler-overrides.json:
 *
 *     cflags_remove: ["-O2", "-fforce-addr"]      cflags_add: ["-O1"]
 *
 * The -O1 half alone reaches 92.2% (9 of 116, size exact). Read the long note
 * in work/VerifyFlashSector/VerifyFlashSector.c for the mechanism: the two-statement
 * spelling of the THUMB-bit XOR plus `-fforce-addr` OFF is what makes agbcc
 * RE-LOAD the pool word instead of copying it, and both axes are needed.
 *
 * THE WAVE-44 PARK NOTE'S "+4 BYTES" IS GONE AND ITS EXPLANATION WAS WRONG.
 * It read the +4 as a fifth callee-saved register forced by an entry address
 * hoisted above the copy loop, and called that "evidence FOR that diagnosis".
 * At -O1 there is no hoist and no +4: the function is size exact at 116 bytes
 * before any source change, and the residual was the same 9-byte copy/re-load
 * difference its two siblings had. The hoist was an artefact of -O2.
 *
 * It relocates VerifyFlashSector_Core bounded by VerifyFlashSector -- the same 0x30 bytes
 * VerifyFlashSector copies, NOT a range ending at its own address. That remains the
 * one place the family's pattern breaks: the bound is the next function after
 * the one being COPIED, not the next function after the copier.
 */
int VerifyFlashSectorNBytes(u16 a1, int a2, int a3)
{
    u16 buf[0x80];
    const u16 *s;
    u16 *d;
    u16 n;

    REG_WAITCNT = (REG_WAITCNT & 0xFFFC) | 3;

    s = (const u16 *)VerifyFlashSector_Core;
    s = (const u16 *)((u32)s ^ 1);
    d = buf;
    n = ((u32)VerifyFlashSector - (u32)VerifyFlashSector_Core) / 2;

    while (n != 0)
    {
        *d++ = *s++;
        n--;
    }

    return (int)((u8 *(*)(const u8 *, u8 *, int))((int)buf + 1))(
        (const u8 *)a2,
        (u8 *)((a1 << gUnknown_08485550.unk1c) + 0x0E000000),
        a3);
}
asm(".global sub_0808AF74\n.thumb_set sub_0808AF74, VerifyFlashSectorNBytes\n");
