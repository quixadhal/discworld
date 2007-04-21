package mapmaker;

import java.awt.*;
import java.awt.event.*;
import java.util.*;
import javax.swing.*;
import javax.swing.table.*;

public class EditableList extends JTable
{
    EditableListModel listModel = new EditableListModel();

    public EditableList()
    {
        setModel(listModel);
        setTableHeader(null);

        // Commit changes when focus is lost.
        addFocusListener(new FocusAdapter()
        {
            public void focusLost(FocusEvent e)
            {
                int row = getSelectedRow();

                if (row == -1)
                {
                    return;
                }

                int col = getSelectedColumn();

                DefaultCellEditor editor = (DefaultCellEditor)getCellEditor(row, 0);
                JTextField field = (JTextField)editor.getComponent();
                String value = field.getText();

                if (value == null || value.compareTo("") == 0)
                {
                    return;
                }

                editor.stopCellEditing();
            }
        });
    }

    public Vector getListItems()
    {
        return listModel.getData();
    }

    public void setListItems(Vector aItems)
    {
        listModel.setData(aItems);
    }
}

class EditableListModel extends AbstractTableModel
{
    static String NEW_ITEM = "";

    Vector data = new Vector();

    public EditableListModel()
    {
        data.addElement(NEW_ITEM);
    }

    public int getColumnCount()
    {
        return 1;
    }

    public Class getColumnClass(int aCol)
    {
        return String.class;
    }

    public Object getValueAt(int aRow, int aCol)
    {
        return data.elementAt(aRow);
    }

    public void setValueAt(Object aValue, int aRow, int aCol)
    {
        String oldItem = (String)data.elementAt(aRow);
        String newItem = (String)aValue;

        if (oldItem.compareTo(NEW_ITEM) == 0 && newItem.compareTo(NEW_ITEM) != 0)
        {
            data.addElement(NEW_ITEM);
            fireTableRowsInserted(aRow + 1, aRow + 1);
        }
        else if (oldItem.compareTo(NEW_ITEM) != 0 && newItem.compareTo(NEW_ITEM) == 0)
        {
            data.removeElementAt(aRow);
            fireTableRowsDeleted(aRow, aRow);
            return;
        }

        data.setElementAt(aValue, aRow);

        fireTableCellUpdated(aRow, aCol);
    }

    public boolean isCellEditable(int aRow, int aCol)
    {
        return true;
    }

    public int getRowCount()
    {
        return data.size();
    }

    public void setData(Vector aData)
    {
        data = aData;
        fireTableDataChanged();
    }

    public Vector getData()
    {
        return data;
    }
}
