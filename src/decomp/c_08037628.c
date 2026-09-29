#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08037628.
 * sub_08037628 @ 0x08037628
 */

/* Family F000 (tools/families.py): `push {lr}; ldr r0,=X; bl S;
 * pop {r0}; bx r0` -- a one-line forwarder. `pop {r0}` is the void epilogue
 * per docs/agbcc-codegen.md, so the callee's result is discarded and this
 * returns nothing. The exemplar is src/decomp/c_080733B8.c.
 */


/* gUnknown_03001470 list, not a proc: StartMapPreviewPictureScript starts it with
 * `sub_080152EC(gUnknown_0849D41C, 0)` and stashes a word at +0x18 of the slot
 * it gets back; this stops it. sub_080152EC's parameter is already
 * `const void *`, which is where sub_0801537C's comes from.
 */

void EndMapPreviewPictureScript(void)
{
    sub_0801537C(gUnknown_0849D41C);
}
asm(".global sub_08037628\n.thumb_set sub_08037628, EndMapPreviewPictureScript\n");
