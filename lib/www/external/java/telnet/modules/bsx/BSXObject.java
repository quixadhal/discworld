/*
 * BSXObject	-- a simple BSX Object
 * --
 * $Id: BSXObject.java,v 1.1 1997/12/16 10:59:49 leo Exp $
 */
package modules.bsx;

public class BSXObject
{
  public int position = 0, layer = 0;
  public boolean visible = true;

  public BSXObject(int p, int l) { position = p; layer = l; }
}
