(whap
  (pattern "<indirect:any-living>")
  (pattern "<string> <indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You whap yourself repeatedly.")
      (rest "$mcname$ whaps $mobj$self repeatedly.")
    )
  )
  (targeted
    (no-arguments
      (self   "You whap $hcname$ over the head.")
      (target "$mcname$ whap$s$ you over the head.")
      (rest  "$mcname$ whap$s$ $hcname$ over the head.")
    )
    (arguments
      (arguments  hard,harder,really hard,incredibly hard,
                  with a fish,with a wet towel,with Descartes' LPC manual,
                  with a frog,with a big stick,twice,viciously,carefully,
     violently,lovingly,repeatedly,like a knight with a fish in shining armour,# )
      (self   "You whap $hcname$ over the head $arg$.")
      (target "$mcname$ whap$s$ you over the head $arg$.")
      (rest  "$mcname$ whap$s$ $hcname$ over the head $arg$.")
    )
  )
)

