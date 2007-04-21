package mapmaker;

import java.awt.*;
import java.awt.event.*;
import java.util.*;
import javax.swing.*;

public class ItemDialog extends StandardDialog
{
    JLabel mShortsLabel = new JLabel("Short Descriptions");
    EditableList mShortsList = new EditableList();
    JLabel mLongLabel = new JLabel("Long Description");
    JTextArea mLongTextArea = new JTextArea();

    Item mItem;

    public ItemDialog(JFrame aFrame)
    {
        super(aFrame, "Item");

        JPanel shortsPanel = new JPanel(new BorderLayout(0, 5));
        shortsPanel.add(mShortsLabel, BorderLayout.NORTH);
        JScrollPane shortsListScollPane = new JScrollPane(mShortsList);
        shortsListScollPane.setPreferredSize(new Dimension(150, 100));
        shortsPanel.add(shortsListScollPane, BorderLayout.CENTER);

        JPanel longPanel = new JPanel(new BorderLayout(0, 5));
        longPanel.add(mLongLabel, BorderLayout.NORTH);
        JScrollPane longTextScollPane = new JScrollPane(mLongTextArea);
        longTextScollPane.setPreferredSize(new Dimension(300, 100));
        longPanel.add(longTextScollPane, BorderLayout.CENTER);

        JPanel mainPanel = new JPanel(new BorderLayout(12, 0));
        mainPanel.add(shortsPanel, BorderLayout.WEST);
        mainPanel.add(longPanel, BorderLayout.CENTER);

        setMainPanel(mainPanel);

        mLongTextArea.setLineWrap(true);
        mLongTextArea.setWrapStyleWord(true);

        pack();
        setLocationRelativeTo(aFrame);
    }

    class ItemOkayAction extends OkayAction
    {
        public void actionPerformed(ActionEvent e)
        {
            Vector shorts = mShortsList.getListItems();

            if (shorts.size() < 1)
            {
                JOptionPane.showMessageDialog(ItemDialog.this,
                    "Please enter at least one short descripion in the "
                    + "short description list.\n",
                    "Error", JOptionPane.ERROR_MESSAGE);
                return;
            }

            if (!Utils.isFilledIn(mLongTextArea))
            {
                JOptionPane.showMessageDialog(ItemDialog.this,
                    "Please enter the long description of this item in the "
                    + "long description text area.\n",
                    "Error", JOptionPane.ERROR_MESSAGE);
                return;
            }

            super.actionPerformed(e);
        }
    }

    public void setItem(Item aItem)
    {
        mItem = aItem;
    }

    public Item getItem()
    {
        Vector shorts = mShortsList.getListItems();
        String description = mLongTextArea.getText();

        shorts.removeElement("");

        if (mItem == null)
        {
            mItem = new Item(shorts, description);
        }
        else
        {
            mItem.setShorts(shorts);
            mItem.setDescription(description);
        }

        return mItem;
    }

    public static Item showEditDialog(JFrame aFrame, Item aItem)
    {
        ItemDialog itemDialog = new ItemDialog(aFrame);
        itemDialog.setItem(aItem);

        if (aItem != null)
        {
            Vector shorts = aItem.getShorts();
            shorts.removeElement("");
            shorts.addElement("");
            itemDialog.mShortsList.setListItems(shorts);
            itemDialog.mLongTextArea.setText(aItem.getDescription());
        }

        itemDialog.setVisible(true);

        if (itemDialog.isOkay())
        {
            return itemDialog.getItem();
        }

        return null;
    }

    public static Item showNewDialog(JFrame aFrame)
    {
        ItemDialog itemDialog = new ItemDialog(aFrame);
        itemDialog.setVisible(true);

        if (itemDialog.isOkay())
        {
            return itemDialog.getItem();
        }

        return null;
    }
}