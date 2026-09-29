#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08067504.
 * sub_08067504 @ 0x08067504, sub_0806752C @ 0x0806752C
 */

#include "proc.h"

/* Proc_BreakEach open-coded over proc.c's sProcArray instead of walking the
 * proc tree: all 32 slots at stride 0x6c, selecting on proc_script and calling
 * Proc_Break. Proc_EndAllExceptScript next door is the same scan with the guard sense
 * flipped and Proc_End as the action. sub_0806A444 is the only caller.
 *
 * sProcArray is NOT static in src/proc.c -- see include/proc.h, where it is now
 * declared next to gProcTreeRootArray, which has had the identical
 * EWRAM_DATA-definition/plain-extern-declaration pair since wave 36. Nothing in
 * src/proc.c is touched.
 *
 * The loop is written ascending even though the ROM counts r5 down from 0x1f:
 * `check_dbra_loop` turns the ascending source counter into the decrementing
 * one, which is why the pointer still advances (`adds r4, #0x6c`) while the
 * counter descends. proc.c's own Proc_Find/Proc_Count/Proc_FindUnblocked are
 * spelled exactly this way and are upstream's matching source. */
void Proc_BreakEachOpenCoded(const struct ProcCmd * script)
{
    s32 i;

    struct Proc * it = sProcArray;

    for (i = 0; i < 32; i++, it++)
    {
        if (it->proc_script == script)
        {
            Proc_Break(it);
        }
    }
}
asm(".global sub_08067504\n.thumb_set sub_08067504, Proc_BreakEachOpenCoded\n");

/* The twin of Proc_BreakEachOpenCoded: the same open-coded scan of proc.c's sProcArray,
 * but the guard sense is INVERTED and the action is Proc_End. The ROM's `beq`
 * skips the call, so this ends every proc whose script is NOT the argument --
 * an "end everything except" sweep, not a Proc_EndEach. Free slots have a NULL
 * proc_script and so are swept too; Proc_End on an already-dead slot is a
 * no-op, which is presumably why the original could get away with it. */
void Proc_EndAllExceptScript(const struct ProcCmd * script)
{
    s32 i;

    struct Proc * it = sProcArray;

    for (i = 0; i < 32; i++, it++)
    {
        if (it->proc_script != script)
        {
            Proc_End(it);
        }
    }
}
asm(".global sub_0806752C\n.thumb_set sub_0806752C, Proc_EndAllExceptScript\n");
