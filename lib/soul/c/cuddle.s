(cuddle
  (pattern "<indirect:any-living>")
  (pattern "<string> <indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (targeted
    (no-arguments
      (self   "You cuddle $hcname$.")
      (target "$mcname$ cuddles you.")
      (rest   "$mcname$ cuddles $hcname$.")
    )
    (arguments
      (arguments  close, affectionately, cutely, happily, sadly, 
                  comfortingly, madly, #)
      (self   "You cuddle $hcname$ $arg$.")
      (target "$mcname$ cuddles you $arg$.")
      (rest   "$mcname$ cuddles $hcname$ $arg$.")
    )
  )
)

