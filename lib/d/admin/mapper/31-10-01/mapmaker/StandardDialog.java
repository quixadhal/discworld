package mapmaker;

import java.awt.*;
import java.net.*;
import java.awt.event.*;
import javax.swing.*;

public class StandardDialog extends JDialog
{
    boolean mOkaySelected = false;

    DialogPanel mDialogPanel = new DialogPanel();

    JButton mOkayButton = new JButton(new OkayAction());
    JButton mCancelButton = new JButton(new CancelAction());

    public StandardDialog(JFrame aFrame, String aTitle)
    {
        super(aFrame, aTitle, true);

        setResizable(false);
        setDefaultCloseOperation(JDialog.DISPOSE_ON_CLOSE);

        mDialogPanel.addButton(mCancelButton);
        mDialogPanel.addButton(mOkayButton);
        setContentPane(mDialogPanel);

        pack();

        setLocationRelativeTo(aFrame);
    }

    public void addComponentPair(Component aLabel, Component aField)
    {
        mDialogPanel.addComponentPair(aLabel, aField);
    }

    public void addTripleComponentPair(Component aLabel1, Component aField1,
        Component aLabel2, Component aField2, Component aLabel3, Component aField3)
    {
        mDialogPanel.addTripleComponentPair(aLabel1, aField1, aLabel2, aField2,
            aLabel3, aField3);
    }

    public void setMainPanel(Component aPanel)
    {
        mDialogPanel.setMainPanel(aPanel);
    }

    public void setOkayAction(Action aAction)
    {
        mOkayButton.setAction(aAction);
    }

    public boolean isOkay()
    {
        return mOkaySelected;
    }

    class OkayAction extends AbstractAction
    {
        private static final String NAME = "OK";
        private static final String SHORT_DESCRIPTION = "OK";
        private static final String SMALL_ICON = "Okay16.gif";
        private static final String LONG_DESCRIPTION = "OK";

        public OkayAction()
        {
            putValue(Action.NAME, NAME);
            putValue(Action.SHORT_DESCRIPTION, SHORT_DESCRIPTION);
            putValue(Action.SMALL_ICON, Utils.getIcon(SMALL_ICON));
            putValue(Action.LONG_DESCRIPTION, LONG_DESCRIPTION);
        }

        public void actionPerformed(ActionEvent e)
        {
            mOkaySelected = true;
            setVisible(false);
        }
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