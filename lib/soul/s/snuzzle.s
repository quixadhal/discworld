(snuzzle
  (pattern "<indirect:any-living>")
  (pattern "<string> <indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (targeted
    (no-arguments
      (self   "You snuzzle $hcname$.")
      (target "$mcname$ snuzzle$s$ you.")
      (rest   "$mcname$ snuzzle$s$ $hcname$.")
    )
    (arguments
      (arguments affectionately, warmly, gently, absentmindedly, 
                 seductively, dreamily, adorably, lustily, tenderly, 
                 lovingly, sensuously, longingly, softly, passionately,
                 excitedly, snuzzily, snuzzably, on a coffee table, #)
        (self   "You snuzzle $hcname$ $arg$.")
        (target "$mcname$ snuzzle$s$ you $arg$.")
        (rest   "$mcname$ snuzzle$s$ $hcname$ $arg$.")
    )
  )
)

/* Saffra, 10-17-00, on the bequest of Laurana. */
