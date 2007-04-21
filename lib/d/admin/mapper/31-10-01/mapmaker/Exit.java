package mapmaker;

import java.io.*;
import java.util.*;

public class Exit implements Serializable, Comparable
{
    int mDirection = -1;
    String mName;
    MapLocation mFrom;
    MapLocation mTo;
    String mDummy;
    String mType;

    public Exit(int aDirection, MapLocation aFrom, MapLocation aTo, String aType)
    {
        setDirection(aDirection);
        setFrom(aFrom);
        setTo(aTo);
        setType(aType);
    }

    public Exit(String aName, MapLocation aFrom, MapLocation aTo, String aType)
    {
        setName(aName);
        setFrom(aFrom);
        setTo(aTo);
        setType(aType);
    }

    public Exit(int aDirection, MapLocation aFrom, String aTo, String aType)
    {
        setDirection(aDirection);
        setFrom(aFrom);
        setDummy(aTo);
        setType(aType);
    }

    public Exit(String aName, MapLocation aFrom, String aTo, String aType)
    {
        setName(aName);
        setFrom(aFrom);
        setDummy(aTo);
        setType(aType);
    }

    public void setDirection(int aDirection)
    {
        mDirection = aDirection;
    }

    public int getDirection()
    {
        return mDirection;
    }

    public boolean isDirection()
    {
        return mDirection != -1;
    }

    public void setName(String aName)
    {
        mDirection = -1;
        mName = aName;
    }

    public String getName()
    {
        return mName;
    }

    public void setType(String aType)
    {
        mType = aType;
    }

    public String getType()
    {
        return mType;
    }

    public void setTo(MapLocation aTo)
    {
        mTo = aTo;
    }

    public MapLocation getTo()
    {
        return mTo;
    }

    public void setDummy(String aDummy)
    {
        mDummy = aDummy;
    }

    public String getDummy()
    {
        return mDummy;
    }

    public boolean isDummy()
    {
        return mDummy != null;
    }

    public void setFrom(MapLocation aFrom)
    {
        mFrom = aFrom;
    }

    public MapLocation getFrom()
    {
        return mFrom;
    }

    public Exit getOpposite()
    {
        if (isDummy())
        {
            return null;
        }

        return mTo.getOpposite(this);
    }

    public int compareTo(Object aExit)
    {
        Exit exit = (Exit)aExit;

        if (Direction.getExitOrder(exit.getDirection()) >
            Direction.getExitOrder(this.getDirection()))
        {
            return -1;
        }
        else if (Direction.getExitOrder(exit.getDirection()) <
            Direction.getExitOrder(this.getDirection()))
        {
            return 1;
        }

        return 0;
    }
}
