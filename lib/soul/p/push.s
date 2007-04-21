(push
  (pattern "<indirect:any-living>")
  (targeted
    (no-arguments
      (self   "You push $hcname$ over$position$.")
      (target "$mcname$ push$s$ you over$position$.")
      (rest  "$mcname$ push$s$ $hcname$ over$position$.")
      (position
        (standing "sit")
        (lying "ignore")
        (default "lie")
        (silent "yes")
      )
    )
  )
)

