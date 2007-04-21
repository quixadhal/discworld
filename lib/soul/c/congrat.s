(congrat
  (pattern "<indirect:any-living>")
  (pattern "<string> <indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You congratulate everyone.")
      (rest "$mcname$ congratulates everyone.")
    )
    (arguments
      (arguments  heartily, warmly, cheerfully, sadly, sincerely, 
                  sarcastically, ruefully, #)
      (self "You congratulate everyone $arg$.")
      (rest "$mcname$ congratulates everyone $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You congratulate $hcname$.")
      (target "$mcname$ congratulates you.")
      (rest   "$mcname$ congratulates $hcname$.")
    )
    (arguments
      (arguments  heartily, warmly, cheerfully, sadly, sincerely, 
                  sarcastically, ruefully, #)
      (self   "You congratulate $hcname$ $arg$.")
      (target "$mcname$ congratulates you $arg$.")
      (rest   "$mcname$ congratulates $hcname$ $arg$.")
    )
  )
)
