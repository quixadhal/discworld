(arf
  (pattern "[at] <indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> [at] <indirect:any-living>")
  (pattern "[at] <indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You arf.")
      (rest "$mcname$ $verb$arfs,arf$.")
    )
    (arguments
      (arguments happily,sadly,nervously,wryly,broadly,crookedly,stupidly,idiotically,condescendingly,ironically,patiently,brightly,slightly,nastily,excitedly,strangely,with forced patience,plasticly,carefully,sadistically,lopsidedly,fishily,cattishly,faintly,like a train,angrily,paranoically,innocently,froggily,slyly,weakly,humbly,paradoxically,sarcastically,viciously,garishly,sweetly,innocently,lovingly,dreamily,radiantly,enthusiastically,insolently,quirkily,completely,unconsciously,drunkenly,insanely,cutely,maliciously,smugly,absentmindedly,curtly,disturbingly,unbelievably,quietly,loudly,differently,winningly,in triumph,seductively,tightly,gosh there are lots of arguments,softly,lividly,demonically,warmly,grimly,indolently,ingeniously,wickedly,contentedly,statistically,spasmodically,incandescently,pleasantly,with menace,like Ooeeoo,like ooeeoo,ingenuously,manically,warily,sardonically,lazily,serenely,disappointedly,#)
      (self "You arf $arg$.")
      (rest "$mcname$ $verb$arfs,arf$ $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You arf at $hcname$.")
      (target "$mcname$ $verb$arfs,arf$ at you.")
      (rest  "$mcname$ $verb$arfs,arf$ at $hcname$.")
    )
    (arguments
      (arguments  happily,sadly,nervously,wryly,broadly,crookedly,stupidly,idiotically,condescendingly,ironically,patiently,brightly,slightly,nastily,excitedly,strangely,with forced patience,plasticly,manically,carefully,sadistically,lopsidedly,fishily,cattishly,faintly,weakly,humbly,paradoxically,sarcastically,viciously,garishly,insolently,quirkily,completely,unconsciously,drunkenly,absentmindedly,curtly,disturbingly,unbelievably,quietly,loudly,differently,winningly,in triumph,seductively,sweetly,innocently,lovingly,dreamily,radiantly,enthusiastically,like a train,angrily,paranoically,innocently,froggily,slyly,macabrely,ruefully,tentatively,like a dog,tightly,gosh there are lots of arguments,softly,insanely,cutely,maliciously,smugly,grimly,lividly,demoniacally,indolently,ingeniously,wickedly,contentedly,statistically,spasmodically,incandescently,pleasantly,with menace,like Jim Douglas,arfishly,outlandishly,up the wall and round the corner,weirdly,erratically,smoke rings,like Ooeeoo,like ooeeoo,ingenuously,sardonically,warily,lazily,serenely,disappointedly,#)
      (self   "You arf $arg$ at $hcname$.")
      (target "$mcname$ $verb$arfs,arf$ $arg$ at you.")
      (rest  "$mcname$ $verb$arfs,arf$ $arg$ at $hcname$.")
    )
  )
)

