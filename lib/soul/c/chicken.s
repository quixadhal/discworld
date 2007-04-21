(chicken
  (pattern "<indirect:any-living>")
  (pattern "<string> <indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
   (targeted
    (no-arguments
   (self   "You flap your arms and cluck like a chicken at $hcname$.")
   (target "$mcname$ flaps $mposs$ arms and clucks like a chicken at you.")
   (rest  "$mcname$ flaps $mposs$ arms and clucks like a chicken at $hcname$.")
         )
       )
    )
/*Hobbes 970317 */
