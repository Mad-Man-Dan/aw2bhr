#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08071ADC.
 * sub_08071ADC @ 0x08071ADC, sub_08071AF0 @ 0x08071AF0, sub_08071B0C @ 0x08071B0C
 */

/* The +0x58 slot is a whole word (`ldr`) and the `lsls #0x10; lsrs #0x10`
 * after it is the conversion to StartCoSpeechScript's declared `u16` first
 * parameter, not a field width -- the narrowing sits at the use, which is
 * docs/agbcc-codegen.md's copy-then-narrow readout. */
struct Unk8071ADC
{
    /* 0x00 */ u8 filler_00[0x58];
    /* 0x58 */ u32 unk58;
};
#include "proc.h"

void sub_08071ADC(struct Unk8071ADC *proc)
{
    StartCoSpeechScript(proc->unk58, 0, 0);
}

/* A front end onto StartPalFade that adds only the palette. See the
 * StartPalFade note in include/unknown-functions.h: the first argument is ROM
 * PALETTE DATA, not the proc script -- the script is hard-coded in
 * StartPalFade's own pool. This function's own three parameters are
 * (index, b, parent) and it forwards all three untouched; the bare prologue
 * `adds r4,r0,#0; adds r5,r1,#0; adds r3,r2,#0` narrows nothing, so the first
 * two are wide, and `pop {r4,r5}; pop {r0}` makes it void.
 *
 * StartPalFadeToWhite is the same function over gUnknown_08613F74. */
void StartPalFadeToBlack(int index, int b, ProcPtr parent)
{
    StartPalFade(gUnknown_08613F54, index, b, parent);
}
asm(".global sub_08071AF0\n.thumb_set sub_08071AF0, StartPalFadeToBlack\n");

/* StartPalFadeToBlack's twin over the other palette -- byte-identical apart from the
 * pool symbol, so one derivation gives both, but each member's own data_refs
 * were re-read before transcribing. See StartPalFadeToBlack and the StartPalFade
 * note in include/unknown-functions.h. */
void StartPalFadeToWhite(int index, int b, ProcPtr parent)
{
    StartPalFade(gUnknown_08613F74, index, b, parent);
}
asm(".global sub_08071B0C\n.thumb_set sub_08071B0C, StartPalFadeToWhite\n");
