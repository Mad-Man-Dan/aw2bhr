# sub_0801A718 — wave 92 (W92-B)

Draft unchanged at **68.94%, size-exact (132/132)**. The linker symbol this
function waited five waves for now exists and is in the draft; what is left is
register allocation. Seven spellings measured this wave, all negative, in two
compile runs.

## The sentinel symbol landed and it was worth 10 points

`aw2bhr.lds` now has `. = 0x00C618; gUnknown_0200C618 = .;` and
`include/unknown-globals.h` declares `extern struct Unk0808E5C8
gUnknown_0200C618;`. The draft's tail reads `gUnknown_030020A8.unk04 =
gUnknown_0200C618.unk04;` and the function went from 59.09% to 68.94%,
size-exact. Confirmed here by re-measuring the pre-symbol spelling
(`gUnknown_0808E5D0->unk04`) side by side: 59.09%, same size. The symbol is the
right fix and the old pointer-word workaround should not come back.

## Going through the ROM pointer words is worse, not better — measured

0x0808E5C8, 0x0808E5CC and 0x0808E5D0 are three consecutive ROM words holding
0x0200C618, 0x030020A8 and 0x0200C618, and this function's own literal pool
holds 0x0808E5CC (at +0x1C) and 0x0808E5D0 (at +0x48). That looks like an
invitation to name those words and read through them, and the candidate's
`R_ARM_ABS32 .rodata` at +0x1C where the ROM has `R_ARM_ABS32 gUnknown_0808E5CC`
looks like the difference. It is not. Both were tried:

    read gUnknown_030020A8 through gUnknown_0808E5CC ......  13.97%, size +4
    that plus the sentinel through gUnknown_0808E5D0 ......  14.29%, size +8
    sentinel through gUnknown_0808E5D0 only ...............  59.09%, size-exact

Naming a ROM address word and dereferencing it makes agbcc add its own
force-addr level *on top*, one load and four bytes per word. This generalises
wave 58's result from gUnknown_0808E5D0 to gUnknown_0808E5CC: no declaration of
one of these words reaches the ROM's load count, only naming the RAM object
does. Read the other way, the three words at 0x0808E5C8/CC/D0 behave exactly
like the address-constant cells agbcc emits for a multi-block reference, which
is what the header note already says; the candidate emitting its own `.rodata`
cell there is the honest spelling, and per the standing wave-18 rule the
promotion carries it.

## The `cur = base - 1` spelling axis is folded — four spellings, one probe

The ROM derives the list's head sentinel from the register that already holds
the node array's base, copying first:

    ROM     ldr r0, [pc, #16] / adds r2, r1, r0 / adds r3, r0, #0 / subs r3, #12
    draft   ldr r2, [pc, #28] / ... / adds r3, r0, r2 / subs r2, #12

so the ROM spends a copy the draft does not, and the draft loads the base
earlier. Four spellings were compiled to try to force the copy:

    base bound to a local, node and cur both derived from it .... byte-identical
    base bound to a local, cur still from the plain global ...... byte-identical
    cur = &gUnknown_0200C624[-1] ............................... byte-identical
    cur = (struct Unk0808E5C8 *)((u8 *)gUnknown_0200C624 - 12) .. byte-identical

All four are the current draft's 68.94% to the byte. Whether the base and `cur`
share a register is decided after the source is gone; it is not reachable by
rebinding or by respelling the subtraction. Together with the two already in the
parked entry, the spelling axis here is six deep and closed.

## Residual

Size-exact, 41 of 132 bytes differ, first difference at +0x2 — and that first
difference is a register number: the ROM keeps the first parameter in r3, the
draft in r5. Everything downstream follows from that and from the base/cur
sharing above. This is a pure allocation residual on a size-exact function,
which is the permuter's documented case (wave 59's repeated-run recipe). No
permuter run has been made on this function; it is the obvious next step and was
not started only because two runs were already occupying this agent's slots.

## Pool words owned

    +0x1C -> the address cell for gUnknown_030020A8 (ROM: 0x0808E5CC)
    +0x44 -> gUnknown_0200C624
    +0x48 -> the address cell for gUnknown_0200C618 (ROM: 0x0808E5D0)
