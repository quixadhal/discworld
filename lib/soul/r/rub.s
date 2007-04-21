(rub
  (pattern "<indirect:any-living>")
    (pattern "<string> [against] <indirect:any-living>")
    (pattern "<indirect:any-living> [against] <string>")
  (pattern "<string>")
  (single
    (arguments
      (arguments hands together,eyes,tummy,nose,leg,arm,eyebrows,head, #)
      (self "You rub your $arg$.")
      (rest "$mcname$ rub$s$ $mposs$ $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You rub $hcname$ up the wrong way.")
      (target "$mcname$ rub$s$ you up the wrong way.")
      (rest  "$mcname$ rub$s$ $hcname$ up the wrong way.")
    )
    (arguments
        (arguments cabbage, fruitcake, dancing hippotomaus, unshaven body, #)
        (self "You rub your $arg$ against $hcname$.")
        (target "$mcname$ rub$s$ $mposs$ $arg$ against you.")
        (rest "$mcname$ rub$s$ $mposs$ $arg$ against $hcname$.")
    )
  )
)

