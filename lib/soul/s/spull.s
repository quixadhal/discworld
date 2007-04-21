(spull
  (pattern "<indirect:any-living>")
  (pattern "<string> <indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (targeted
    (arguments
     (arguments over, under, away, along, around, up, down, #)
      (self "You pull $hcname$ $arg$.")
      (target "$mcname$ pull$s$ you $arg$.")
      (rest "$mcname$ pull$s$ $hcname$ $arg$.")
    )
  )
)
/* Saffra, 4-26-00 */

