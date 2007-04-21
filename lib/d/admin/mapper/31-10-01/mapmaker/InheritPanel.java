package mapmaker;

import java.awt.*;
import javax.swing.*;

public class InheritPanel extends PairedComponentPanel
{
    JTextField mNameField = new JTextField();
    JTextField mFilenameField = new JTextField();
    JTextField mAutoNameField = new JTextField();
    ColorButton mBackgroundButton = new ColorButton("Background Colour", Color.white);
    ColorButton mForegroundButton = new ColorButton("Foreground Colour", Color.black);

    //Inherit mInherit;

    public InheritPanel()
    {
        setLayout(new GridBagLayout());
        setBorder(BorderFactory.createEmptyBorder(12, 12, 11, 11));

        JLabel nameLabel = new JLabel("Name: ");
        JLabel filenameLabel = new JLabel("Filename: ");
        JLabel autoNameLabel = new JLabel("Auto Name: ");

        addComponentPair(nameLabel, mNameField);
        addComponentPair(filenameLabel, mFilenameField);
        addComponentPair(autoNameLabel, mAutoNameField);

        JPanel colorPanel = new JPanel();
        colorPanel.setLayout(new GridLayout(1, 0, 6, 0));
        colorPanel.add(mForegroundButton);
        colorPanel.add(mBackgroundButton);

        addHangingComponent(colorPanel);

/*
        InheritOkayAction okayAction = new InheritOkayAction();
        mNameField.addActionListener(okayAction);
        mFilenameField.addActionListener(okayAction);
        mRoomSizeField.addActionListener(okayAction);
        mAutoNameField.addActionListener(okayAction);
        setOkayAction(okayAction);

        pack();
        setLocationRelativeTo(aFrame);
*/
    }
/*
    public void setInherit(Inherit aInherit)
    {
        mInherit = aInherit;
    }

    public Inherit getInherit()
    {
        if (mInherit == null)
        {
            mInherit = new Inherit();
        }

        String name = mNameField.getText();
        String autoName = mAutoNameField.getText();
        String filename = mFilenameField.getText();

        mInherit.setRoomName(name);
        mInherit.setAutoName(autoName);
        mInherit.setInheritFilename(filename);
        mInherit.setBackgroundColor(mBackgroundButton.getColor());
        mInherit.setForegroundColor(mForegroundButton.getColor());

        // TODO:
        System.out.println("Bing 2: " + 20);
        mInherit.setRoomSize(20);

        return mInherit;
    }
*/
/*
    class InheritOkayAction extends OkayAction
    {
        public void actionPerformed(ActionEvent e)
        {
            if (!Utils.isNumberString(mRoomSizeField))
            {
                JOptionPane.showMessageDialog(InheritDialog.this,
                    "Please enter a whole number value in the room size field.\n",
                    "Error", JOptionPane.ERROR_MESSAGE);
                return;
            }

            if (!Utils.isFilledIn(mNameField))
            {
                JOptionPane.showMessageDialog(InheritDialog.this,
                    "Please enter a name for this inheritable in the name field.\n",
                    "Error", JOptionPane.ERROR_MESSAGE);
                return;
            }

            if (!Utils.isValidAutoName(mAutoNameField))
            {
                JOptionPane.showMessageDialog(InheritDialog.this,
                    "Please enter an auto name comprised to letters and\n"
                    + "underscore characters in the auto name field.\n",
                    "Error", JOptionPane.ERROR_MESSAGE);
                return;
            }

            if (!Utils.isFilledIn(mFilenameField))
            {
                JOptionPane.showMessageDialog(InheritDialog.this,
                    "Please enter a filename in the filename field.\n",
                    "Error", JOptionPane.ERROR_MESSAGE);
                return;
            }

            super.actionPerformed(e);
        }
    }

    public static Inherit showNewDialog(MainFrame aFrame)
    {
        InheritDialog inheritDialog = new InheritDialog(aFrame);
        inheritDialog.setVisible(true);

        if (inheritDialog.isOkay())
        {
            return inheritDialog.getInherit();
        }

        return null;
    }

    public static Inherit showEditDialog(MainFrame aFrame, Inherit aInherit)
    {
        InheritDialog inheritDialog = new InheritDialog(aFrame);
        inheritDialog.setInherit(aInherit);

        if (aInherit != null)
        {
            inheritDialog.mNameField.setText(aInherit.mName);
            inheritDialog.mRoomSizeField.setText("" + aInherit.getRoomSize());
            inheritDialog.mAutoNameField.setText("" + aInherit.getAutoName());
            inheritDialog.mFilenameField.setText("" + aInherit.getInheritFilename());
            inheritDialog.mBackgroundColorComboBox.setSelectedItem(aInherit.mBackgroundColor);
            inheritDialog.mForegroundColorComboBox.setSelectedItem(aInherit.mForegroundColor);
        }

        inheritDialog.setVisible(true);

        if (inheritDialog.isOkay())
        {
            return inheritDialog.getInherit();
        }

        return null;
    }
    */
}