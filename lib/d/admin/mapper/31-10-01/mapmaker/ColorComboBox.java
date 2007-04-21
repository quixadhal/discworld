package mapmaker;

import java.awt.*;
import javax.swing.*;

public class ColorComboBox extends JComboBox
{
    final static Color[] mColors = {
        // Replace with resourse bundle read or color chooser.
        Color.black, Color.white,
        new Color(Integer.parseInt("003300", 16)),
        new Color(Integer.parseInt("006600", 16)),
        new Color(Integer.parseInt("009900", 16)),
        new Color(Integer.parseInt("00CC00", 16)),
        new Color(Integer.parseInt("000066", 16)),
        new Color(Integer.parseInt("0066FF", 16)),
        new Color(Integer.parseInt("0099FF", 16)),
        new Color(Integer.parseInt("00CCFF", 16)),
        new Color(Integer.parseInt("993300", 16)),
        new Color(Integer.parseInt("996600", 16)),
        new Color(Integer.parseInt("999900", 16)),
        new Color(Integer.parseInt("CC6600", 16)),
        new Color(Integer.parseInt("FFCC00", 16)),
        new Color(Integer.parseInt("FFFF00", 16)),
        new Color(Integer.parseInt("FFFF00", 16)),
        new Color(Integer.parseInt("FFFF99", 16)),
        new Color(Integer.parseInt("FF0000", 16)),
        new Color(Integer.parseInt("CC0000", 16)),
        new Color(Integer.parseInt("FF6600", 16)),
        new Color(Integer.parseInt("FF9933", 16)) };

    public ColorComboBox()
    {
        super(mColors);

        setRenderer(new ColorComboBoxEditor());
    }
}

class ColorComboBoxEditor extends DefaultListCellRenderer
{
    public Component getListCellRendererComponent(JList aList, Object aValue,
        int aIndex, boolean aIsSelected, boolean aCellHasFocus)
    {
        JLabel defaultRenderer = (JLabel)super.getListCellRendererComponent(aList,
            aValue, aIndex, aIsSelected, aCellHasFocus);

        ColorLabel colorLabel = new ColorLabel((Color)aValue);
        colorLabel.setOpaque(true);
        colorLabel.setBackground(defaultRenderer.getBackground());

        return colorLabel;
    }
}

class ColorLabel extends JLabel
{
    Color mColor;

    public ColorLabel(Color aColor)
    {
        mColor = aColor;
        setPreferredSize(new Dimension(24, 24));
    }

    public void paintComponent(Graphics g)
    {
        super.paintComponent(g);

        Dimension d = getSize();

        int height = (int)d.getHeight() - 8;
        int width = (int)d.getWidth() - 8;

        g.setColor(mColor);
        g.fillRect(4, 4, width, height);

        g.setColor(Color.black);
        g.drawRect(4, 4, width, height);
    }
}
