#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08039750.
 * sub_08039750 @ 0x08039750
 */

#include "proc.h"

/* Family F000 (tools/families.py): `push {lr}; ldr r0,=X; bl S;
 * pop {r0}; bx r0` -- a one-line forwarder. `pop {r0}` is the void epilogue
 * per docs/agbcc-codegen.md, so the callee's result is discarded and this
 * returns nothing. The exemplar is src/decomp/c_080733B8.c.
 */


/* The only Proc_BreakEach in this batch. gUnknown_0849D6D4 is Proc_Start'ed by
 * CoPowerIntro_Init and broken by both CoPowerIntro_FadeInLoop and this, so the script blocks
 * and the pair is break/resume rather than start/end. Proc_BreakEach's
 * parameter is `const struct ProcCmd *` exactly as Proc_EndEach's is.
 */

void CoPowerIntro_BreakPortrait(void)
{
    Proc_BreakEach(gUnknown_0849D6D4);
}
asm(".global sub_08039750\n.thumb_set sub_08039750, CoPowerIntro_BreakPortrait\n");
