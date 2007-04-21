(astound
  (pattern "<indirect:any-living>")
  (pattern "<string> [at] <indirect:any-living>")
  (pattern "[at] <indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You are astounded!")
      (rest "$mcname$ $V$0=is,are$V$ astounded!")
    )
  )
  (targeted
    (no-arguments
      (self   "You astound $hcname$!")
      (target "$mcname$ astound$s$ you!")
      (rest  "$mcname$ astound$s$ $hcname$.")
    )
    (arguments
      (arguments  stupidity, cleverness, thoughtfulness, dumbness, idiocy, clever use of soul commands, deviousness, VW Beetle, annoyingness, paranoia, wideness of your grin, coffee table, womble, grinning frog, stupid pet tricks, #)
      (self   "You astound $hcname$ with your $arg$!")
      (target "$mcname$ astound$s$ you with $mposs$ $arg$!")
      (rest  "$mcname$ astound$s$ $hcname$ with $mposs$ $arg$.")
    )
  )
)
