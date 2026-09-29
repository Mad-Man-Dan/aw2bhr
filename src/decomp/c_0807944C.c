#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0807944C.
 * sub_0807944C @ 0x0807944C
 */

void ResultsScreen_LoadCoFullBody(void)
{
    LoadCoFullBodyAndPalette(gPlayers[GetResultsArmy()].co, 0, 11);
}
asm(".global sub_0807944C\n.thumb_set sub_0807944C, ResultsScreen_LoadCoFullBody\n");
