#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801B6EC.
 * sub_0801B6EC @ 0x0801B6EC, sub_0801B6FC @ 0x0801B6FC
 */

/* Family F024's shape, but the `bl` target is `_call_via_r1` -- the ARMv4T
 * interworking veneer (`bx r1`), which is how gcc/agbcc compiles an INDIRECT
 * call in THUMB, since the GBA has no `blx rN`. There are 40 of these in the
 * tree, so this is ordinary compiler output and nothing here is hand-written.
 *
 * TWO THINGS WERE SETTLED BEFORE DRAFTING, and both were worth the time:
 *
 * 1. The pool word is a genuine LITERAL, not the address of a pointer
 *    variable. `gUnknown_0300619D` exists only because the splitter saw the word
 *    and emitted `. = 0x00619D; gUnknown_0300619D = .;` into aw2bhr.lds; the map
 *    file shows *fill* at that address, i.e. no object is allocated there. So
 *    the value 0x0300619D IS the callee -- an odd address, which is what a
 *    stored THUMB function pointer looks like, pointing into IWRAM.
 *
 * 2. The register is r1 and not r0 because r0 is OCCUPIED. gcc puts the
 *    pointer in the first free scratch register, so `_call_via_r1` is itself
 *    evidence that this is a ONE-ARGUMENT call. There are SIX callers
 *    (MapPreviewPictureScript_Loop, DrawMapPreviewToBg, RenderMapPreviewToVram, WorldMapMapPreview_Init, sub_08086DB4,
 *    BuildMapSelectPreviewNow) and every one passes a word loaded from offset 0x18 of the
 *    struct in its own first parameter -- MapPreviewPictureScript_Loop is `ldr r0,[r2,#0x18]`.
 *    Hence `void *`.
 *
 *    That +0x18 field is written by StartMapPreviewPictureScript, matched this same wave:
 *    `sub_080152EC(gUnknown_0849D41C, 0)->unk18 = arg`. Producer and consumer
 *    agree that the field holds a pointer, which is the discriminating use the
 *    type would otherwise lack -- the parameter is passed straight through
 *    here, so nothing in THIS function's bytes could settle it.
 *
 * A probe of three spellings -- literal cast, `&symbol` cast, and a typedef'd
 * local -- produced the IDENTICAL five-instruction body. They differ only in
 * the pool word, and that difference is real: the literal
 * `((void (*)(void *))0x0300619D)(dst)` emits `.word 0x0300619D` with no
 * relocation, while the ROM's object has `.word 0` plus
 * `R_ARM_ABS32 gUnknown_0300619D`. Same 16 bytes after linking, but trymatch
 * scores it 81.2%, so the `&symbol` spelling is what is used here.
 *
 * That is a spelling forced by the split, not a claim about the original
 * source -- which, since no object exists at that address, must have had a
 * constant or a macro. Note this is NOT the "different symbols that resolve to
 * the same address" false mismatch docs/agbcc-codegen.md describes; it is
 * relocation-vs-no-relocation, which that check does not cover. */
void sub_0801B6EC(void *dst)
{
    ((void (*)(void *))&gUnknown_0300619D)(dst);
}

/* Family F024's shape, but the `bl` target is `_call_via_r1` -- the ARMv4T
 * interworking veneer (`bx r1`), which is how gcc/agbcc compiles an INDIRECT
 * call in THUMB, since the GBA has no `blx rN`. There are 40 of these in the
 * tree, so this is ordinary compiler output and nothing here is hand-written.
 *
 * TWO THINGS WERE SETTLED BEFORE DRAFTING, and both were worth the time:
 *
 * 1. The pool word is a genuine LITERAL, not the address of a pointer
 *    variable. `gUnknown_03006511` exists only because the splitter saw the word
 *    and emitted `. = 0x006511; gUnknown_03006511 = .;` into aw2bhr.lds; the map
 *    file shows *fill* at that address, i.e. no object is allocated there. So
 *    the value 0x03006511 IS the callee -- an odd address, which is what a
 *    stored THUMB function pointer looks like, pointing into IWRAM.
 *
 * 2. The register is r1 and not r0 because r0 is OCCUPIED. gcc puts the
 *    pointer in the first free scratch register, so `_call_via_r1` is itself
 *    evidence that this is a ONE-ARGUMENT call. There are SIX callers
 *    (MapPreviewPictureScript_Loop, DrawMapPreviewToBg, RenderMapPreviewToVram, WorldMapMapPreview_Init, sub_08086DD4,
 *    BuildMapSelectPreviewNow) and every one passes a word loaded from offset 0x18 of the
 *    struct in its own first parameter -- MapPreviewPictureScript_Loop is `ldr r0,[r2,#0x18]`.
 *    Hence `void *`.
 *
 *    That +0x18 field is written by StartMapPreviewPictureScript, matched this same wave:
 *    `sub_080152EC(gUnknown_0849D41C, 0)->unk18 = arg`. Producer and consumer
 *    agree that the field holds a pointer, which is the discriminating use the
 *    type would otherwise lack -- the parameter is passed straight through
 *    here, so nothing in THIS function's bytes could settle it.
 *
 * A probe of three spellings -- literal cast, `&symbol` cast, and a typedef'd
 * local -- produced the IDENTICAL five-instruction body. They differ only in
 * the pool word, and that difference is real: the literal
 * `((void (*)(void *))0x03006511)(dst)` emits `.word 0x03006511` with no
 * relocation, while the ROM's object has `.word 0` plus
 * `R_ARM_ABS32 gUnknown_03006511`. Same 16 bytes after linking, but trymatch
 * scores it 81.2%, so the `&symbol` spelling is what is used here.
 *
 * That is a spelling forced by the split, not a claim about the original
 * source -- which, since no object exists at that address, must have had a
 * constant or a macro. Note this is NOT the "different symbols that resolve to
 * the same address" false mismatch docs/agbcc-codegen.md describes; it is
 * relocation-vs-no-relocation, which that check does not cover. */
void sub_0801B6FC(void *dst)
{
    ((void (*)(void *))&gUnknown_03006511)(dst);
}
