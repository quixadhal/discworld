package mapmaker;

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;

public class ExportOptionsDialog extends StandardDialog
{
    JFrame mFrame;

    JLabel mBracesLabel = new JLabel("Braces: ");
    JLabel mIndentLabel = new JLabel("Indent: ");
    JLabel mMaxColsLabel = new JLabel("Columns: ");
    JLabel mExportDirectoryLabel = new JLabel("Export Directory: ");

    FileChooserField mExportDirectoryField =
        new FileChooserField(FileChooserField.CHOOSE_DIRECTORY);

    JTextField mIndentField = new JTextField();
    JTextField mMaxColsField = new JTextField();

    JRadioButton mEndOfLineButton = new JRadioButton("End of Line");
    JRadioButton mNextLineButton = new JRadioButton("Next Line");

    ButtonGroup mButtonGroup = new ButtonGroup();

    public ExportOptionsDialog(JFrame aFrame)
    {
        super(aFrame, "Export Options");

        int height = mIndentField.getPreferredSize().height;

        mIndentField.setPreferredSize(new Dimension(50, height));
        mMaxColsField.setPreferredSize(new Dimension(50, height));

        PairedComponentPanel mainPanel = new PairedComponentPanel();

        mButtonGroup.add(mEndOfLineButton);
        mButtonGroup.add(mNextLineButton);

        mainPanel.addComponentPair(mExportDirectoryLabel,
            mExportDirectoryField);
        mainPanel.addDoubleComponentPair(mMaxColsLabel, mMaxColsField,
            mIndentLabel, mIndentField);
        mainPanel.addComponentPair(mBracesLabel, mEndOfLineButton);
        mainPanel.addHangingComponent(mNextLineButton);

        setMainPanel(mainPanel);

        OptionsOkayAction optionsOkayAction = new OptionsOkayAction();
        mIndentField.addActionListener(optionsOkayAction);
        mMaxColsField.addActionListener(optionsOkayAction);
        setOkayAction(optionsOkayAction);

        pack();
        setLocationRelativeTo(aFrame);
    }

    public void setExportOptions(ExportOptions aExportOptions)
    {
        mExportDirectoryField.setText(aExportOptions.getDirectory());
        mIndentField.setText("" + aExportOptions.getIndent());
        mMaxColsField.setText("" + aExportOptions.getCols());

        int braceType = aExportOptions.getBraceType();

        switch (braceType)
        {
            case ExportOptions.BRACE_END_OF_LINE :
                mEndOfLineButton.setSelected(true);
                break;
            case ExportOptions.BRACE_NEXT_LINE :
                mNextLineButton.setSelected(true);
                break;
        }
    }

    public ExportOptions getExportOptions()
    {
        int cols = Integer.parseInt(mMaxColsField.getText());
        int indent = Integer.parseInt(mIndentField.getText());
        String directory = mExportDirectoryField.getText();

        int braceType;

        if (mEndOfLineButton.isSelected())
        {
            braceType = ExportOptions.BRACE_END_OF_LINE;
        }
        else
        {
            braceType = ExportOptions.BRACE_NEXT_LINE;
        }

        ExportOptions exportOptions = new ExportOptions();
        exportOptions.setCols(cols);
        exportOptions.setIndent(indent);
        exportOptions.setBraceType(braceType);
        exportOptions.setDirectory(directory);

        return exportOptions;
    }

    public static ExportOptions showExportOptionsDialog(JFrame aFrame,
        ExportOptions aExportOptions)
    {
        ExportOptionsDialog optionsDialog = new ExportOptionsDialog(aFrame);
        optionsDialog.setExportOptions(aExportOptions);
        optionsDialog.setVisible(true);

        if (optionsDialog.isOkay())
        {
            return optionsDialog.getExportOptions();
        }

        return null;
    }

    class OptionsOkayAction extends OkayAction
    {
        public void actionPerformed(ActionEvent e)
        {
            try
            {
                int cols = Integer.parseInt(mMaxColsField.getText());
                int indent = Integer.parseInt(mIndentField.getText());
            }
            catch (Exception ex)
            {
                JOptionPane.showMessageDialog(ExportOptionsDialog.this,
                    "Please ensure both Columns and Indent fields "
                    + "are whole numbers.",
                    "Error", JOptionPane.ERROR_MESSAGE);
                return;
            }

            super.actionPerformed(e);
        }
    }
}