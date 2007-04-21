package mapmaker;

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;

public class DescriptionPanel extends JPanel
{
    InheritField mShortField = new InheritField();
    InheritField mDeterminateField = new InheritField();
    InheritField mLightField = new InheritField();
    InheritField mRoomSizeField = new InheritField();

    InheritTextArea mLongTextArea = new InheritTextArea(6, 40);
    InheritTextArea mDayLongTextArea = new InheritTextArea(6, 40);
    InheritTextArea mNightLongTextArea = new InheritTextArea(6, 40);

    public DescriptionPanel()
    {
        super(new BorderLayout());

        JLabel shortLabel = new JLabel("Short Description: ");
        JLabel determinateLabel = new JLabel("Determinate: ");
        JLabel lightLabel = new JLabel("Light Level: ");
        JLabel longLabel = new JLabel("Long Description: ");
        JLabel roomSizeLabel = new JLabel("Room Size: ");

        JTabbedPane longTabbedPane = new JTabbedPane(JTabbedPane.BOTTOM);
        longTabbedPane.addTab("Standard", Utils.getIcon("Description16.gif"),
            mLongTextArea);
        longTabbedPane.addTab("Day", Utils.getIcon("Daytime16.gif"),
            mDayLongTextArea);
        longTabbedPane.addTab("Night", Utils.getIcon("Nighttime16.gif"),
            mNightLongTextArea);

        PairedComponentPanel descriptionPanel = new PairedComponentPanel();
        descriptionPanel.setBorder(BorderFactory.createEmptyBorder(12, 12, 11, 11));
        descriptionPanel.addComponentPair(shortLabel, mShortField);
        descriptionPanel.addDoubleComponentPair(determinateLabel,
            mDeterminateField, lightLabel, mLightField);
        descriptionPanel.addDoubleComponentPair(roomSizeLabel,
            mRoomSizeField, new JPanel(), new JPanel());

        descriptionPanel.addAreaComponentPair(longLabel, longTabbedPane, 3);

        add(descriptionPanel, BorderLayout.CENTER);
    }

    public Descriptions getDescriptions()
    {
        Descriptions descriptions = new Descriptions();

        descriptions.setLight(mLightField.getText());
        descriptions.setLightInherited(mLightField.isInherited());

        descriptions.setDeterminate(mDeterminateField.getText());
        descriptions.setDeterminateInherited(mDeterminateField.isInherited());

        descriptions.setShort(mShortField.getText());
        descriptions.setShortInherited(mShortField.isInherited());

        descriptions.setLong(mLongTextArea.getText());
        descriptions.setLongInherited(mLongTextArea.isInherited());

        descriptions.setDayLong(mDayLongTextArea.getText());
        descriptions.setDayLongInherited(mDayLongTextArea.isInherited());

        descriptions.setNightLong(mNightLongTextArea.getText());
        descriptions.setNightLongInherited(mNightLongTextArea.isInherited());

        descriptions.setSize(mRoomSizeField.getText());
        descriptions.setSizeInherited(mRoomSizeField.isInherited());

        return descriptions;
    }
}