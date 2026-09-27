# sub_08039588

## Wave 93 (W93-D) -- the hoist ORDER is solved; only the constant merge is left

Reading the table entry into a local as the FIRST statement of the search loop
body fixes the preheader ORDER, which was half of residual (a):

    for (k = 0; gUnknown_08090F30[k] != 0; k++) {
        c = gUnknown_08090F30[k];
        dst = j * 0x100 + 0x6140;
        if (str[i] == c) { ... break; }
    }

With `c` first the preheader comes out `ldr r3, =gUnknown_08090F30` THEN the
`j << 8` computation -- the ROM's order. Without it the two are reversed. The
leading read costs nothing, because the ROM loads tbl[k] into a register there
anyway. GENERAL LEVER, worth reusing: LICM emits its hoists in the order the
invariants appear in the loop body, so a leading reference to the value you
want hoisted FIRST puts it first.

What is left is only the constant merge: the preheader gets `lsl r4, r4, #8`
and the `+ 0x6140` is folded into the use as a single `=0x6016140` pool word.
164 bytes (-8) at 43.0%.

`-fno-cse-follow-jumps` does NOT prevent the merge (measured on this spelling:
43.0%, -8, unchanged). That is expected in hindsight: gcc lays the if-body out
as the FALL-THROUGH of the inverted compare, so the def and the use sit on one
cse path without any jump being followed. Breaking it needs a JOIN -- a label
with two or more predecessors -- between the def and the use inside the loop,
and no C construct that survives the `jump` pass creates one here.

The impasse is now exactly stated:
  * the def must be INSIDE the inner loop and BEFORE any conditional branch in
    the body, or LICM will not hoist it. Everything after the `if` is
    `maybe_never`, which is why the bottom-of-body spelling keeps both
    constants but never moves.
  * the def must be OUT of the use's fall-through path, or cse reassociates
    0x6140 with 0x06010000.
Every position in the body satisfies exactly one of the two.

Draft unchanged: size-exact 172 bytes, 87.2%.
