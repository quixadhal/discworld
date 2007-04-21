package mapmaker;

import java.awt.*;
import java.util.*;
import javax.swing.*;
import javax.swing.table.*;

public class ExitTypeBean extends JPanel
{
    ExitTypeTableModel mModel = new ExitTypeTableModel();
    JTable mTable = new JTable(mModel);

    public ExitTypeBean()
    {
        super(new BorderLayout());

        setBorder(BorderFactory.createEmptyBorder(12, 12, 11, 11));

        Dimension dimension = new Dimension(0, 150);

        JScrollPane tableScrollPane = new JScrollPane(mTable);
        tableScrollPane.setPreferredSize(dimension);

        add(tableScrollPane);
    }

    public Vector getExits()
    {
        return mModel.getExitList();
    }

    public void setExits(Vector aExits)
    {
        mModel.setExitList(aExits);
    }
}

class ExitTypeTableModel extends AbstractTableModel
{
    Vector mExitList = new Vector();

    public Object getValueAt(int aRow, int aCol)
    {
        String string = "";
        Exit exit = (Exit)mExitList.elementAt(aRow);

        switch (aCol)
        {
            case 0 :
                int dir = exit.getDirection();

                if (dir != -1)
                {
                    string = Direction.getDirectionName(dir);
                }
                else
                {
                    string = exit.getName();
                }
                break;
            case 1 :
                if (exit.isDummy())
                {
                    string = exit.getDummy();
                }
                else
                {
                    MapLocation location = exit.getTo();

                    if (location.isUseAutoName())
                    {
                        string = location.getAutoName();
                    }
                    else
                    {
                        string = location.getFileName();
                    }
                }
                break;
            case 2 :
                string = exit.getType();
                break;
        }

        return string;
    }

    public void setValueAt(Object aValue, int aRow, int aCol)
    {
        Exit exit = (Exit)mExitList.elementAt(aRow);

        switch (aCol)
        {
            case 0 :
                exit.setName((String)aValue);
                break;
            case 1 :
                exit.setDummy((String)aValue);
                break;
            case 2 :
                exit.setType((String)aValue);
                break;
        }

        fireTableCellUpdated(aRow, aCol);
    }

    public boolean isCellEditable(int aRow, int aCol)
    {
        boolean editable = false;
        Exit exit = (Exit)mExitList.elementAt(aRow);

        switch (aCol)
        {
            case 0 :
                if (!exit.isDirection())
                {
                    editable = true;
                }
                break;
            case 1 :
                if (!(exit.getTo() instanceof MapLocation))
                {
                    editable = true;
                }
                break;
            case 2 :
                editable = true;
                break;
        }

        return editable;
    }

    public String getColumnName(int aCol)
    {
        String string = null;

        switch (aCol)
        {
            case 0 :
                string = "Direction";
                break;
            case 1 :
                string = "Destination";
                break;
            case 2 :
                string = "Type";
                break;
        }

        return string;
    }

    public int getRowCount()
    {
        return mExitList.size();
    }

    public int getColumnCount()
    {
        return 3;
    }

    public void setExitList(Vector aData)
    {
        mExitList = aData;
        fireTableDataChanged();
    }

    public Vector getExitList()
    {
        return mExitList;
    }
}