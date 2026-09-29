
## wave 95

Base: the old draft (kept as `sub_080506B0.w95-start.c`, 38.45%, size +4). Draft now 41.96%, size +4, first difference +0xa (unchanged).

What moved it: the first-region read `gUnknown_03004580[side][1]` respelled as `*(u16 *)((u8 *)gUnknown_03004580 + 2 + side * 16)` (38.5 -> 42.0%). The ROM adds the +2 to the loaded word before adding the row offset, and this spelling reproduces that instruction order. Four equivalent spellings (`&g[side][1]`, `(u8 *)&g[side] + 2`, product-first sum, `((u16 *)((u8 *)g + 2))[side * 8]`) are byte-identical to each other.

Pre-registered hypothesis (word appears when the address is an address VALUE with a variable term): NOT confirmed. The value-with-variable-term spelling above still leaves gUnknown_03004580 a plain literal. Note the ROM's word has a single use in the text (the read in the first region); the later `gUnknown_03004582[..][0]` reads are a separate plain pool word, so the "P needs a second use" reading of the gcse chapter (docs: "The .rodata force-addr word is made by GCSE's PRE") does not by itself explain this word. Not resolved.

Not run: the permuter (cannot create the missing word).

Proposed summary:
- does: sets up the sprite for a unit's tile-marker effect and stores its screen offsets
- status: 42% at +4 bytes; three of the ROM's three `.rodata` words are needed, the draft creates two
- left: the `.rodata` word for gUnknown_03004580 (single use in the first region)
- tried: two-index vs byte-offset spelling of the read (byte-offset is better, no word); four equivalent spellings; the u8-cast form does not create the word
