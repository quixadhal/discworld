/*
 * BSXGraphic	-- BSX Graphics Object
 * --
 * $Id: BSXGraphic.java,v 1.1 1997/12/16 10:59:49 leo Exp $
 * $timestamp: Wed Feb 14 21:35:42 1996 by Matthias L. Jugel :$
 */
package modules.bsx;

import java.util.Vector;

public class BSXGraphic extends Vector
{
  public BSXGraphic() { super(); }
  public BSXGraphic(int s) 
  { 
		super(s);
  }

  public void addPolygon(BSXPolygon p) { addElement(p); }
}
