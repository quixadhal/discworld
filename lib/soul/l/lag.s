(lag
  (pattern "<indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> <indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You lag.")
      (rest "$mcname$ lag$s$.")
    )
    (arguments
      (arguments terribly, tragically, fatally, uncontrollably, laggily, lots,       
                 like going uphill, far behind, a lot, a little, through the
                 night, through piles of fluff, like molasses, like treacle, 
                 like peanut butter, with the sensation of fingernails on
                 slate, like the slow Discworld light, like a great star
                 turtle, like a wizard after a good meal, like a very laggy
                 thing, like the truth putting its boots on, like the LAG
                 itself, #)
      (self "You lag $arg$.")
      (rest "$mcname$ lag$s$ $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You lag at $hcname$.")
      (target "$mcname$ lag$s$ at you.")
      (rest   "$mcname$ lag$s$ at $hcname$.")
    )
    (arguments
      (arguments terribly, tragically, fatally, uncontrollably, laggily, lots,       
                 like going uphill, far behind, a lot, a little, through the
                 night, through piles of fluff, like molasses, like treacle, 
                 like peanut butter, with the sensation of fingernails on
                 slate, like the slow Discworld light, like a great star
                 turtle, like a wizard after a good meal, like a very laggy
                 thing, like the truth putting its boots on, like the LAG
                 itself, #)
      (self   "You lag at $hcname$ $arg$.")
      (target "$mcname$ lag$s$ at you $arg$.")
      (rest  "$mcname$ lag$s$ at $hcname$ $arg$.")
    )
  )
)
/* 11-29-00 By Saffra with help from Fevvers and Presto */
