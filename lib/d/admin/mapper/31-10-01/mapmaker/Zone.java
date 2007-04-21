package mapmaker;

import java.awt.*;

public class Zone
{
    Color mColor;
    String mName;

    public Zone()
    {
    }

    public Zone(String aName, Color aColor)
    {
        mName = aName;
        mColor = aColor;
    }

    public void setName(String aName)
    {
        mName = aName;
    }

    public String getName()
    {
        return mName;
    }
    public void setColor(Color aColor)
    {
        mColor = aColor;
    }

    public Color getColor()
    {
        return mColor;
    }

    public String toString()
    {
        return mName;
    }
}
