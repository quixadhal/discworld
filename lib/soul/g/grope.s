(grope
  (pattern "<indirect:any-living>")
  (pattern "<string> <indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (targeted
    (no-arguments
      (self   "You grope $hcname$.")
      (target "$mcname$ grope$s$ you.")
      (rest   "$mcname$ grope$s$ $hcname$.")
    )
    (arguments
      (arguments  hand, leg, knee, foot, finger, ear, hair,
                  soft curvaceous pillows, tree trunk, strawberries,
                  eyebrows, nose, cucumber, melons, coconuts, #)
      (self   "You grope $mhcname$ $arg$.")
      (target "$mcname$ grope$s$ your $arg$.")
      (rest   "$mcname$ grope$s$ $mhcname$ $arg$.")
    )
    (arguments
      (arguments  carefully, politely, surreptitiously, without looking, 
                  just the once, twice, when you're least expecting it, 
                  randomly, jealously, fondly, neatly, gently, surprisingly, 
                  in accordance with Rule 4: subsection vi)
      (self   "You grope $hcname$ $arg$.")
      (target "$mcname$ grope$s$ you $arg$.")
      (rest   "$mcname$ grope$s$ $hcname$ $arg$.")
    )
  )
)
