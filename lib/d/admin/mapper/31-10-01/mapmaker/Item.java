package mapmaker;

import java.io.*;
import java.util.*;

public class Item implements Serializable
{
    Vector mShorts = new Vector();
    String mDescription;
    String mType;
    int mInheritLevel;
    Vector mExcludedIn = new Vector();

    public Item(Vector aShorts, String aDescription)
    {
        mShorts = aShorts;
        mDescription = aDescription;
    }

    public void incrementInheritLevel()
    {
        mInheritLevel++;
    }

    public void decrementInheritLevel()
    {
        mInheritLevel--;
    }

    public int setInheritLevel()
    {
        return mInheritLevel;
    }

    public int getInheritLevel()
    {
        return mInheritLevel;
    }

    public boolean isExcludedIn(MapLocation aMapLocation)
    {
        return mExcludedIn.contains(aMapLocation);
    }

    public void addExcludedIn(MapLocation aMapLocation)
    {
        mExcludedIn.addElement(aMapLocation);
    }

    public void removeExcludedIn(MapLocation aMapLocation)
    {
        mExcludedIn.removeElement(aMapLocation);
    }

    public void setType(String aType)
    {
        mType = aType;
    }

    public String getType()
    {
        return mType;
    }

    public void setShorts(Vector aShorts)
    {
        mShorts = aShorts;
    }

    public Vector getShorts()
    {
        return mShorts;
    }

    public void setDescription(String aDescription)
    {
        mDescription = aDescription;
    }

    public String getDescription()
    {
        return mDescription;
    }
}