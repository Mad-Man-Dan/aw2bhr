#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0806209C.
 * sub_0806209C @ 0x0806209C
 */

/* One setup call and three identical two-argument calls with a literal pair
 * each. The (index, value) pairs are 0/1, 1/6 and 2/5 -- an index that steps
 * and a value that does not, so the second argument is data rather than a
 * count. */
void AiBuildInterestListsForArmy(void)
{
    AiClearInterestLists();
    AiFillInterestList(0, 1);
    AiFillInterestList(1, 6);
    AiFillInterestList(2, 5);
}
asm(".global sub_0806209C\n.thumb_set sub_0806209C, AiBuildInterestListsForArmy\n");
