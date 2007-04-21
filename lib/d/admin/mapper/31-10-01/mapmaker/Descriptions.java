package mapmaker;

import java.io.*;

public class Descriptions implements Serializable
{
    String mSize;
    String mLight;
    String mDeterminate;
    String mShort;
    String mLong;
    String mDayLong;
    String mNightLong;

    boolean mSizeInherited;
    boolean mLightInherited;
    boolean mDeterminateInherited;
    boolean mShortInherited;
    boolean mLongInherited;
    boolean mDayLongInherited;
    boolean mNightLongInherited;

    public Descriptions()
    {
    }

    public void setLight(String aLight)
    {
        mLight = aLight;
    }

    public String getLight()
    {
        return mLight;
    }

    public void setLightInherited(boolean aLightInherited)
    {
        mLightInherited = aLightInherited;
    }

    public boolean isLightInherited()
    {
        return mLightInherited;
    }

    public void setDeterminate(String aDeterminate)
    {
        mDeterminate = aDeterminate;
    }

    public String getDeterminate()
    {
        return mDeterminate;
    }

    public void setDeterminateInherited(boolean aDeterminateInherited)
    {
        mDeterminateInherited = aDeterminateInherited;
    }

    public boolean isDeterminateInherited()
    {
        return mDeterminateInherited;
    }

    public void setShort(String aShort)
    {
        mShort = aShort;
    }

    public String getShort()
    {
        return mShort;
    }

    public void setShortInherited(boolean aShortInherited)
    {
        mShortInherited = aShortInherited;
    }

    public boolean isShortInherited()
    {
        return mShortInherited;
    }

    public void setLong(String aLong)
    {
        mLong = aLong;
    }

    public String getLong()
    {
        return mLong;
    }

    public void setLongInherited(boolean aLongInherited)
    {
        mLongInherited = aLongInherited;
    }

    public boolean isLongInherited()
    {
        return mLongInherited;
    }

    public void setDayLong(String aDayLong)
    {
        mDayLong = aDayLong;
    }

    public String getDayLong()
    {
        return mDayLong;
    }

    public void setDayLongInherited(boolean aDayLongInherited)
    {
        mDayLongInherited = aDayLongInherited;
    }

    public boolean isDayLongInherited()
    {
        return mDayLongInherited;
    }

    public void setNightLong(String aNightLong)
    {
        mNightLong = aNightLong;
    }

    public String getNightLong()
    {
        return mNightLong;
    }

    public void setNightLongInherited(boolean aNightLongInherited)
    {
        mNightLongInherited = aNightLongInherited;
    }

    public boolean isNightLongInherited()
    {
        return mNightLongInherited;
    }

    public void setSize(String aSize)
    {
        mSize = aSize;
    }

    public String getSize()
    {
        return mSize;
    }

    public void setSizeInherited(boolean aSizeInherited)
    {
        mSizeInherited = aSizeInherited;
    }

    public boolean isSizeInherited()
    {
        return mSizeInherited;
    }
}
