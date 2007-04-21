package mapmaker;

import java.util.*;
import javax.swing.table.*;

class ItemTableModel extends AbstractTableModel
{
    MapLocation mMapLocation;
    Vector mItemList = new Vector();

    public int getColumnCount()
    {
        return 3;
    }

    public String getColumnName(int aCol)
    {
        String string = null;

        switch (aCol)
        {
            case 0 :
                string = "Short Descriptions";
                break;
            case 1 :
                string = "Long Description";
                break;
            case 2 :
                string = "Inherit?";
                break;
        }

        return string;
    }

    public Class getColumnClass(int aCol)
    {
        Class[] colClasses = { String.class, String.class, Boolean.class };

        return colClasses[aCol];
    }

    public Object getValueAt(int aRow, int aCol)
    {
        Object ob = null;
        String string = "";
        Item item = (Item)mItemList.elementAt(aRow);

        switch (aCol)
        {
            case 0 :
                Vector shorts = item.getShorts();
                for (int i = 0; i < shorts.size(); i++)
                {
                    if (i == shorts.size() - 1)
                    {
                        string += (String)shorts.elementAt(i);
                    }
                    else
                    {
                        string += (String)shorts.elementAt(i) + ", ";
                    }
                }
                ob = string;
                break;
            case 1 :
                string = item.getDescription();
                ob = string;
                break;
            case 2 :
                ob = new Boolean(!item.isExcludedIn(mMapLocation));
        }

        return ob;
    }

    public boolean isCellEditable(int aRow, int aCol)
    {
        if (aCol == 2)
        {
            Item item = (Item)mItemList.elementAt(aRow);

            if (item.getInheritLevel() == 0)
            {
                return false;
            }

            return true;
        }
        return false;
    }

    public void setValueAt(Object aValue, int aRow, int aCol)
    {
        if (aCol == 2)
        {
            Item item = (Item)mItemList.elementAt(aRow);

            if (((Boolean)aValue).booleanValue() == true)
            {
                item.removeExcludedIn(mMapLocation);
            }
            else
            {
                item.addExcludedIn(mMapLocation);
            }
        }

        fireTableCellUpdated(aRow, aCol);
    }

    public int getRowCount()
    {
        return mItemList.size();
    }

    public void setMapLocation(MapLocation aMapLocation)
    {
        mMapLocation = aMapLocation;
    }

    public MapLocation getMapLocation()
    {
        return mMapLocation;
    }

    public void setItemList(Vector aData)
    {
        mItemList = aData;
        fireTableDataChanged();
    }

    public void addItem(Item aItem)
    {
        mItemList.addElement(aItem);
        int row = mItemList.indexOf(aItem);
        fireTableRowsInserted(row, row);
    }

    public void removeItem(Item aItem)
    {
        mItemList.removeElement(aItem);
        int row = mItemList.indexOf(aItem);
        fireTableRowsDeleted(row, row);
    }

    public void replaceItem(Item aOldItem, Item aNewItem)
    {
        int row = mItemList.indexOf(aOldItem);
        mItemList.setElementAt(aNewItem, row);
        fireTableRowsUpdated(row, row);
    }

    public Vector getItemList()
    {
        return mItemList;
    }
}