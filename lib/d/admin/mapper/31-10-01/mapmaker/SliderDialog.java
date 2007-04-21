package mapmaker;

import java.net.*;
import java.awt.*;
import java.awt.event.*;
import javax.swing.*;

public class SliderDialog extends JDialog
{
    boolean mOkaySelected = false;

    DialogPanel mDialogPanel = new DialogPanel();

    JButton mCancelButton = new JButton(new CancelAction());

    JLabel mActionLabel = new JLabel();
    JTextField mActionField = new JTextField();

    JLabel mSliderLabel = new JLabel();
    JProgressBar mProgressBar = new JProgressBar();

    public SliderDialog(JFrame aFrame, String aTitle, String aActionLabel,
        String aSliderLabel)
    {
        super(aFrame, aTitle, true);

        setResizable(false);
        setDefaultCloseOperation(JDialog.DISPOSE_ON_CLOSE);

        mActionField.setEditable(false);

        mSliderLabel.setText(aSliderLabel);
        mActionLabel.setText(aActionLabel);

        mDialogPanel.addComponentPair(mActionLabel, mActionField);
        mDialogPanel.addComponentPair(mSliderLabel, mProgressBar);

        mDialogPanel.addButton(mCancelButton);
        setContentPane(mDialogPanel);

        pack();

        setLocationRelativeTo(aFrame);
    }

    class CancelAction extends AbstractAction
    {
        private static final String NAME = "Cancel";
        private static final String SHORT_DESCRIPTION = "Cancel";
        private static final String SMALL_ICON = "Cancel16.gif";
        private static final String LONG_DESCRIPTION = "Cancel";

        public CancelAction()
        {
            putValue(Action.NAME, NAME);
            putValue(Action.SHORT_DESCRIPTION, SHORT_DESCRIPTION);
            putValue(Action.SMALL_ICON, Utils.getIcon(SMALL_ICON));
            putValue(Action.LONG_DESCRIPTION, LONG_DESCRIPTION);
        }

        public void actionPerformed(ActionEvent e)
        {
            mOkaySelected = false;
            setVisible(false);
        }
    }
}