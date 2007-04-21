package mapmaker;

import java.awt.*;
import java.util.*;
import javax.swing.*;

public class ButtonPanel extends JPanel
{
    JPanel mSizePanel;

    Vector mComponents = new Vector();

    int mPixelWidth = 6;

    public ButtonPanel()
    {
        super(new FlowLayout(FlowLayout.RIGHT, 0, 0));

        mSizePanel = new JPanel(new GridLayout(1, 0, mPixelWidth - 1, 0));
        add(mSizePanel);
    }

    public ButtonPanel(int aPixelWidth)
    {
        super(new FlowLayout(FlowLayout.RIGHT, 0, 0));

        mPixelWidth = aPixelWidth;

        mSizePanel = new JPanel(new GridLayout(1, 0, mPixelWidth - 1, 0));
        add(mSizePanel);
    }

    /**
     * This method adds buttons in reverse, so that additional buttons which
     * are added appear to the left of the Okay and Cancel buttons.
     *
     * @param aButton the button to add
     */
    public void addButton(Component aButton)
    {
        mComponents.addElement(aButton);

        mSizePanel.removeAll();

        for (int i = mComponents.size() - 1; i >= 0; i--)
        {
            mSizePanel.add((Component)mComponents.elementAt(i));
        }
    }
}