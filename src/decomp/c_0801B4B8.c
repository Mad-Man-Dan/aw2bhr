#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801B4B8.
 * sub_0801B4B8 @ 0x0801B4B8, sub_0801B4BC @ 0x0801B4BC
 */

/* An EMPTY function -- the whole body is one `bx lr`, four bytes, and agbcc
 * emits exactly that for `void f(void) {}` (no prologue, because nothing is
 * saved and nothing is called).
 *
 * void(void) and not void(int): its only caller is DebugBackupUtility_Loop, reached
 * through the forwarder sub_0803AF78, and there r0 holds the `4 & flags` of
 * the `cmp r0, #0; beq` that just branched -- a leftover from the test, not an
 * argument anyone set up. Its twin sub_0801B4BC four bytes below is identical.
 */
void sub_0801B4B8(void)
{
}

/* An EMPTY function -- one `bx lr`, four bytes. See sub_0801B4B8 four bytes
 * above: same body, and the same reason it takes no parameter (DebugBackupUtility_Loop
 * leaves `1 & flags` in r0 at the call through sub_0803AF84, which is the
 * residue of the test that branched there).
 */
void sub_0801B4BC(void)
{
}
