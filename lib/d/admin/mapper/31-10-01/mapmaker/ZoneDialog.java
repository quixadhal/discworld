package mapmaker;

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;

public class ZoneDialog extends StandardDialog
{
    JTextField mNameField = new JTextField();
    ColorComboBox mColorComboBox = new ColorComboBox();

    Zone mZone = null;

    public ZoneDialog(JFrame aFrame)
    {
        super(aFrame, "Zone");

        JLabel nameLabel = new JLabel("Name: ");
        JLabel colorLabel = new JLabel("Colour: ");

        DialogPanel dialogPanel = (DialogPanel)getContentPane();
        dialogPanel.addComponentPair(nameLabel, mNameField);
        dialogPanel.addComponentPair(colorLabel, mColorComboBox);

        ZoneOkayAction okayAction = new ZoneOkayAction();
        mNameField.addActionListener(okayAction);
        setOkayAction(okayAction);

        pack();
        setLocationRelativeTo(aFrame);
    }

    public void setZone(Zone aZone)
    {
        mZone = aZone;
    }

    public Zone getZone()
    {
        if (mZone == null)
        {
            mZone = new Zone();
        }

        mZone.setName(mNameField.getText());
        mZone.setColor((Color)mColorComboBox.getSelectedItem());

        return mZone;
    }

    class ZoneOkayAction extends OkayAction
    {
        public void actionPerformed(ActionEvent e)
        {
            if (!Utils.isFilledIn(mNameField))
            {
                JOptionPane.showMessageDialog(ZoneDialog.this,
                    "Please enter a name for this zone in the name field.\n",
                    "Error", JOptionPane.ERROR_MESSAGE);
                return;
            }

            super.actionPerformed(e);
        }
    }

    public static Zone showEditDialog(JFrame aFrame, Zone aZone)
    {
        ZoneDialog zoneDialog = new ZoneDialog(aFrame);
        zoneDialog.setZone(aZone);

        if (aZone != null)
        {
            zoneDialog.mNameField.setText(aZone.getName());
            zoneDialog.mColorComboBox.setSelectedItem(aZone.getColor());
        }

        zoneDialog.setVisible(true);

        if (zoneDialog.isOkay())
        {
            return zoneDialog.getZone();
        }

        return null;
    }

    public static Zone showNewDialog(JFrame aFrame)
    {
        ZoneDialog zoneDialog = new ZoneDialog(aFrame);
        zoneDialog.setVisible(true);

        if (zoneDialog.isOkay())
        {
            return zoneDialog.getZone();
        }

        return null;
    }
}