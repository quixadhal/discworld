/*
 * BSXScene	-- BSX Scene
 * --
 * $Id: BSXScene.java,v 1.1 1997/12/16 10:59:49 leo Exp $
 */
package modules.bsx;

import java.util.Hashtable;

public class BSXScene
{
  public BSXGraphic background;
  public Hashtable objects = new Hashtable();

  public BSXScene() { background = null; }
	public BSXScene(BSXGraphic pic) { background = pic; }
}
