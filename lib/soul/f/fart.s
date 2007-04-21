(fart
  (pattern "[at] <indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> [at] <indirect:any-living>")
  (pattern "[at] <indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You fart.")
      (rest "$mcname$ fart$s$.")
    )
    (arguments
      (arguments loudly, discreetly, violently, discreetly but violently, 
                 backwards, stinkily, downwind, mischievously, devilishly, 
                 impishly, cheekily, idiotically, nervously, wryly, quietly,
                 skeptically, and leaves a large hole in your underwear,  
                 with a big stink, without shame, 
                 and lifts several inches off the ground, and it isn't funny, 
                 like a small nuclear detonation, #)
      (self "You fart $arg$.")
      (rest "$mcname$ fart$s$ $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You fart in $mhcname$ general direction.")
      (target "$mcname$ fart$s$ in your general direction.")
      (rest  "$mcname$ fart$s$ in $mhcname$ general direction.")
    )
    (arguments
      (arguments loudly, discreetly, violently, discreetly but violently, 
                 backwards, stinkily, downwind, mischievously, devilishly, 
                 impishly, cheekily, idiotically, nervously, wryly, quietly,
                 skeptically, and leaves a large hole in your underwear,  
                 with a big stink, without shame, 
                 and lifts several inches off the ground, and it isn't funny, 
                 like a small nuclear detonation, #)
      (self   "You fart at $hcname$ $arg$.")
      (target "$mcname$ fart$s$ at you $arg$.")
      (rest  "$mcname$ fart$s$ at $hcname$ $arg$.")

    )
  )
)

/* Rewritten by Hobbes, 970531 */
