package mapmaker;

import java.awt.*;
import java.awt.event.*;
import java.util.*;
import javax.swing.*;
import javax.swing.table.*;
import javax.swing.event.*;

public class ItemsPanel extends JPanel
{
    JTable mItemsTable = new JTable(new ItemTableModel());
    JTable mDayItemsTable = new JTable(new ItemTableModel());
    JTable mNightItemsTable = new JTable(new ItemTableModel());

    JButton mNewItemButton = new JButton(new NewItemAction());
    JButton mEditItemButton = new JButton(new EditItemAction());
    JButton mDeleteItemButton = new JButton(new DeleteItemAction());
    JButton mPromoteItemButton = new JButton(new PromoteItemAction());

    JFrame mFrame;

    public ItemsPanel(JFrame aFrame)
    {
        super(new BorderLayout(12, 12));

        mFrame = aFrame;

        setBorder(BorderFactory.createEmptyBorder(12, 12, 11, 11));

        Dimension dimension = new Dimension(0, 150);

        JTabbedPane itemsTabbedPane = new JTabbedPane(JTabbedPane.BOTTOM);
        itemsTabbedPane.setPreferredSize(dimension);
        itemsTabbedPane.addTab("Standard", Utils.getIcon("Items16.gif"),
            new JScrollPane(mItemsTable));
        itemsTabbedPane.addTab("Day", Utils.getIcon("Daytime16.gif"),
            new JScrollPane(mDayItemsTable));
        itemsTabbedPane.addTab("Night", Utils.getIcon("Nighttime16.gif"),
            new JScrollPane(mNightItemsTable));

        mItemsTable.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
        mDayItemsTable.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
        mNightItemsTable.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);

        mItemsTable.setDefaultRenderer(String.class,
            new InheritRenderer());
        mItemsTable.setDefaultRenderer(Boolean.class,
            new InheritBooleanRenderer());
        mDayItemsTable.setDefaultRenderer(String.class,
            new InheritRenderer());
        mDayItemsTable.setDefaultRenderer(Boolean.class,
            new InheritBooleanRenderer());
        mNightItemsTable.setDefaultRenderer(String.class,
            new InheritRenderer());
        mNightItemsTable.setDefaultRenderer(Boolean.class,
            new InheritBooleanRenderer());

        ButtonPanel itemButtonPanel = new ButtonPanel();
        itemButtonPanel.addButton(mNewItemButton);
        itemButtonPanel.addButton(mEditItemButton);
        itemButtonPanel.addButton(mPromoteItemButton);
        itemButtonPanel.addButton(mDeleteItemButton);

        ListSelectionModel itemSelectionModel = mItemsTable.getSelectionModel();
        itemSelectionModel.addListSelectionListener(new SelectionListener(mItemsTable));
        ListSelectionModel dayItemSelectionModel = mDayItemsTable.getSelectionModel();
        dayItemSelectionModel.addListSelectionListener(new SelectionListener(mDayItemsTable));
        ListSelectionModel nightItemSelectionModel = mNightItemsTable.getSelectionModel();
        nightItemSelectionModel.addListSelectionListener(new SelectionListener(mNightItemsTable));

        mEditItemButton.setEnabled(false);
        mDeleteItemButton.setEnabled(false);
        mPromoteItemButton.setEnabled(false);

        itemsTabbedPane.addChangeListener(new ChangeListener()
        {
            public void stateChanged(ChangeEvent e)
            {
                // Maybe not the best solution.
                mEditItemButton.setEnabled(false);
                mDeleteItemButton.setEnabled(false);
                mPromoteItemButton.setEnabled(false);

                mItemsTable.clearSelection();
                mDayItemsTable.clearSelection();
                mNightItemsTable.clearSelection();
            }
        });

        add(itemsTabbedPane, BorderLayout.CENTER);
        add(itemButtonPanel, BorderLayout.SOUTH);
    }

    public Vector getItems()
    {
        ItemTableModel model = (ItemTableModel)mItemsTable.getModel();

        Vector items = model.getItemList();
        if (items.size() < 1)
        {
            return null;
        }

        return items;
    }

    public Vector getDayItems()
    {
        ItemTableModel model = (ItemTableModel)mDayItemsTable.getModel();

        Vector items = model.getItemList();
        if (items.size() < 1)
        {
            return null;
        }

        return items;
    }

    public Vector getNightItems()
    {
        ItemTableModel model = (ItemTableModel)mNightItemsTable.getModel();

        Vector items = model.getItemList();
        if (items.size() < 1)
        {
            return null;
        }

        return items;
    }

    class InheritRenderer extends DefaultTableCellRenderer
    {
        public Component getTableCellRendererComponent(JTable aTable,
            Object aValue, boolean aIsSelected, boolean aHasFocus, int aRow,
            int aCol)
        {
            Component component = (JLabel)super.getTableCellRendererComponent(
                aTable, aValue, aIsSelected, aHasFocus, aRow, aCol);

            Vector itemList = ((ItemTableModel)mItemsTable.getModel()).getItemList();
            Item item = (Item)itemList.elementAt(aRow);

            if (item.getInheritLevel() != 0)
            {
                if (component instanceof JComponent)
                {
                    JComponent jComponent = (JComponent)component;

                    jComponent.setForeground(Color.gray);
                }
            }

            return component;
        }
    }

    class InheritBooleanRenderer extends DefaultTableCellRenderer
    {
        public Component getTableCellRendererComponent(JTable aTable,
            Object aValue, boolean aIsSelected, boolean aHasFocus, int aRow,
            int aCol)
        {
            Component component = (JLabel)super.getTableCellRendererComponent(
                aTable, aValue, aIsSelected, aHasFocus, aRow, aCol);

            boolean booleanValue = ((Boolean)aValue).booleanValue();
            JCheckBox checkBox = new JCheckBox("", booleanValue);

            checkBox.setBackground(component.getBackground());
            checkBox.setHorizontalAlignment(SwingConstants.CENTER);

            Vector itemList = ((ItemTableModel)mItemsTable.getModel()).getItemList();
            Item item = (Item)itemList.elementAt(aRow);

            if (item.getInheritLevel() == 0)
            {
                checkBox.setEnabled(false);
            }

            return checkBox;
        }
    }

    class NewItemAction extends AbstractAction
    {
        private static final String NAME = "New...";
        private static final String SHORT_DESCRIPTION = "New";
        private static final String LONG_DESCRIPTION = "Create a new item";

        public NewItemAction()
        {
            putValue(Action.NAME, NAME);
            putValue(Action.SHORT_DESCRIPTION, SHORT_DESCRIPTION);
            putValue(Action.LONG_DESCRIPTION, LONG_DESCRIPTION);
        }

        public void actionPerformed(ActionEvent e)
        {
            Item item = ItemDialog.showNewDialog(mFrame);

            ItemTableModel itemTableModel = null;

            if (item != null)
            {
                if (mItemsTable.isShowing())
                {
                    itemTableModel = (ItemTableModel)mItemsTable.getModel();
                }
                else if (mDayItemsTable.isShowing())
                {
                    itemTableModel = (ItemTableModel)mDayItemsTable.getModel();
                }
                else if (mNightItemsTable.isShowing())
                {
                    itemTableModel = (ItemTableModel)mNightItemsTable.getModel();
                }

                itemTableModel.addItem(item);
            }
        }
    }

    class EditItemAction extends AbstractAction
    {
        private static final String NAME = "Edit...";
        private static final String SHORT_DESCRIPTION = "Edit";
        private static final String LONG_DESCRIPTION = "Edit the selected item";

        public EditItemAction()
        {
            putValue(Action.NAME, NAME);
            putValue(Action.SHORT_DESCRIPTION, SHORT_DESCRIPTION);
            putValue(Action.LONG_DESCRIPTION, LONG_DESCRIPTION);
        }

        public void actionPerformed(ActionEvent e)
        {
            JTable itemTable = null;

            if (mItemsTable.isShowing())
            {
                itemTable = mItemsTable;
            }
            else if (mDayItemsTable.isShowing())
            {
                itemTable = mDayItemsTable;
            }
            else if (mNightItemsTable.isShowing())
            {
                itemTable = mNightItemsTable;
            }

            int row = itemTable.getSelectionModel().getLeadSelectionIndex();

            if (row == -1)
            {
                JOptionPane.showMessageDialog(mFrame,
                    "Please select an item to edit.", "Error",
                    JOptionPane.ERROR_MESSAGE);
                return;
            }

            ItemTableModel model = (ItemTableModel)itemTable.getModel();
            Item oldItem = (Item)model.getItemList().elementAt(row);

            Item newItem = ItemDialog.showEditDialog(mFrame, oldItem);

            if (newItem != null)
            {
                model.replaceItem(oldItem, newItem);
            }
        }
    }

    class PromoteItemAction extends AbstractAction
    {
        private static final String NAME = "Promote";
        private static final String SHORT_DESCRIPTION = "Promote";
        private static final String LONG_DESCRIPTION = "Promote the item to the inheritable";

        public PromoteItemAction()
        {
            putValue(Action.NAME, NAME);
            putValue(Action.SHORT_DESCRIPTION, SHORT_DESCRIPTION);
            putValue(Action.LONG_DESCRIPTION, LONG_DESCRIPTION);
        }

        public void actionPerformed(ActionEvent e)
        {
            JTable itemTable = null;

            if (mItemsTable.isShowing())
            {
                itemTable = mItemsTable;
            }
            else if (mDayItemsTable.isShowing())
            {
                itemTable = mDayItemsTable;
            }
            else if (mNightItemsTable.isShowing())
            {
                itemTable = mNightItemsTable;
            }

            int row = itemTable.getSelectionModel().getLeadSelectionIndex();

            if (row == -1)
            {
                JOptionPane.showMessageDialog(mFrame,
                    "Please select an item to promote.", "Error",
                    JOptionPane.ERROR_MESSAGE);
                return;
            }

            ItemTableModel model = (ItemTableModel)itemTable.getModel();
            Item item = (Item)model.getItemList().elementAt(row);
            item.incrementInheritLevel();
            model.fireTableDataChanged();
        }
    }

    class DeleteItemAction extends AbstractAction
    {
        private static final String NAME = "Delete";
        private static final String SHORT_DESCRIPTION = "Delete";
        private static final String LONG_DESCRIPTION = "Delete the selected item";

        public DeleteItemAction()
        {
            putValue(Action.NAME, NAME);
            putValue(Action.SHORT_DESCRIPTION, SHORT_DESCRIPTION);
            putValue(Action.LONG_DESCRIPTION, LONG_DESCRIPTION);
        }

        public void actionPerformed(ActionEvent e)
        {
            JTable itemTable = null;

            if (mItemsTable.isShowing())
            {
                itemTable = mItemsTable;
            }
            else if (mDayItemsTable.isShowing())
            {
                itemTable = mDayItemsTable;
            }
            else if (mNightItemsTable.isShowing())
            {
                itemTable = mNightItemsTable;
            }

            int row = itemTable.getSelectionModel().getLeadSelectionIndex();

            if (row == -1)
            {
                JOptionPane.showMessageDialog(mFrame,
                    "Please select an item to delete.", "Error",
                    JOptionPane.ERROR_MESSAGE);
                return;
            }

            ItemTableModel model = (ItemTableModel)itemTable.getModel();
            Item item = (Item)model.getItemList().elementAt(row);
            model.removeItem(item);
        }
    }

    class SelectionListener implements ListSelectionListener
    {
        JTable mTable;

        public SelectionListener(JTable aTable)
        {
            mTable = aTable;
        }

        public void valueChanged(ListSelectionEvent e)
        {
            boolean enabled = (mTable.getSelectedRow() != -1);

            mEditItemButton.setEnabled(enabled);
            mDeleteItemButton.setEnabled(enabled);

            if (enabled)
            {
                ItemTableModel model = (ItemTableModel)mTable.getModel();
                Vector itemList = model.getItemList();
                Item item = (Item)itemList.elementAt(mTable.getSelectedRow());

                if (item.getInheritLevel() == 0)
                {
                    if (model.getMapLocation() == null) System.out.println("Fucker!");

                    if (model.getMapLocation().mInheritedRoomProperties != null)
                    {
                        mPromoteItemButton.setEnabled(true);
                    }
                }
            }
            else
            {
                mPromoteItemButton.setEnabled(false);
            }
        }
    }
}