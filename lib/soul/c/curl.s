(curl
  (pattern "<indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (pattern "<string> <indirect:any-living>")
  (single
    (no-arguments
      (self "You curl up into a ball.")
      (rest "$mcname$ curls up into a ball.")
    )
  )
  (targeted
    (no-arguments
      (self   "You curl up in $mhcname$ lap.")
      (target "$mcname$ curls up in your lap.")
      (rest   "$mcname$ curls up in $mhcname$ lap.")
    )
    (arguments
      (arguments  arms, hands, hair, bed, bathtub, car, dreams,
                  thoughts, heart, laundry, room, pants, #)  
      (self   "You curl up in $mhcname$ $arg$.")
      (target "$mcname$ curls up in your $arg$.")
      (rest   "$mcname$ curls up in $mhcname$ $arg$.")
    )
  )
)

/* Koryn suggested it. Saffra, 10-6-00 */
