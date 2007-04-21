package mapmaker;

import java.awt.*;
import javax.swing.*;

public class DialogPanel extends JPanel
{
    PairedComponentPanel mMainPanel = new PairedComponentPanel();
    ButtonPanel mButtonPanel = new ButtonPanel();

    int mPixelWidth = 6;

    public DialogPanel()
    {
        super(new BorderLayout(0, 12));

        add(mMainPanel, BorderLayout.CENTER);
        add(mButtonPanel, BorderLayout.SOUTH);

        setBorder(BorderFactory.createEmptyBorder(12, 12, 11, 11));
    }

    public DialogPanel(int aPixelWidth)
    {
        super(new BorderLayout(0, aPixelWidth * 2));

        add(mMainPanel, BorderLayout.CENTER);
        add(mButtonPanel, BorderLayout.SOUTH);

        setBorder(BorderFactory.createEmptyBorder(aPixelWidth * 2, aPixelWidth * 2,
            aPixelWidth * 2 - 1, aPixelWidth * 2 - 1));
    }

    public void addComponentPair(Component aLabel, Component aField)
    {
        mMainPanel.addComponentPair(aLabel, aField);
    }

    public void addHangingComponent(Component aComponent)
    {
        mMainPanel.addHangingComponent(aComponent);
    }

    public void addComponent(Component aComponent)
    {
        mMainPanel.addComponent(aComponent);
    }

    public void setMainPanel(Component aPanel)
    {
        add(aPanel, BorderLayout.CENTER);
    }

    public void addButton(Component aButton)
    {
        mButtonPanel.addButton(aButton);
    }

    public void addTripleComponentPair(Component aLabel1, Component aField1,
        Component aLabel2, Component aField2, Component aLabel3, Component aField3)
    {
        mMainPanel.addTripleComponentPair(aLabel1, aField1, aLabel2, aField2,
            aLabel3, aField3);
    }
}