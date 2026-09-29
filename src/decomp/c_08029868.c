#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08029868.
 * sub_08029868 @ 0x08029868
 */

/* The two compares are a SWITCH RANGE CHECK, not two `if`s.
 * `case 1: case 2:` with one body group into a single case node with
 * low=1, high=2, and stmt.c's emit_case_nodes emits the bounds test
 * directly as RTL -- `cmp #2; bgt default` then `cmp #1; blt default` --
 * with the LITERAL constants. That path never goes through tree-level
 * fold, which is what canonicalises `v >= 1` to `v > 0` (`cmp #0`) and
 * `v < 1` to `v <= 0` and defeated every `if` spelling tried in wave 31.
 * The signed `bgt`/`blt` come from the index's default promotion to int. */
void sub_08029868(u8 a)
{
    switch (a)
    {
    case 1:
    case 2:
        PlayMusicOrSfx2(0x50);
        break;
    }
}
