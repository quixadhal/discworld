package mapmaker;

import java.awt.*;

public class Linker
{
    String mAreaName;
    String mDynamicPrep;
    String mStaticPrep;
    Color mColor;

    public Linker()
    {
    }

    public Linker(String aAreaName, String aDynamicPrep, String aStaticPrep, Color aColor)
    {
        mAreaName = aAreaName;
        mDynamicPrep = aDynamicPrep;
        mStaticPrep = aStaticPrep;
        mColor = aColor;
    }

    public void setAreaName(String aAreaName)
    {
        mAreaName = aAreaName;
    }

    public String getAreaName()
    {
        return mAreaName;
    }

    public void setDynamicPrep(String aDynamicPrep)
    {
        mDynamicPrep = aDynamicPrep;
    }

    public String getDynamicPrep()
    {
        return mDynamicPrep;
    }

    public void setStaticPrep(String aStaticPrep)
    {
        mStaticPrep = aStaticPrep;
    }

    public String getStaticPrep()
    {
        return mStaticPrep;
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
        return mAreaName;
    }
}