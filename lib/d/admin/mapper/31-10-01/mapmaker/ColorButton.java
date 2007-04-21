package mapmaker;

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;

public class ColorButton extends JPanel
{
    Color mColor;
    String mText;

    JPanel mColorPanel = new JPanel();
    JButton mButton = new JButton(new ChooseAction());

    JColorChooser mColorChooser = new JColorChooser();

    public ColorButton(String aText, Color aColor)
    {
        mText = aText;
        mColor = aColor;

        mColorPanel.setBackground(aColor);

        mButton.setText(aText);
        mColorPanel.setPreferredSize(new Dimension(24, 24));
        mColorPanel.setBorder(BorderFactory.createLineBorder(Color.black));

        setLayout(new BorderLayout(6, 0));

        JPanel squishPanel = new JPanel(new FlowLayout(FlowLayout.CENTER, 0, 0));
        squishPanel.add(mColorPanel);

        add(mButton, BorderLayout.CENTER);
        add(squishPanel, BorderLayout.EAST);
    }

    public Color getColor()
    {
        return mColor;
    }

    class ChooseAction extends AbstractAction
    {
        private static final String NAME = "Choose";
        private static final String SMALL_ICON = "Blank16.gif";
        private static final String LARGE_ICON = "Blank24.gif";
        private static final String SHORT_DESCRIPTION = "Choose";
        private static final String LONG_DESCRIPTION = "Choose Color";
        private static final int MNEMONIC_KEY = 'C';

        public ChooseAction()
        {
            putValue(Action.NAME, NAME);
            putValue(Action.SHORT_DESCRIPTION, SHORT_DESCRIPTION);
            putValue(Action.LONG_DESCRIPTION, LONG_DESCRIPTION);
            putValue(Action.MNEMONIC_KEY, new Integer(MNEMONIC_KEY));
            putValue(Action.ACTION_COMMAND_KEY, ACTION_COMMAND_KEY);
        }

        public void actionPerformed(ActionEvent e)
        {
            Color color = mColorChooser.showDialog(ColorButton.this,
                "Choose " + mText, mColor);

            if (color != null)
            {
                mColorPanel.setBackground(color);
                mColor = color;
            }
        }
    }
}
