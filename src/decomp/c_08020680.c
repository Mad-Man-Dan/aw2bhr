#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08020680.
 * sub_08020680 @ 0x08020680
 */

/* Skips a byte-coded command stream until the terminator 4 -- the same
 * terminator RevertMovementScript appends just above.  Nothing calls it and nothing in
 * the ROM holds its address; r0 and r1 are never read, so both leading
 * parameters are dead, and so is everything the four non-terminating cases
 * once did.
 *
 * The five-entry jump table is the load-bearing detail and it is what forces
 * the dead local.  agbcc only emits a tablejump when the case nodes are still
 * distinct when the switch is expanded, and `group_case_nodes` merges adjacent
 * cases that share a body -- so `case 0: case 1: case 2: case 3: break;`
 * collapses to one node, drops below the tablejump threshold, and comes out as
 * three compares (`blt`/`ble`/`bne`) instead.  Giving each case a body of its
 * own keeps four nodes alive through expansion; `flow` then deletes the dead
 * stores and cross-jumps the four arms together, which is why every one of the
 * first four table entries points at the loop head.  The values themselves are
 * not recoverable -- only the fact that the four cases were written out
 * separately is. */
void ScanMovementScriptToEnd(int a, int b, u8 *p)
{
    int x;

    x = 0;
    for (;;)
    {
        switch (*p++)
        {
        case 0:
            x = 1;
            break;
        case 1:
            x = 2;
            break;
        case 2:
            x = 3;
            break;
        case 3:
            x = 4;
            break;
        case 4:
            return;
        }
    }
}
asm(".global sub_08020680\n.thumb_set sub_08020680, ScanMovementScriptToEnd\n");
