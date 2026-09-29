#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08045C18.
 * sub_08045C18 @ 0x08045C18
 */

/* MATCHED wave 73 (W73-F) by decomp-permuter, after four waves of hand attempts
 * and three chained permuter runs had all stalled at 89.4%.
 *
 * WHAT ACTUALLY FOUND IT: running the permuter with --current, from the DRAFT,
 * instead of chaining from best.c. best.c held a 97.1% variant that is
 * SEMANTICALLY WRONG (it reassigns the index local from a loaded byte in test 7
 * and reuses it as test 10's index), and three runs chained from it plateaued
 * there. The draft was a different basin: base score 110 -> 15 -> 0 and a
 * byte-for-byte match inside one 600 s run. On a size-exact target, check
 * work/<fn>/best.json before chaining -- trymatch moves best.c forward on
 * byte-identity percentage alone, so it can be owned by a variant you would
 * never adopt.
 *
 * DO NOT TIDY. `new_var` is the whole edit. It binds the COMPARISON CONSTANT
 * 0x40 at test 4 and reuses it for tests 5-10, which is semantically exact --
 * tests 5-10 are only reached after test 4's condition has been evaluated, so
 * new_var is 0x40 at every use. That extra allocno is what inverts the two
 * registers the ROM and every earlier draft disagreed on:
 *
 *     ROM    terrain = r3   const 0xe0 = r2
 *     old    terrain = r2   const 0xe0 = r3
 *
 * The position matters as much as the binding: bound at test 1, or declared
 * with an initialiser, CSE const-propagates it away and the pseudo vanishes.
 *
 * The wave-45 conclusion that the allocno-priority model here was "INCOMPLETE,
 * not mis-parameterised", with arithmetic showing the ROM needed terrain at
 * <= 10 refs or the constant at >= 11, "both a whole test away", is REFUTED.
 * One extra allocno flips it.
 *
 * AXES MEASURED AND NEGATIVE, kept so nobody re-runs them:
 *   - `return A && B && ...;` as one expression: builds an r5 accumulator and
 *     `push {r4,r5,lr}`. Wrong shape.
 *   - the row offsets as declared locals: 81.25%, puts them in a low register
 *     and loses ip.
 *   - `(x & 0xe0) == 0x40` as the FIRST test: 14.1%. The first test must carry
 *     the mask AND the shift or the constant is not materialised in the first
 *     basic block at all.
 *   - reordering the AND operands (`0xe0 & x`): byte-neutral.
 *   - `int mask = 0xe0;` bound before the tests: moves the `movs` into the
 *     prologue. Naming the MASK anywhere, including through a comma inside the
 *     first test, costs 16 bytes -- see the note below.
 *   - naming the LOADED BYTE: 192 bytes (-16) for all ten, 200 (-8) for tests
 *     6-10. Naming the load or the mask lets combine drop the nine
 *     `adds r0, r2, #0` copies the ROM spends, because THUMB `ands` is
 *     two-operand and only spends a copy when both its inputs are anonymous.
 *     The comparison constant is the one operand of the ten tests that is not
 *     an input to the `ands`, which is why it is the one that could be named.
 *   - naming the INDEX constant 0x19: 89.4%, CSE const-propagates it.
 *   - all seven compiler profiles: configured/default/no-force 89.4%; o1 and
 *     o1-no-force 4.3%; old-agbcc and old-agbcc-no-force 35.6%.
 *   - W73-E's three pointer-global spellings: all three TIE at 89.4%.
 *
 * 0x417A and 0x417E are rowOffset[0] and rowOffset[2]. */
int MapEventCond_Army2OwnsTenCellsX21To25Y0AndY2(void)
{
  unsigned short new_var;
  struct Map *m = gMap;
  if (((m->terrain[m->rowOffset[0] + 0x15] & 0xe0) >> 5) != 2)
  {
    return 0;
  }
  if ((m->terrain[m->rowOffset[0] + 0x16] & 0xe0) != 0x40)
  {
    return 0;
  }
  if ((m->terrain[m->rowOffset[0] + 0x17] & 0xe0) != 0x40)
  {
    return 0;
  }
  if ((m->terrain[m->rowOffset[0] + 0x18] & 0xe0) != (new_var = 0x40))
  {
    return 0;
  }
  if ((m->terrain[m->rowOffset[0] + 0x19] & 0xe0) != new_var)
  {
    return 0;
  }
  if ((m->terrain[m->rowOffset[2] + 0x15] & 0xe0) != new_var)
  {
    return 0;
  }
  if ((m->terrain[m->rowOffset[2] + 0x16] & 0xe0) != new_var)
  {
    return 0;
  }
  if ((m->terrain[m->rowOffset[2] + 0x17] & 0xe0) != new_var)
  {
    return 0;
  }
  if ((m->terrain[m->rowOffset[2] + 0x18] & 0xe0) != new_var)
  {
    return 0;
  }
  if ((m->terrain[m->rowOffset[2] + 0x19] & 0xe0) != new_var)
  {
    return 0;
  }
  return 1;
}
asm(".global sub_08045C18\n.thumb_set sub_08045C18, MapEventCond_Army2OwnsTenCellsX21To25Y0AndY2\n");
