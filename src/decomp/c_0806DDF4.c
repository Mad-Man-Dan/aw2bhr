#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0806DDF4.
 * sub_0806DDF4 @ 0x0806DDF4
 */

/* Two things here look like decoration and are both load-bearing.
 *
 * `pp = &gUnknown_0816E1B8` is the -fforce-addr escape. The global is read
 * inside the loop and again after it, which is the "used before and after a
 * loop" trigger, and every naive spelling routes the symbol through a
 * synthesised .rodata word -- one extra `ldr` at each access and a pool
 * relocation against .rodata rather than against the symbol. Taking the address
 * is not a "use", so reaching the tail through pp suppresses it. The global is
 * `const` for a separate reason: that is what lets LICM hoist the load into the
 * preheader while leaving the `->unk00` deref inside the loop, which is exactly
 * what the ROM does.
 *
 * The `do { ... } while (0)` is the register allocation. Without it the record
 * pointer lands in r3 and the stored zero in r4; the ROM has them the other way
 * round, and that is the whole 4-byte diff. A bare nested block does NOT do it,
 * and neither does an embedded assignment, a comma expression, or hoisting the
 * assignment out of the loop -- probed side by side, all of those give the
 * r3/r4 order. Presumably the original wrote the body through a do/while(0)
 * macro. See docs/agbcc-codegen.md.
 */
void RulesScreenHighlightSelectedRuleOption(void)
{
    struct Unk0816E1B8 *const *pp;
    struct Unk08580934 *rec;
    int i;

    i = 0;
    pp = &gUnknown_0816E1B8;

    for (; i <= 6; i++)
    {
        rec = gUnknown_0816E1B8->unk00;
        do { rec->unk54[i]->unk46 = 0; } while (0);
    }

    (*pp)->unk00->unk54[(*pp)->unk00->unk33]->unk46 = 1;
}
asm(".global sub_0806DDF4\n.thumb_set sub_0806DDF4, RulesScreenHighlightSelectedRuleOption\n");
