package mapmaker;

import java.net.*;
import javax.swing.*;
import javax.swing.text.*;

public class Utils
{
    public static boolean isNumberString(JTextComponent aTextField)
    {
        String string = aTextField.getText();

        try
        {
            int num = Integer.parseInt(string);
        }
        catch (Exception e)
        {
            return false;
        }

        return true;
    }

    public static boolean isFilledIn(JTextComponent aTextField)
    {
        String string = aTextField.getText();

        if (string == null || string.compareTo("") == 0)
        {
            return false;
        }

        return true;
    }

    public static boolean isValidAutoName(JTextComponent aTextField)
    {
        String string = aTextField.getText();
        char[] chars = string.toCharArray();

        if (!isFilledIn(aTextField))
        {
            return false;
        }

        for (int i = 0; i < chars.length; i++)
        {
            if (!Character.isLetter(chars[i]) && chars[i] != '_')
            {
                return false;
            }
        }

        return true;
    }

    public static boolean isValidFileName(JTextComponent aTextField)
    {
        String string = aTextField.getText();
        char[] chars = string.toCharArray();

        if (!isFilledIn(aTextField))
        {
            return false;
        }

        for (int i = 0; i < chars.length; i++)
        {
            if (!Character.isLetterOrDigit(chars[i]) && chars[i] != '_')
            {
                return false;
            }
        }

        return true;
    }

    public static ImageIcon getIcon(String name)
    {
        String imagePath = "/images/" + name;
        Utils utils = new Utils();
        URL url = utils.getClass().getResource(imagePath);
        if (url != null)
        {
            return new ImageIcon(url);
        }
        return null;
    }
}