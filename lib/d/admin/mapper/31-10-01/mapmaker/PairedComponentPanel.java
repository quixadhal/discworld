package mapmaker;

import java.awt.*;
import javax.swing.*;

public class PairedComponentPanel extends JPanel
{
    static int mRowCount;

    int mPixelWidth = 6;

    public PairedComponentPanel()
    {
        super(new GridBagLayout());
    }

    public PairedComponentPanel(int aPixelWidth)
    {
        super(new GridBagLayout());

        mPixelWidth = aPixelWidth;
    }

    public void addComponentPair(Component aLabel, Component aField)
    {
        add(aLabel, new GridBagConstraints(0, mRowCount, 1, 1, 0.0, 0.0,
            GridBagConstraints.CENTER, GridBagConstraints.BOTH,
            new Insets(0, 0, mPixelWidth - 1, mPixelWidth * 2), 0, 0));
        add(aField, new GridBagConstraints(1, mRowCount,
            GridBagConstraints.REMAINDER, 1, 1.0, 0.0,
            GridBagConstraints.CENTER, GridBagConstraints.HORIZONTAL,
            new Insets(0, 0, mPixelWidth - 1, 0), 0, 0));
        mRowCount++;
    }

    public void addHangingComponent(Component aComponent)
    {
        add(aComponent, new GridBagConstraints(1, mRowCount,
            GridBagConstraints.REMAINDER, 1, 1.0, 0.0,
            GridBagConstraints.CENTER, GridBagConstraints.HORIZONTAL,
            new Insets(0, 0, mPixelWidth - 1, 0), 0, 0));
        mRowCount++;
    }

    public void addComponent(Component aComponent)
    {
        add(aComponent, new GridBagConstraints(0, mRowCount,
            GridBagConstraints.REMAINDER, 1, 1.0, 0.0,
            GridBagConstraints.CENTER, GridBagConstraints.HORIZONTAL,
            new Insets(0, 0, mPixelWidth - 1, 0), 0, 0));
        mRowCount++;
    }

    public void addAreaComponentPair(Component aLabel, Component aField, int aRowSpan)
    {
        add(aLabel, new GridBagConstraints(0, mRowCount, 1, 1, 0.0, 0.0,
            GridBagConstraints.CENTER, GridBagConstraints.BOTH,
            new Insets(0, 0, mPixelWidth - 1, mPixelWidth * 2), 0, 0));
        add(aField, new GridBagConstraints(1, mRowCount,
            GridBagConstraints.REMAINDER, aRowSpan, 1.0, 0.0,
            GridBagConstraints.CENTER, GridBagConstraints.HORIZONTAL,
            new Insets(0, 0, mPixelWidth - 1, 0), 0, 0));
        mRowCount += aRowSpan;
    }

    public void addAreaComponent(Component aAreaComponent, int aRowSpan)
    {
        add(aAreaComponent, new GridBagConstraints(0, mRowCount,
            GridBagConstraints.REMAINDER, aRowSpan, 1.0, 1.0,
            GridBagConstraints.CENTER, GridBagConstraints.HORIZONTAL,
            new Insets(0, 0, mPixelWidth - 1, 0), 0, 0));
        mRowCount += aRowSpan;
    }

    public void addDoubleComponentPair(Component aLabel1, Component aField1,
        Component aLabel2, Component aField2)
    {
        add(aLabel1, new GridBagConstraints(0, mRowCount, 1, 1, 0.0, 0.0,
            GridBagConstraints.CENTER, GridBagConstraints.BOTH,
            new Insets(0, 0, mPixelWidth - 1, mPixelWidth * 2), 0, 0));
        add(aField1, new GridBagConstraints(1, mRowCount, 1, 1, 1.0, 0.0,
            GridBagConstraints.CENTER, GridBagConstraints.HORIZONTAL,
            new Insets(0, 0, mPixelWidth - 1, mPixelWidth * 2), 0, 0));
        add(aLabel2, new GridBagConstraints(2, mRowCount, 1, 1, 0.0, 0.0,
            GridBagConstraints.CENTER, GridBagConstraints.BOTH,
            new Insets(0, 0, mPixelWidth - 1, mPixelWidth * 2), 0, 0));
        add(aField2, new GridBagConstraints(3, mRowCount,
            GridBagConstraints.REMAINDER, 1, 1.0, 0.0,
            GridBagConstraints.CENTER, GridBagConstraints.HORIZONTAL,
            new Insets(0, 0, mPixelWidth - 1, 0), 0, 0));
        mRowCount++;
    }

    public void addTripleComponentPair(Component aLabel1, Component aField1,
        Component aLabel2, Component aField2, Component aLabel3, Component aField3)
    {
        add(aLabel1, new GridBagConstraints(0, mRowCount, 1, 1, 0.0, 0.0,
            GridBagConstraints.CENTER, GridBagConstraints.BOTH,
            new Insets(0, 0, mPixelWidth - 1, mPixelWidth * 2), 0, 0));
        add(aField1, new GridBagConstraints(1, mRowCount, 1, 1, 1.0, 0.0,
            GridBagConstraints.CENTER, GridBagConstraints.HORIZONTAL,
            new Insets(0, 0, mPixelWidth - 1, mPixelWidth * 2), 0, 0));
        add(aLabel2, new GridBagConstraints(2, mRowCount, 1, 1, 0.0, 0.0,
            GridBagConstraints.CENTER, GridBagConstraints.BOTH,
            new Insets(0, 0, mPixelWidth - 1, mPixelWidth * 2), 0, 0));
        add(aField2, new GridBagConstraints(3, mRowCount, 1, 1, 1.0, 0.0,
            GridBagConstraints.CENTER, GridBagConstraints.HORIZONTAL,
            new Insets(0, 0, mPixelWidth - 1, mPixelWidth * 2), 0, 0));
        add(aLabel3, new GridBagConstraints(4, mRowCount, 1, 1, 0.0, 0.0,
            GridBagConstraints.CENTER, GridBagConstraints.BOTH,
            new Insets(0, 0, mPixelWidth - 1, mPixelWidth * 2), 0, 0));
        add(aField3, new GridBagConstraints(5, mRowCount,
            GridBagConstraints.REMAINDER, 1, 1.0, 0.0,
            GridBagConstraints.CENTER, GridBagConstraints.HORIZONTAL,
            new Insets(0, 0, mPixelWidth - 1, 0), 0, 0));
        mRowCount++;
    }
}