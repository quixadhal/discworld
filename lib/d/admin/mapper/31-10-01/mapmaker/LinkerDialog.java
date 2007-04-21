package mapmaker;

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;

public class LinkerDialog extends StandardDialog
{
    JLabel mAreaNameLabel = new JLabel("Area Name: ");
    JTextField mAreaNameField = new JTextField();
    JLabel mDynamicPrepositionLabel = new JLabel("Dynamic Preposition: ");
    JTextField mDynamicPrepositionField = new JTextField();
    JLabel mStaticPrepositionLabel = new JLabel("Static Preposition: ");
    JTextField mStaticPrepositionField = new JTextField();
    JLabel mColorLabel = new JLabel("Colour: ");
    ColorComboBox mColorComboBox = new ColorComboBox();

    Linker mLinker;

    public LinkerDialog(JFrame aFrame)
    {
        super(aFrame, "Linker");

        DialogPanel dialogPanel = (DialogPanel)getContentPane();

        int prefHeight = mDynamicPrepositionField.getPreferredSize().height;
        mDynamicPrepositionField.setPreferredSize(new Dimension(40, prefHeight));
        mStaticPrepositionField.setPreferredSize(new Dimension(40, prefHeight));

        dialogPanel.addComponentPair(mAreaNameLabel, mAreaNameField);
        dialogPanel.addTripleComponentPair(mDynamicPrepositionLabel,
            mDynamicPrepositionField, mStaticPrepositionLabel,
            mStaticPrepositionField, mColorLabel, mColorComboBox);

        LinkerOkayAction linkerOkayAction = new LinkerOkayAction();
        mAreaNameField.addActionListener(linkerOkayAction);
        mDynamicPrepositionField.addActionListener(linkerOkayAction);
        mStaticPrepositionField.addActionListener(linkerOkayAction);
        setOkayAction(linkerOkayAction);

        pack();
        setLocationRelativeTo(aFrame);
    }

    public void setLinker(Linker aLinker)
    {
        mLinker = aLinker;
    }

    public Linker getLinker()
    {
        if (mLinker == null)
        {
            mLinker = new Linker();
        }

        mLinker.setAreaName(mAreaNameField.getText());
        mLinker.setDynamicPrep(mDynamicPrepositionField.getText());
        mLinker.setStaticPrep(mStaticPrepositionField.getText());
        mLinker.setColor((Color)mColorComboBox.getSelectedItem());

        return mLinker;
    }

    class LinkerOkayAction extends OkayAction
    {
        public void actionPerformed(ActionEvent e)
        {
            if (!Utils.isFilledIn(mAreaNameField))
            {
                JOptionPane.showMessageDialog(LinkerDialog.this,
                    "Please enter an area name for this linker in the "
                    + "area name field.\n",
                    "Error", JOptionPane.ERROR_MESSAGE);
                return;
            }

            if (!Utils.isFilledIn(mDynamicPrepositionField))
            {
                JOptionPane.showMessageDialog(LinkerDialog.this,
                    "Please enter a dynamic preposition for this linker in the "
                    + "dynamic preposition field.\n",
                    "Error", JOptionPane.ERROR_MESSAGE);
                return;
            }

            if (!Utils.isFilledIn(mDynamicPrepositionField))
            {
                JOptionPane.showMessageDialog(LinkerDialog.this,
                    "Please enter a static preposition for this linker in the "
                    + "static preposition field.\n",
                    "Error", JOptionPane.ERROR_MESSAGE);
                return;
            }

            super.actionPerformed(e);
        }
    }

    public static Linker showNewDialog(JFrame aFrame)
    {
        LinkerDialog linkerDialog = new LinkerDialog(aFrame);
        linkerDialog.setVisible(true);

        if (linkerDialog.isOkay())
        {
            return linkerDialog.getLinker();
        }

        return null;
    }

    public static Linker showEditDialog(JFrame aFrame, Linker aLinker)
    {
        LinkerDialog linkerDialog = new LinkerDialog(aFrame);
        linkerDialog.setLinker(aLinker);

        if (aLinker != null)
        {
            linkerDialog.mAreaNameField.setText(aLinker.getAreaName());
            linkerDialog.mColorComboBox.setSelectedItem(aLinker.getColor());
            linkerDialog.mDynamicPrepositionField.setText(aLinker.getDynamicPrep());
            linkerDialog.mStaticPrepositionField.setText(aLinker.getStaticPrep());
        }

        linkerDialog.setVisible(true);

        if (linkerDialog.isOkay())
        {
            return linkerDialog.getLinker();
        }

        return null;
    }
}