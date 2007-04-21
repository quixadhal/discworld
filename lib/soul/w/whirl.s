#  -*- LPC -*- #
#
# $Locker:  $
# $Id: whirl.s,v 1.2 2003/04/22 01:38:10 saffra Exp $
#
#
#

(whirl
  (pattern "<indirect:any-living>")
  (pattern "<string>")
  (pattern "<indirect:any-living> [at] <string>")
  (pattern "<string> [at] <indirect:any-living>")
  (single
    (no-arguments
      (self "You whirl around.")
      (rest "$mcname$ whirl$s$ around.")
    )
    (arguments
      (arguments suddenly, quickly, slowly, dizzily, nauseatingly, in a panic,
                 like a whirlwind, like a tornado, like a blender, like a dust
                 devil, like a dervish, like a Tasmanian Devil, in a spiral, 
                 in a holding pattern, #)
      (self "You whirl around $arg$.")
      (rest "$mcname$ whirl$s$ around $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You whirl around $hcname$.")
      (target "$mcname$ whirl$s$ around you.")
      (rest   "$mcname$ whirl$s$ around $hcname$.")
    )
    (arguments
      (arguments suddenly, quickly, slowly, dizzily, nauseatingly, in a panic,
                 like a whirlwind, like a tornado, like a blender, like a dust
                 devil, like a dervish, like a Tasmanian Devil, in a spiral, 
                 in a holding pattern, #)
      (self   "You whirl around $hcname$ $arg$.")
      (target "$mcname$ whirl$s$ around you $arg$.")
      (rest   "$mcname$ whirl$s$ around $hcname$ $arg$.")
    )
  )
)
/* Saffra 5-11-01 */
