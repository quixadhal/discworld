package mapmaker;

import java.io.*;
import java.util.*;

public class Chat implements Serializable
{
    String mMinimum;
    String mMaximum;
    String mType;
    Vector mChats;

    public Chat()
    {
    }

    public Chat(String aMinimum, String aMaximum, Vector aChats)
    {
        mMinimum = aMinimum;
        mMaximum = aMaximum;
        mChats = aChats;
    }

    public void setType(String aType)
    {
        mType = aType;
    }

    public String getType()
    {
        return mType;
    }

    public void setMinimum(String aMinimum)
    {
        mMinimum = aMinimum;
    }

    public String getMinimum()
    {
        return mMinimum;
    }

    public void setMaximum(String aMaximum)
    {
        mMaximum = aMaximum;
    }

    public String getMaximum()
    {
        return mMaximum;
    }

    public void setChats(Vector aChats)
    {
        mChats = aChats;
    }

    public Vector getChats()
    {
        return mChats;
    }
}
