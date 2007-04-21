package mapmaker;

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;

public class InheritField extends JPanel implements InheritComponent
{
    private JTextField mTextField = new JTextField();
    public JCheckBox mCheckBox = new JCheckBox("Inherit?");

    private String mInheritedValue;
    private String mValue;

    public InheritField()
    {
        setLayout(new BorderLayout(5, 0));

        mCheckBox.addActionListener(new ActionListener()
        {
            public void actionPerformed(ActionEvent e)
            {
                boolean selected = mCheckBox.isSelected();

                if (selected)
                {
                    // Record old value.
                    mValue = mTextField.getText();

                    // Insert inherited value.
                    mTextField.setText(mInheritedValue);
                }
                else
                {
                    // Restore old value.
                    mTextField.setText(mValue);
                }

                setInherited(selected);
            }
        });

        mCheckBox.setEnabled(false);

        add(mTextField, BorderLayout.CENTER);
        add(mCheckBox, BorderLayout.EAST);
    }

    public void setValues(String aInheritedValue, String aValue, boolean aInherited)
    {
        // Determine if there is a value to inherit.
        boolean enabled = (aInheritedValue != null && !aInheritedValue.equals(""));

        mCheckBox.setEnabled(enabled);

        setInherited(aInherited);

        if (aInherited)
        {
            mTextField.setText(aInheritedValue);
        }
        else
        {
            mTextField.setText(aValue);
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
        mTextField.setEnabled(!aInherited);
    }

    public boolean isInherited()
    {
        return mCheckBox.isSelected();
    }

    public void setText(String aText)
    {
        mTextField.setText(aText);
    }

    public String getText()
    {
        return mTextField.getText();
    }
}