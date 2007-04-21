package mapmaker;

import java.awt.*;
import java.awt.event.*;
import java.util.*;
import javax.swing.*;

public class ChatsPanel extends JPanel
{
    JTextField mChatMinField = new JTextField();
    JTextField mChatMaxField = new JTextField();
    JTextField mDayChatMinField = new JTextField();
    JTextField mDayChatMaxField = new JTextField();
    JTextField mNightChatMinField = new JTextField();
    JTextField mNightChatMaxField = new JTextField();
    EditableList mChatsTable = new EditableList();
    EditableList mDayChatsTable = new EditableList();
    EditableList mNightChatsTable = new EditableList();

    public ChatsPanel()
    {
        super(new BorderLayout());

        JTabbedPane chatsTabbedPane = new JTabbedPane(JTabbedPane.BOTTOM);
        JLabel chatMinLabel = new JLabel("Minimum: ");
        JLabel chatMaxLabel = new JLabel("Maximum: ");
        JLabel dayChatMinLabel = new JLabel("Minimum: ");
        JLabel dayChatMaxLabel = new JLabel("Maximum: ");
        JLabel nightChatMinLabel = new JLabel("Minimum: ");
        JLabel nightChatMaxLabel = new JLabel("Maximum: ");

        Dimension dimension = new Dimension(0, 150);

        setBorder(BorderFactory.createEmptyBorder(12, 12, 11, 11));

        PairedComponentPanel chatPanel = new PairedComponentPanel();
        chatPanel.setBorder(BorderFactory.createEmptyBorder(12, 12, 11, 11));
        chatPanel.addDoubleComponentPair(chatMinLabel, mChatMinField,
            chatMaxLabel, mChatMaxField);
        JScrollPane chatPane = new JScrollPane(mChatsTable);
        chatPane.setPreferredSize(dimension);
        chatPanel.addAreaComponent(chatPane, 3);

        PairedComponentPanel dayChatPanel = new PairedComponentPanel();
        dayChatPanel.setBorder(BorderFactory.createEmptyBorder(12, 12, 11, 11));
        dayChatPanel.addDoubleComponentPair(dayChatMinLabel, mDayChatMinField,
            dayChatMaxLabel, mDayChatMaxField);
        JScrollPane dayChatPane = new JScrollPane(mDayChatsTable);
        dayChatPane.setPreferredSize(dimension);
        dayChatPanel.addAreaComponent(dayChatPane, 3);

        PairedComponentPanel nightChatPanel = new PairedComponentPanel();
        nightChatPanel.setBorder(BorderFactory.createEmptyBorder(12, 12, 11, 11));
        nightChatPanel.addDoubleComponentPair(nightChatMinLabel, mNightChatMinField,
            nightChatMaxLabel, mNightChatMaxField);
        JScrollPane nightChatPane = new JScrollPane(mNightChatsTable);
        nightChatPane.setPreferredSize(dimension);
        nightChatPanel.addAreaComponent(nightChatPane, 3);

        chatsTabbedPane.addTab("Standard", Utils.getIcon("Chat16.gif"),
            chatPanel);
        chatsTabbedPane.addTab("Day", Utils.getIcon("Daytime16.gif"),
            dayChatPanel);
        chatsTabbedPane.addTab("Night", Utils.getIcon("Nighttime16.gif"),
            nightChatPanel);

        add(chatsTabbedPane, BorderLayout.CENTER);
    }

    public Chat getChats()
    {
        Vector chats = mChatsTable.getListItems();

        if (chats.size() < 1)
        {
            return null;
        }

        return new Chat(mChatMinField.getText(),
            mChatMaxField.getText(), chats);
    }

    public Chat getDayChats()
    {
        Vector chats = mDayChatsTable.getListItems();

        if (chats.size() < 1)
        {
            return null;
        }

        return new Chat(mDayChatMinField.getText(),
            mDayChatMaxField.getText(), chats);
    }

    public Chat getNightChats()
    {
        Vector chats = mNightChatsTable.getListItems();

        if (chats.size() < 1)
        {
            return null;
        }

        return new Chat(mNightChatMinField.getText(),
            mNightChatMaxField.getText(), chats);
    }
}