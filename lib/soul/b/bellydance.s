(bellydance
  (pattern "[at] <indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> [at] <indirect:any-living>")
  (pattern "[at] <indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You bellydance.")
      (rest "$mcname$ bellydance$s$.")
    )
    (arguments
    (arguments sensuously, around, in circles, in ellipses, in figure 7+1s,
               jigglingly, with lots of shoulder, with lots of hip, with 
               lots of thigh, with lots of leg, with lots of belly, with 
               lots of waist, with lots of bottom, with lots of arm movements, 
               with armwaving, with handwaving, with bells, with chimes, 
               with bangles, like a harem girl, wildly, slowly, with baba 
               ganoush, like a Klatchian, # )
      (self "You bellydance $arg$.")
      (rest "$mcname$ bellydance$s$ $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You bellydance at $hcname$.")
      (target "$mcname$ bellydance$s$ at you.")
      (rest   "$mcname$ bellydance$s$ at $hcname$.")
    )
    (arguments
    (arguments around, up to, away from, near, with)
      (self   "You bellydance $arg$ $hcname$.")
      (target "$mcname$ bellydance$s$ $arg$ you.")
      (rest   "$mcname$ bellydance$s$ $arg$ $hcname$.")
    )
    (arguments
    (arguments sensuously, in circles, in ellipses, in figure 7+1s,
               jigglingly, with lots of shoulder, with lots of hip, with 
               lots of thigh, with lots of leg, with lots of belly, with 
               lots of waist, with lots of bottom, with lots of arm movements, 
               with armwaving, with handwaving, with bells, with chimes, 
               with bangles, like a harem girl, wildly, slowly, with baba 
               ganoush, like a Klatchian, # )
      (self   "You bellydance at $hcname$ $arg$.")
      (target "$mcname$ bellydance$s$ at you $arg$.")
      (rest   "$mcname$ bellydance$s$ at $hcname$ $arg$.")
    )
  )
)

/* Saffra Mar 30 2003 */
