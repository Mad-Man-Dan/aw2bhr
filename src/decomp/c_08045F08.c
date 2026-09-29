#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08045F08.
 * sub_08045F08 @ 0x08045F08, sub_08045F18 @ 0x08045F18
 */

/* Family F000 (tools/families.py): `push {lr}; ldr r0,=X; bl S;
 * pop {r0}; bx r0` -- a one-line forwarder. `pop {r0}` is the void epilogue
 * per docs/agbcc-codegen.md, so the callee's result is discarded and this
 * returns nothing. The exemplar is src/decomp/c_080733B8.c.
 */

/* The sibling of sub_0803F540 on PlayMusic, and the same caveat: 0x19D is
 * a pool constant, so this call site cannot discriminate the parameter width.
 */

void PlayMusic19D(void)
{
    PlayMusic(0x19D);
}
asm(".global sub_08045F08\n.thumb_set sub_08045F08, PlayMusic19D\n");

/* Family F003 (data/families.json): 34 twelve-byte forwarders shaped
 * `push {lr}; movs r0,#K; bl S; pop {r0}; bx r0`, i.e. one call with one
 * literal argument and nothing else. `pop {r0}; bx r0` fixes this as void.
 * The argument's C type is not recoverable from here -- `movs r0,#K` is the
 * same two bytes for int/u32/u16/s16/u8 -- so the spelling follows the
 * callee's declaration in unknown-functions.h and nothing else.
 * Exemplar: src/decomp/c_08004A60.c. */

void SetMusicVolumeC0(void)
{
    SetMusicVolume(0xC0);
}
asm(".global sub_08045F18\n.thumb_set sub_08045F18, SetMusicVolumeC0\n");
