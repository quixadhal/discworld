package mapmaker;

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;

public class InheritTextArea extends JPanel implements InheritComponent
{
    private JTextArea mTextArea;
    public JCheckBox mCheckBox = new JCheckBox("Inherit?");

    private String mInheritedValue;
    private String mValue;

    public InheritTextArea(int aRows, int aCols)
    {
        mTextArea = new JTextArea(aRows, aCols);

        setLayout(new BorderLayout());

        mCheckBox.addActionListener(new ActionListener()
        {
            public void actionPerformed(ActionEvent e)
            {
                boolean selected = mCheckBox.isSelected();

                if (selected)
                {
                    // Record old value.
                    mValue = mTextArea.getText();

                    // Insert inherited value.
                    mTextArea.setText(mInheritedValue);
                }
                else
                {
                    // Restore old value.
                    mTextArea.setText(mValue);
                }

                setInherited(selected);
            }
        });

        add(new JScrollPane(mTextArea), BorderLayout.CENTER);

        JPanel placerPanel = new JPanel(new FlowLayout(FlowLayout.RIGHT));
        placerPanel.add(mCheckBox);

        add(placerPanel, BorderLayout.SOUTH);

        mCheckBox.setEnabled(false);

        mTextArea.setLineWrap(true);
        mTextArea.setWrapStyleWord(true);
    }

    public void setValues(String aInheritedValue, String aValue, boolean aInherited)
    {
        // Determine if there is a value to inherit.
        boolean enabled = (aInheritedValue != null && !aInheritedValue.equals(""));
        mCheckBox.setEnabled(enabled);

        setInherited(aInherited);

        if (aInherited)
        {
            mTextArea.setText(aInheritedValue);
        }
        else
        {
            mTextArea.setText(aValue);
        }

        mInheritedValue = aInheritedValue;
        mValue = aValue;
    }

    public String getInheritedValue()
    {
        return mInheritedValue;
    }

    public void setInherited(boolean aInherited)
    {
        mCheckBox.setSelected(aInherited);
        mTextArea.setEnabled(!aInherited);
    }

    public boolean isInherited()
    {
        return mCheckBox.isSelected();
    }

    public void setText(String aText)
    {
        mTextArea.setText(aText);
    }

    public String getText()
    {
        return mTextArea.getText();
    }
}