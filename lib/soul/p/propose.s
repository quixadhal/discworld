(propose
  (pattern "<indirect:any-living>")
  (pattern "<string> <indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (targeted
    (no-arguments
      (self   "You propose to $hcname$.")
      (target "$mcname$ propose$s$ to you.")
      (rest   "$mcname$ propose$s$ to $hcname$.")
    )
    (arguments
      (arguments  close, affectionately, cutely, happily, sadly, 
                  comfortingly, madly, #)
      (self   "You propose to $hcname$ $arg$.")
      (target "$mcname$ propose$s$ to you $arg$.")
      (rest   "$mcname$ propose$s$ to $hcname$ $arg$.")
    )
  )
)

