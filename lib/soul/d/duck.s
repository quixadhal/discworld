(duck
  (pattern "[from] <indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> [from] <indirect:any-living>")
  (pattern "[from] <indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You duck.")
      (rest "$mcname$ duck$s$.")
    )
    (arguments
     (arguments for cover, the snowball, quickly, slowly, badly,
                     inappropriately, pointlessly, sadly, carefully, #)
      (self "You duck $arg$.")
      (rest "$mcname$ duck$s$ $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You duck away from $hcname$.")
      (target "$mcname$ duck$s$ away from you.")
      (rest  "$mcname$ duck$s$ away from $hcname$.")
    )
    (arguments
     (arguments quickly, slowly, badly, inappropriately, pointlessly, sadly, carefully, #)
      (self   "You duck away from $hcname$ $arg$.")
      (target "$mcname$ duck$s$ away from $hcname$ $arg$.")
      (rest  "$mcname$ duck$s$ away from $hcname$ $arg$.")
    )
  )
)

