(prance
  (pattern "[around] <indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> [around] <indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You prance around.")
      (rest "$mcname$ prance$s$ around.")
    )
    (arguments
      (arguments happily, madly, brightly, sadly, backwards, mischievously, 
                 gleefully, like a pictsie, like a sprite, like a mountain 
                 goat, like a court jester, like a tumbler, like a gymnast, 
                 like a troll, like a ballerina, like a hippopotamus, like an 
                 assassin, #)
      (self "You prance around $arg$.")
      (rest "$mcname$ prance$s$ around $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You prance around $hcname$.")
      (target "$mcname$ prance$s$ around you.")
      (rest   "$mcname$ prance$s$ around $hcname$.")
    )    
    (arguments
      (arguments happily, madly, brightly, sadly, backwards, mischievously, 
                 gleefully, like a pictsie, like a sprite, like a mountain 
                 goat, like a court jester, like a tumbler, like a gymnast, 
                 like a troll, like a ballerina, like a hippopotamus, like an 
                 assassin, #)
      (self   "You prance around $hcname$ $arg$.")
      (target "$mcname$ prance$s$ around you $arg$.")
      (rest   "$mcname$ prance$s$ around $hcname$ $arg$.")
    )
  )
)

/* Implemented by Saffra, suggested by Puk and Beale Apr 02 2003 */
