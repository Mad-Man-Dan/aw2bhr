# Wave 93 (W93-C) -- best.c was a spelling the repo had already rejected

Draft unchanged at 140/144 (-4), 51.39%, first difference +0x5.
`best.c` has been renamed `best.c.wrongc`. No probe was spent: the answer was
already written down in the header.

`best.c` declares `extern s8 *gUnknown_081CC4D4;` IN THE .c FILE, which the
wave brief forbids outright, and indexes it instead of the real array
`gUnknown_0202FEF8`. `include/unknown-globals.h` already records the verdict
on exactly that spelling:

    WAVE 38 (W38-E) -- 0x081CC4D4 RE-TESTED AS A REAL POINTER OBJECT AND IT
    IS NOT ONE. [...] Declaring `extern s8 *gUnknown_081CC4D4;` and indexing
    it does NOT replace the indirection -- agbcc applies force-addr to the
    pointer variable as well and emits three levels, `ldr r7,=<pool>;
    ldr r6,[r7]; ldr r0,[r6]`, plus an `R_ARM_ABS32 .rodata` word the ROM
    does not have (53.5%, size exact).

"53.5%, size exact" is best.c's 53.47%, size-exact, to the hundredth. It
reaches the ROM's byte count by adding an indirection level and a data word
the ROM does not have. 0x081CC4D4 is this function's -fforce-addr word
holding &gUnknown_0202FEF8; the ROM's `ldr r7,=<word>; ldr r5,[r7]` is TWO
levels, which is what the draft's honest `gUnknown_0202FEF8[i]` already
emits. The draft is the correct base.

The residual is unchanged and is the wave-38 one: the ROM loads the
force-addr word's address into r7 BEFORE the loop's entry test, so the
loop-skip path reaches the table through that same register and shares the
final store, and the pool word dumps mid-function after the resulting
unconditional branch.

## wave 97

Base: sub_08074AD0.c (51.39%, -4). Not vesly-best.c (its 53.5% is the wave-38 stacked-pointer artefact).
Probed: `for (i = 0; ...; i++)` form -> byte-identical (140 B). Guard-and-early-return form
(`if (dist - (step>>1) < 0) { table[0] = dist; ...; return; }` before the loop) reaches 144 B size-exact but 12.5%: it
moves the `ldr` of the force-addr word to a per-arm position and reallocates `proc` into r3 (ROM keeps it in ip), i.e. it
gives the ROM's size for the wrong reason. A dummy `table[i] = 0` store before the loop: +12, extra frame register.
No lever found in 4 probes; residual is unchanged (the force-addr word address must be live before the guard).
