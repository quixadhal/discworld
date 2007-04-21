(surprise
  (pattern "<indirect:any-living>")
  (pattern "<string> <indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (targeted
    (no-arguments
      (self   "You surprise $hcname$.")
      (target "$mcname$ surprise$s$ you.")
      (rest   "$mcname$ surprise$s$ $hcname$.")
    )
    (arguments
      (arguments  happily, sadly, cunningly, again, with a rubber snake, 
                  with a real snake, nicely, wonderfully, sneakily, 
                  with flowers, with chocolates, with a present, 
                  with two tickets for a rim-to-rim Disc-wide adventure 
                  holiday visiting every major attraction en route, #)
      (self   "You surprise $hcname$ $arg$.")
      (target "$mcname$ surprise$s$ you $arg$.")
      (rest   "$mcname$ surprise$s$ $hcname$ $arg$.")
    )
  )
)
