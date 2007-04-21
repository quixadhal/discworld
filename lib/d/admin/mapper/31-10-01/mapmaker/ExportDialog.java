package mapmaker;

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;

public class ExportDialog extends StandardDialog
{
    public final static int EXPORT_LPC = 0;
    public final static int EXPORT_JPG = 1;
    public final static int EXPORT_SVG = 2;

    JFrame mFrame;

    JLabel mExportDirectoryLabel = new JLabel("Export Directory: ");
    FileChooserField mExportDirectoryField =
        new FileChooserField(FileChooserField.CHOOSE_DIRECTORY);

    JLabel mExportAsLabel = new JLabel("Export As: ");

    ButtonGroup mExportAsGroup = new ButtonGroup();

    JRadioButton mLPCButton = new JRadioButton("LPC");
    JRadioButton mJPGButton = new JRadioButton("JPG");
    JRadioButton mSVGButton = new JRadioButton("SVG");

    JButton mOptionsButton;

    public ExportDialog(JFrame aFrame)
    {
        super(aFrame, "Export");

        mFrame = aFrame;

        mOptionsButton = new JButton(((MainFrame)mFrame).mExportOptionsAction);
        mOptionsButton.setText("Options...");

        mExportAsGroup.add(mLPCButton);
        mExportAsGroup.add(mJPGButton);
        //mExportAsGroup.add(mSVGButton);

        JPanel radioPanel = new JPanel(new FlowLayout(FlowLayout.LEFT, 6, 0));
        radioPanel.add(mLPCButton);
        radioPanel.add(mJPGButton);
        //radioPanel.add(mSVGButton);

        mDialogPanel.addComponentPair(mExportAsLabel, radioPanel);
        mDialogPanel.addComponentPair(mExportDirectoryLabel, mExportDirectoryField);

        mLPCButton.setSelected(true);
        setOkayAction(new ExportOkayAction());

        mDialogPanel.addButton(mOptionsButton);

        pack();
        setLocationRelativeTo(aFrame);
    }

    public String getDirectory()
    {
        return mExportDirectoryField.getText();
    }

    public int getExportType()
    {
        if (mLPCButton.isSelected())
        {
            return ExportDialog.EXPORT_LPC;
        }
        else
        {
            return ExportDialog.EXPORT_JPG;
        }
    }

    public static ExportDialog showExportDialog(JFrame aFrame)
    {
        ExportDialog exportDialog = new ExportDialog(aFrame);

        // Read default directory etc?

        exportDialog.setVisible(true);

        return exportDialog;
    }

    class ExportOkayAction extends OkayAction
    {
        public void actionPerformed(ActionEvent e)
        {
            if (!Utils.isFilledIn(mExportDirectoryField.mFileField))
            {
                JOptionPane.showMessageDialog(ExportDialog.this,
                    "Please select the directory you wish to export to.\n",
                    "Error", JOptionPane.ERROR_MESSAGE);
                return;
            }

            super.actionPerformed(e);
        }
    }
}