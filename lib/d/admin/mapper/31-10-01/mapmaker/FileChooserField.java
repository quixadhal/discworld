package mapmaker;

import java.awt.*;
import java.net.*;
import java.awt.event.*;
import javax.swing.*;

public class FileChooserField extends JPanel
{
    public static final int CHOOSE_DIRECTORY = 0;
    public static final int CHOOSE_FILE = 1;

    JTextField mFileField = new JTextField();
    JButton mChooserButton = new JButton();

    public FileChooserField(int aType)
    {
        super(new BorderLayout(6, 0));

        mChooserButton.setAction(new ChooseAction(aType));

        mFileField.setEditable(false);
        mFileField.setPreferredSize(new Dimension(160, 0));

        add(mFileField, BorderLayout.CENTER);
        add(mChooserButton, BorderLayout.EAST);
    }

    public String getText()
    {
        return mFileField.getText();
    }

    public void setText(String aString)
    {
        mFileField.setText(aString);
    }

    class ChooseAction extends AbstractAction
    {
        private static final String NAME = "Choose";
        private static final String SMALL_ICON = "Open16.gif";
        private static final String LARGE_ICON = "Open24.gif";
        private static final String SHORT_DESCRIPTION = "Choose";
        private static final int MNEMONIC_KEY = 'C';

        public ChooseAction(int aType)
        {
            putValue(Action.NAME, NAME);
            putValue(Action.SMALL_ICON, Utils.getIcon(SMALL_ICON));
            putValue("largeIcon", Utils.getIcon(LARGE_ICON));
            putValue(Action.SHORT_DESCRIPTION, SHORT_DESCRIPTION);

            switch (aType)
            {
                case CHOOSE_DIRECTORY :
                    putValue(Action.LONG_DESCRIPTION, "Choose a Directory");
                    break;
                case CHOOSE_FILE :
                    putValue(Action.LONG_DESCRIPTION, "Choose a File");
                    break;
            }

            putValue(Action.MNEMONIC_KEY, new Integer(MNEMONIC_KEY));
            putValue(Action.ACTION_COMMAND_KEY, ACTION_COMMAND_KEY);
        }

        public void actionPerformed(ActionEvent e)
        {
            JFileChooser fileChooser = new JFileChooser();
            fileChooser.setMultiSelectionEnabled(false);
            fileChooser.setFileSelectionMode(JFileChooser.DIRECTORIES_ONLY);

            int returnVal = fileChooser.showDialog(FileChooserField.this,
                "Select");

            if (returnVal != JFileChooser.APPROVE_OPTION)
            {
                return;
            }

            mFileField.setText(fileChooser.getSelectedFile().getAbsolutePath());
        }
    }
}