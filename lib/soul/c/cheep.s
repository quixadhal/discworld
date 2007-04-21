#  -*- LPC -*- #
#
# $Locker: presto $
# $Id: cheep.s,v 1.1 2002/05/03 03:12:52 saffra Exp presto $
#
#
#

(cheep
  (pattern "[at] <indirect:any-living>")
  (pattern "<string>")
  (pattern " <string> [at] <indirect:any-living>")
  (pattern "[at] <indirect:any-living> <string> ")
  (single
    (no-arguments
      (self "You cheep like a cute fluffy chick.")
      (rest "$mcname$ cheep$s$ like a cute fluffy chick.")
    )
    (arguments
      (arguments cutely, adorably, fluffily, brightly, quietly,
                 like a wet hen, like a ruffled chicken, like a 
                 marshmallow peep,  # )
      (self "You cheep $arg$.")
      (rest "$mcname$ cheep$s$ $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self "You cheep at $hcname$ like a cute fluffy chick.")
      (target "$mcname$ cheep$s$ at you like a cute fluffy chick.")
      (rest "$mcname$ cheep$s$ at $hcname$ like a cute fluffy chick.")
    )
    (arguments
      (arguments cutely, adorably, fluffily, brightly, quietly,
                 like a wet hen, like a ruffled chicken, like a 
                 marshmallow peep,  # )
      (self   "You cheep $arg$ at $hcname$.")
      (target "$mcname$ cheep$s$ $arg$ at you.")
      (rest  "$mcname$ cheep$s$ $arg$ at $hcname$.")
    )
  )
)
/* Saffra 6-14-00 */
