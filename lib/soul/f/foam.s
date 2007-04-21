(foam
  (pattern "[at] <indirect:any-living>")
  (pattern "<string> [at] <indirect:any-living>")
  (pattern "[at] <indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You roll your tongue around in your mouth until spit bubbles and fo
ams on your lips.")
      (rest "$mcname$ roll$s$ $mposs$ tongue around in $mposs$ mouth until spit bu
bbles and foams on $mposs$ lips.")
    )
  )
  (targeted
    (no-arguments
      (self "You roll your tongue around in your mouth until spit bubbles and fo
ams on your lips while thinking of $hcname$.")
      (target "$mcname$ roll$s$ $mposs$ tongue around in $mposs$ mouth until spit 
bubbles and foams on $mposs$ lips while thinking of you.")
      (rest "$mcname$ roll$s$ $mposs$ tongue around in $mposs$ mouth until spit bu
bbles and foams on $mposs$ lips while thinking of $hcname$.")
    )
  )
)

