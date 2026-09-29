
## wave 95

Base: the old draft (kept as `sub_0804CA98.w95-start.c`, 17.1%, size +0, first difference +0xa). Left as the draft.

Pre-registered hypothesis (force-addr word appears when the address is an address VALUE with a variable term, as in c_0804EB78 / c_0804F3C8): REFUTED for this function. The old draft already had the products-plus-`(u8 *)` spelling in its tail, and this wave's rewrite of the whole guarded region in the exemplar spelling (entry pointer `e = a2*sz + a1*grp + (u8 *)g` at the top, `dst` and the two `gUnknown_08552148` entries in the same spelling, `int`-free u16 reads, group stride bound to a `unsigned short` local, tail also in `(u8 *)` spelling, or with the tail by name) produced size -8 / 17.5% (tail `(u8 *)`) or -12 / 14.9% (tail by name). Inlining the entry as a macro instead of a local is byte-identical to the local. In every one of these the `.rodata` word appears for gUnknown_02029B94 only (that is new: the earlier draft had neither); gUnknown_02029A10 stays a plain literal loaded four times.
So the same source shape as c_0804EB78 does not create the word here. Differences between the two: EB78 has a call and other globals' words before the entry, and its entry is used for reads and stores in ONE straight run; this function tests `unk00` and branches to a tail. Unresolved which of those matters.
The `-da` dumps show three surviving `(set P (symbol_ref .LC0))` after cse, which gcse leaves un-unified; the word for B94 is unified. Not chased further.

Proposed summary:
- does: steps a sprite entry's animation counter and, when it wraps, advances the frame and starts the follow-up effect
- status: size-exact draft, ROM holds gUnknown_02029A10 in one held register from a `.rodata` word for the stepping block and a plain literal for the tail
- left: the missing `.rodata` word for gUnknown_02029A10
- tried: entry pointer / inline macro / by-name array / mixed tail spellings (see above); none creates the word for gUnknown_02029A10, though the exemplar spelling does create it for gUnknown_02029B94
