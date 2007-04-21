(boogie
  (pattern "<string>")
  (pattern "<indirect:any-living>" )
  (pattern "<string> <indirect:any-living>" )
  (pattern "<indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You boogie down.")
      (rest "$mcname$ boogie$s$ down.")
    )
    (arguments
    (arguments  can)
      (self "Yes sir, you can boogie.")
      (rest "Yes sir, $mcname$ can boogie.")
    )
    (arguments
      (arguments  woogie, in the streets, down the street, 
                  like it's the end of the century, 
                  like it's the Century of the Fruitbat, the night away, 
                  around the room, on the spot, badly, yes sir, 
                  like the roof is on fire, 
                  like you hope your parents never did, 
                  like it's a Saturday night, #)
      (self "You boogie $arg$.")
      (rest "$mcname$ boogie$s$ $arg$.")
    )
    (arguments
      (arguments  till you drop, drop)
      (self "You boogie till you drop.")
      (rest "$mcname$ boogie$s$ till $mpronoun$ drop$s$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You boogie at $hcname$.")
      (target "$mcname$ boogie$s$ at you.")
      (rest   "$mcname$ boogie$s$ at $hcname$.")
    )
    (arguments
      (arguments around, like, up to, down at, away from, with, without)
      (self   "You boogie $arg$ $hcname$.")
      (target "$mcname$ boogie$s$ $arg$ you.")
      (rest   "$mcname$ boogie$s$ $arg$ $hcname$.")
    )
    (arguments
      (arguments  woogie, in the streets, down the street, 
                  like it's the end of the century, 
                  like it's the Century of the Fruitbat, the night away, 
                  around the room, on the spot, badly, yes sir, 
                  like the roof is on fire, 
                  like you hope your parents never did, 
                  like it's a Saturday night, #)
      (self   "You boogie $arg$ at $hcname$.")
      (target "$mcname$ boogie$s$ $arg$ at you.")
      (rest   "$mcname$ boogie$s$ $arg$ at $hcname$.")
    )
    (arguments
      (arguments  till you drop, drop)
      (self   "You boogie till you drop at $hcname$.")
      (target "$mcname$ boogie$s$ till $mpronoun$ drop$s$ at you.")
      (rest   "$mcname$ boogie$s$ till $mpronoun$ drop$s$ at $hcname$.")
    )
  )
)
/* Saffra Sun July 28 2001 */
