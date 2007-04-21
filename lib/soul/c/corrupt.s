#  -*- LPC -*- #
#
# $Locker: presto $
# $Id: corrupt.s,v 1.1 2002/05/03 03:17:14 saffra Exp presto $
#
#
#

(corrupt
  (pattern "<indirect:any-living>")
  (pattern "<string> <indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (targeted
    (no-arguments
      (self   "You corrupt $hcname$.")
      (target "$mcname$ corrupts you.")
      (rest   "$mcname$ corrupts $hcname$.")
    )
    (arguments
      (arguments  lasciviously, lewdly, wickedly, smarmily, seductively,
                  like a dirty old man, like a dirty old woman, like a
                  sailor on a lonely night, with a whip on a train, 
whilst waiting for dinner, #)
       (self   "You corrupt $hcname$ $arg$.")
       (target "$mcname$ corrupts you $arg$.")
       (rest   "$mcname$ corrupts $hcname$ $arg$.")
    )
    (arguments
      (arguments  code, bits, computer, toes, lips, fika, earlobes, monkey, 
                  great big blunt bonus, eyebrow, food, mouse, toys, underwear,             
                  fork, frog, womble, sarcasm, innuendo, pants, oranges, 
                  cucumbers, strawberries, leer)
       (self   "You corrupt $mhcname$ $arg$.")
       (target "$mcname$ corrupts your $arg$.")
       (rest   "$mcname$ corrupts $mhcname$ $arg$.")
    )
  )
)
