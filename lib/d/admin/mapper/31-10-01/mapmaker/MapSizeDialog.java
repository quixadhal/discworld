package mapmaker;

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;

public class MapSizeDialog extends StandardDialog
{
    JLabel mHeightLabel = new JLabel("Height: ");
    JTextField mHeightField = new JTextField();
    JLabel mWidthLabel = new JLabel("Width: ");
    JTextField mWidthField = new JTextField();

    Dimension mMapSize;

    public MapSizeDialog(JFrame aFrame)
    {
        super(aFrame, "Map Size");

        DialogPanel dialogPanel = (DialogPanel)getContentPane();
        dialogPanel.addComponentPair(mWidthLabel, mWidthField);
        dialogPanel.addComponentPair(mHeightLabel, mHeightField);

        SizeOkayAction sizeOkayAction = new SizeOkayAction();
        mHeightField.addActionListener(sizeOkayAction);
        mWidthField.addActionListener(sizeOkayAction);
        setOkayAction(sizeOkayAction);

        pack();
        setLocationRelativeTo(aFrame);
    }

    public void setDimension(Dimension aDimension)
    {
        mHeightField.setText("" + aDimension.height);
        mWidthField.setText("" + aDimension.width);
    }

    class SizeOkayAction extends OkayAction
    {
        public void actionPerformed(ActionEvent e)
        {
            int h, w;

            try
            {
                h = Integer.parseInt(mHeightField.getText());
                w = Integer.parseInt(mWidthField.getText());
            }
            catch (Exception ex)
            {
                JOptionPane.showMessageDialog(MapSizeDialog.this,
                    "Please ensure both height and width fields are whole numbers.",
                    "Error", JOptionPane.ERROR_MESSAGE);
                return;
            }

            mMapSize = new Dimension(w, h);

            super.actionPerformed(e);
        }
    }

    public Dimension getMapSize()
    {
        return mMapSize;
    }

    public static Dimension showNewRoomDialog(MainFrame aFrame, Dimension aDimension)
    {
        MapSizeDialog sizeDialog = new MapSizeDialog(aFrame);
        sizeDialog.setDimension(aDimension);
        sizeDialog.setVisible(true);

        if (sizeDialog.isOkay())
        {
            return sizeDialog.getMapSize();
        }

        return null;
    }
}