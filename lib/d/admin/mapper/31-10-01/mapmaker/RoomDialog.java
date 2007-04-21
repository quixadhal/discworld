package mapmaker;

import java.awt.*;
import java.awt.event.*;
import java.util.*;
import javax.swing.*;
import javax.swing.event.*;
import javax.swing.table.*;

public class RoomDialog extends StandardDialog
{
    ItemsPanel mItemsPanel;
    InheritPanel mInheritPanel = new InheritPanel();
    ChatsPanel mChatsPanel = new ChatsPanel();
    DescriptionPanel mDescriptionPanel = new DescriptionPanel();
    ExitTypeBean mExitPanel = new ExitTypeBean();
    JTabbedPane mMainTabbedPane = new JTabbedPane();

    JFrame mFrame;

    MapLocation mMapLocation;

    public RoomDialog(JFrame aFrame)
    {
        super(aFrame, "Room Properties");

        mItemsPanel = new ItemsPanel(mFrame);

        mFrame = aFrame;

        mMainTabbedPane.addTab("Inherit", Utils.getIcon("Inherit16.gif"),
            mInheritPanel);
        mMainTabbedPane.addTab("Description", Utils.getIcon("Description16.gif"),
            mDescriptionPanel);
        mMainTabbedPane.addTab("Items", Utils.getIcon("Items16.gif"),
            mItemsPanel);
        mMainTabbedPane.addTab("Chats", Utils.getIcon("Chat16.gif"),
            mChatsPanel);
        mMainTabbedPane.addTab("Exits", Utils.getIcon("Exits16.gif"),
            mExitPanel);

        setMainPanel(mMainTabbedPane);

        pack();
        setLocationRelativeTo(aFrame);
    }

    public void setMapLocation(MapLocation aMapLocation)
    {
        mMapLocation = aMapLocation;
    }

    public void updateMapLocation()
    {
        try
        {
            // Update descriptions...
            Descriptions descs = mDescriptionPanel.getDescriptions();
            mMapLocation.setProperty("size", descs.getSize());
            mMapLocation.setInherited("size", descs.isSizeInherited());
            mMapLocation.setProperty("light", descs.getLight());
            mMapLocation.setInherited("light", descs.isLightInherited());
            mMapLocation.setProperty("short", descs.getShort());
            mMapLocation.setInherited("short", descs.isShortInherited());
            mMapLocation.setProperty("determinate", descs.getDeterminate());
            mMapLocation.setInherited("determinate", descs.isDeterminateInherited());
            mMapLocation.setProperty("long", descs.getLong());
            mMapLocation.setInherited("long", descs.isLongInherited());
            mMapLocation.setProperty("dayLong", descs.getDayLong());
            mMapLocation.setInherited("dayLong", descs.isDayLongInherited());
            mMapLocation.setProperty("nightLong", descs.getNightLong());
            mMapLocation.setInherited("nightLong", descs.isNightLongInherited());

            // Update chat...
            mMapLocation.setProperty("chat", mChatsPanel.getChats());
            mMapLocation.setProperty("dayChat", mChatsPanel.getDayChats());
            mMapLocation.setProperty("nightChat", mChatsPanel.getNightChats());
        }
        catch (Exception ex)
        {
            ex.printStackTrace();
        }

        // Update items.
        mMapLocation.setItems(mItemsPanel.getItems());
        mMapLocation.setDayItems(mItemsPanel.getDayItems());
        mMapLocation.setNightItems(mItemsPanel.getNightItems());

        // Update exits.
        Vector exits = mExitPanel.getExits();
        mMapLocation.setExits(exits);
    }

    public RoomProperties getInherit()
    {
        RoomProperties roomProperties = new RoomProperties();

        // Retrieve information from the inherit panel.
        String name = mInheritPanel.mNameField.getText();
        String autoName = mInheritPanel.mAutoNameField.getText();
        String filename = mInheritPanel.mFilenameField.getText();
        Color background = mInheritPanel.mBackgroundButton.getColor();
        Color foreground = mInheritPanel.mForegroundButton.getColor();

        roomProperties.setAutoName(autoName);
        roomProperties.setInheritFilename(filename);
        roomProperties.setBackgroundColor(background);
        roomProperties.setForegroundColor(foreground);

        // Retrieve information from the description panel.
        String light = mDescriptionPanel.mLightField.getText();
        String roomSize = mDescriptionPanel.mRoomSizeField.getText();
        String roomShort = mDescriptionPanel.mShortField.getText();
        String determinate = mDescriptionPanel.mDeterminateField.getText();
        String roomLong = mDescriptionPanel.mLongTextArea.getText();
        String dayLong = mDescriptionPanel.mDayLongTextArea.getText();
        String nightLong = mDescriptionPanel.mNightLongTextArea.getText();

        try
        {
            roomProperties.setProperty("name", name);
            roomProperties.setProperty("size", roomSize);
            roomProperties.setProperty("light", light);
            roomProperties.setProperty("short", roomShort);
            roomProperties.setProperty("determinate", determinate);
            roomProperties.setProperty("long", roomLong);
            roomProperties.setProperty("dayLong", dayLong);
            roomProperties.setProperty("nightLong", nightLong);
        }
        catch (Exception ex)
        {
            ex.printStackTrace();
        }

        ItemTableModel model;
        Vector items;

        // Normal Items
        model = (ItemTableModel)mItemsPanel.mItemsTable.getModel();
        items = model.getItemList();
        roomProperties.setItems(items);

        // Day Items
        model = (ItemTableModel)mItemsPanel.mDayItemsTable.getModel();
        items = model.getItemList();
        roomProperties.setDayItems(items);

        // Night Items
        model = (ItemTableModel)mItemsPanel.mNightItemsTable.getModel();
        items = model.getItemList();
        roomProperties.setNightItems(items);

        return roomProperties;
    }

    public static RoomProperties showNewDialog(JFrame aFrame)
    {
        MapLocation location = new MapLocation(0, 0);

        RoomDialog roomDialog = new RoomDialog(aFrame);
        roomDialog.setMapLocation(location);

        roomDialog.mMainTabbedPane.setEnabledAt(0, true);
        roomDialog.mMainTabbedPane.setEnabledAt(4, false);
        roomDialog.mMainTabbedPane.setSelectedIndex(0);

        ItemTableModel itemModel = (ItemTableModel)roomDialog.mItemsPanel.mItemsTable.getModel();
        ItemTableModel dayItemModel = (ItemTableModel)roomDialog.mItemsPanel.mDayItemsTable.getModel();
        ItemTableModel nightItemModel = (ItemTableModel)roomDialog.mItemsPanel.mNightItemsTable.getModel();

        itemModel.setMapLocation(location);
        dayItemModel.setMapLocation(location);
        nightItemModel.setMapLocation(location);

        roomDialog.setVisible(true);

        if (roomDialog.isOkay())
        {
            return roomDialog.getInherit();
        }

        return null;
    }

    public static void populateInheritField(InheritComponent aInheritComponent,
        MapLocation aLocation, String aProperty)
        throws NoSuchRoomPropertyException
    {
        boolean inherited = aLocation.isPropertyInherited(aProperty);
        String value = (String)aLocation.getProperty(aProperty,
            RoomProperties.INHERIT_DO_CHECK);
        String inheritedValue = (String)aLocation.getProperty(aProperty,
            RoomProperties.INHERIT_FORCE);

        aInheritComponent.setValues(inheritedValue, value, inherited);
    }

    public static void showEditDialog(JFrame aFrame, MapLocation aLocation)
    {
        RoomDialog roomDialog = new RoomDialog(aFrame);
        roomDialog.setMapLocation(aLocation);

        try
        {
            ChatsPanel chatsPanel = roomDialog.mChatsPanel;

            Chat chats = (Chat)aLocation.getProperty("chat",
                RoomProperties.INHERIT_DO_CHECK);
            if (chats != null)
            {
                chatsPanel.mChatMaxField.setText(chats.getMaximum());
                chatsPanel.mChatMinField.setText(chats.getMinimum());
                chatsPanel.mChatsTable.setListItems(chats.getChats());
            }

            Chat dayChats = (Chat)aLocation.getProperty("dayChat",
                RoomProperties.INHERIT_DO_CHECK);
            if (dayChats != null)
            {
                chatsPanel.mDayChatMaxField.setText(dayChats.getMaximum());
                chatsPanel.mDayChatMinField.setText(dayChats.getMinimum());
                chatsPanel.mDayChatsTable.setListItems(dayChats.getChats());
            }

            Chat nightChats = (Chat)aLocation.getProperty("nightChat",
                RoomProperties.INHERIT_DO_CHECK);
            if (nightChats != null)
            {
                chatsPanel.mNightChatMaxField.setText(nightChats.getMaximum());
                chatsPanel.mNightChatMinField.setText(nightChats.getMinimum());
                chatsPanel.mNightChatsTable.setListItems(nightChats.getChats());
            }

            // Populate the items panel.
            Vector items = aLocation.getItems();
            Vector dayItems = aLocation.getDayItems();
            Vector nightItems = aLocation.getNightItems();
            ItemsPanel itemsPanel = roomDialog.mItemsPanel;

            ItemTableModel itemModel = (ItemTableModel)itemsPanel.mItemsTable.getModel();
            ItemTableModel dayItemModel = (ItemTableModel)itemsPanel.mDayItemsTable.getModel();
            ItemTableModel nightItemModel = (ItemTableModel)itemsPanel.mNightItemsTable.getModel();

            itemModel.setMapLocation(aLocation);
            dayItemModel.setMapLocation(aLocation);
            nightItemModel.setMapLocation(aLocation);

            if (items != null)
            {
                itemModel.setItemList(items);
            }
            else
            {
                itemModel.setItemList(new Vector());
            }

            if (dayItems != null)
            {
                dayItemModel.setItemList(dayItems);
            }
            else
            {
                dayItemModel.setItemList(new Vector());
            }

            if (nightItems != null)
            {
                nightItemModel.setItemList(nightItems);
            }
            else
            {
                nightItemModel.setItemList(new Vector());
            }

            // Populate the descriptions panel.
            DescriptionPanel descsPanel = roomDialog.mDescriptionPanel;

            populateInheritField(descsPanel.mLightField,
                aLocation, "light");
            populateInheritField(descsPanel.mRoomSizeField,
                aLocation, "size");
            populateInheritField(descsPanel.mShortField,
                aLocation, "short");
            populateInheritField(descsPanel.mLongTextArea,
                aLocation, "long");
            populateInheritField(descsPanel.mDayLongTextArea,
                aLocation, "dayLong");
            populateInheritField(descsPanel.mNightLongTextArea,
                aLocation, "nightLong");
            populateInheritField(descsPanel.mDeterminateField,
                aLocation, "determinate");
        }
        catch (Exception ex)
        {
            ex.printStackTrace();
        }

        // Populate the exits panel.
        Vector exits = aLocation.getExits();
        ExitTypeBean exitPanel = roomDialog.mExitPanel;
        exitPanel.mModel.setExitList(exits);

        roomDialog.mMainTabbedPane.setEnabledAt(0, false);
        roomDialog.mMainTabbedPane.setEnabledAt(4, true);
        roomDialog.mMainTabbedPane.setSelectedIndex(1);

        roomDialog.setVisible(true);

        if (roomDialog.isOkay())
        {
            roomDialog.updateMapLocation();
        }
    }
}
