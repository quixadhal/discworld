package mapmaker;

import java.io.*;
import java.awt.*;
import java.util.*;

/**
 * This class stores the properties that need to be stored on room objects.
 * It can wrap a different instance of itself to extract values to simulate
 * inheritance of room properties.
 */
public class RoomProperties implements Serializable
{
    /**
     * For use as the second parameter of the getProperty method.
     * Causes the check for an inherited value to be performed.
     */
    public final static int INHERIT_DO_CHECK = 0;

    /**
     * For use as the second parameter of the getProperty method.
     * Does not perform the check for an inherited value.
     */
    public final static int INHERIT_NO_CHECK = 1;

    /**
     * For use as the second parameter of the getProperty method.
     * Retrieves value from inheritable, ignoring local value.
     */
    public final static int INHERIT_FORCE = 2;

    RoomProperties mInheritedRoomProperties;

    Hashtable mPropertyHashtable = new Hashtable();
    Hashtable mPropertyTypeHashtable = new Hashtable();
    Hashtable mInheritanceHashtable = new Hashtable();

    Vector mItems;
    Vector mDayItems;
    Vector mNightItems;

    int mAutoNameNumber = 0;
    String mAutoName;
    int mAutoNumber;
    boolean mUseAutoName = true;

    String mFileName;
    String mInheritFilename;

    Color mBackgroundColor = Color.white;
    Color mForegroundColor = Color.black;

    /**
     * Sets up room properties.
     */
    public RoomProperties()
    {
        addProperty("name", String.class);
        addProperty("size", String.class);
        addProperty("light", String.class);
        addProperty("short", String.class);
        addProperty("determinate", String.class);
        addProperty("nightLong", String.class);
        addProperty("dayLong", String.class);
        addProperty("long", String.class);

        addProperty("nightChat", Chat.class);
        addProperty("dayChat", Chat.class);
        addProperty("chat", Chat.class);
    }

    /**
     * Sets which RoomProperties instance this RoomProperties instance
     * inherits from.
     *
     * @param aRoomProperties the RoomProperties instance to inherit from
     */
    public void setInheritedRoomProperties(RoomProperties aRoomProperties)
    {
        mInheritedRoomProperties = aRoomProperties;
    }

    /**
     * Adds a property to the room specifying its type and default inheritance.
     *
     * @param aProperty the property name
     * @param aClass the Java class of the property
     */
    public void addProperty(String aProperty, Class aClass)
    {
        mPropertyTypeHashtable.put(aProperty, aClass);
        mInheritanceHashtable.put(aProperty, new Boolean(false));
    }

    /**
     * Returns whether or not a property exists.
     *
     * @param aProperty the property to test for
     *
     * @return true if the property exists, false otherwise
     */
    public boolean isProperty(String aProperty)
    {
        Class type = (Class)mPropertyTypeHashtable.get(aProperty);

        if (type == null)
        {
            return false;
        }

        return true;
    }

    /**
     * Sets the value of a property.
     *
     * @param aProperty the property to set
     * @param aObject the value to set the property to
     */
    public void setProperty(String aProperty, Object aObject)
        throws NoSuchRoomPropertyException
    {
        if (!isProperty(aProperty))
        {
            throw new NoSuchRoomPropertyException(
                "Unknown property: " + aProperty);
        }

        mPropertyHashtable.put(aProperty, aObject);
    }

    /**
     * Returns the value of a property.
     *
     * @param aProperty the name of property whose value to get
     * @param aCheck whether to check for inherited values, either \
     * INHERIT_DO_CHECK, INHERIT_NO_CHECK or INHERIT_FORCE
     */
    public Object getProperty(String aProperty, int aCheck)
        throws NoSuchRoomPropertyException
    {
        if (!isProperty(aProperty))
        {
            throw new NoSuchRoomPropertyException(
                "Unknown property: " + aProperty);
        }

        Object ob = mPropertyHashtable.get(aProperty);

        switch (aCheck)
        {
            case RoomProperties.INHERIT_DO_CHECK :
            {
                if (isPropertyInherited(aProperty))
                {
                    ob = mInheritedRoomProperties.getProperty(aProperty,
                        RoomProperties.INHERIT_DO_CHECK);
                }
                break;
            }
            case RoomProperties.INHERIT_FORCE :
            {
                ob = mInheritedRoomProperties.getProperty(aProperty,
                    RoomProperties.INHERIT_DO_CHECK);
                break;
            }
            default :
            {
                break;
            }
        }

        return ob;
    }

    /**
     * Determines whether a property should be inherited from the wrapped
     * instance set by the setInheritedRoomProperties method.
     *
     * @param aProperty the property to check for
     *
     * @return true if the property should be inherited, false otherwise
     */
    public boolean isPropertyInherited(String aProperty)
        throws NoSuchRoomPropertyException
    {
        Boolean inherited = (Boolean)mInheritanceHashtable.get(aProperty);

        return inherited.booleanValue();
    }

    /**
     * Sets whether a property should be inherited from the wrapped instance
     * set by the setInheritedRoomProperties method.
     *
     * @param aProperty the property to check for
     * @param aValue whether the property should be inherited
     */
    public void setInherited(String aProperty, boolean aValue)
        throws NoSuchRoomPropertyException
    {
        if (!isProperty(aProperty))
        {
            throw new NoSuchRoomPropertyException(
                "Unknown property: " + aProperty);
        }

        mInheritanceHashtable.put(aProperty, new Boolean(aValue));
    }

    //==========================================================================
    //  Items...
    //==========================================================================

    public void setItems(Vector aItems)
    {
        Vector ourItems = new Vector();
        Vector theirItems = new Vector();

        if (aItems == null)
        {
            aItems = new Vector();
        }

        for (Enumeration e = aItems.elements(); e.hasMoreElements();)
        {
            Item item = (Item)e.nextElement();

            if (item.getInheritLevel() == 0)
            {
                ourItems.addElement(item);
            }
            else
            {
                theirItems.addElement(item);
            }
        }

        mItems = ourItems;

        if (theirItems.size() != 0)
        {
            decrementInheritLevels(theirItems);
            mInheritedRoomProperties.setItems(theirItems);
        }
    }

    public Vector getItems()
    {
        Vector items = null;

        if (mItems != null)
        {
            items  = mItems;
        }

        if (mInheritedRoomProperties != null)
        {
            Vector inheritedItems = mInheritedRoomProperties.getItems();

            if (inheritedItems == null)
            {
                inheritedItems = new Vector();
            }

            incrementInheritLevels(inheritedItems);

            if (items == null)
            {
                items = new Vector();
            }

            items.addAll(inheritedItems);
        }

        return items;
    }

    public void setDayItems(Vector aItems)
    {
        Vector ourItems = new Vector();
        Vector theirItems = new Vector();

        if (aItems == null)
        {
            aItems = new Vector();
        }

        for (Enumeration e = aItems.elements(); e.hasMoreElements();)
        {
            Item item = (Item)e.nextElement();

            if (item.getInheritLevel() == 0)
            {
                ourItems.addElement(item);
            }
            else
            {
                theirItems.addElement(item);
            }
        }

        mDayItems = ourItems;

        if (theirItems.size() != 0)
        {
            decrementInheritLevels(theirItems);
            mInheritedRoomProperties.setDayItems(theirItems);
        }
    }

    public Vector getDayItems()
    {
        Vector items = null;

        if (mDayItems != null)
        {
            items  = mDayItems;
        }

        if (mInheritedRoomProperties != null)
        {
            Vector inheritedItems = mInheritedRoomProperties.getDayItems();

            if (inheritedItems == null)
            {
                inheritedItems = new Vector();
            }

            incrementInheritLevels(inheritedItems);

            if (items == null)
            {
                items = new Vector();
            }

            items.addAll(inheritedItems);
        }

        return items;
    }

    public void setNightItems(Vector aItems)
    {
        Vector ourItems = new Vector();
        Vector theirItems = new Vector();

        if (aItems == null)
        {
            aItems = new Vector();
        }

        for (Enumeration e = aItems.elements(); e.hasMoreElements();)
        {
            Item item = (Item)e.nextElement();

            if (item.getInheritLevel() == 0)
            {
                ourItems.addElement(item);
            }
            else
            {
                theirItems.addElement(item);
            }
        }

        mNightItems = ourItems;

        if (theirItems.size() != 0)
        {
            decrementInheritLevels(theirItems);
            mInheritedRoomProperties.setNightItems(theirItems);
        }
    }

    public Vector getNightItems()
    {
        Vector items = null;

        if (mNightItems != null)
        {
            items  = mNightItems;
        }

        if (mInheritedRoomProperties != null)
        {
            Vector inheritedItems = mInheritedRoomProperties.getNightItems();

            if (inheritedItems == null)
            {
                inheritedItems = new Vector();
            }

            incrementInheritLevels(inheritedItems);

            if (items == null)
            {
                items = new Vector();
            }

            items.addAll(inheritedItems);
        }

        return items;
    }

    private void incrementInheritLevels(Vector aItems)
    {
        for (Enumeration e = aItems.elements(); e.hasMoreElements() ;)
        {
            Item item = (Item)e.nextElement();
            item.incrementInheritLevel();
        }
    }

    private void decrementInheritLevels(Vector aItems)
    {
        for (Enumeration e = aItems.elements(); e.hasMoreElements() ;)
        {
            Item item = (Item)e.nextElement();
            item.decrementInheritLevel();
        }
    }

    //==========================================================================
    //  Colors.
    //==========================================================================

    public void setBackgroundColor(Color aColor)
    {
        mBackgroundColor = aColor;
    }

    public Color getBackgroundColor()
    {
        if (mInheritedRoomProperties != null)
        {
            return mInheritedRoomProperties.getBackgroundColor();
        }

        return mBackgroundColor;
    }

    public void setForegroundColor(Color aColor)
    {
        mForegroundColor = aColor;
    }

    public Color getForegroundColor()
    {
        if (mInheritedRoomProperties != null)
        {
            return mInheritedRoomProperties.getForegroundColor();
        }

        return mForegroundColor;
    }

    //==========================================================================
    //  File name and auto naming...
    //==========================================================================

    public void setFileName(String aFileName)
    {
        mFileName = aFileName;
    }

    public String getFileName()
    {
        return mFileName;
    }

    public void setUseAutoName(boolean aUseAutoName)
    {
        mUseAutoName = aUseAutoName;
    }

    public boolean isUseAutoName()
    {
        return mUseAutoName;
    }
    public int incrementAutoNumber()
    {
        if (mInheritedRoomProperties != null)
        {
            return mInheritedRoomProperties.incrementAutoNumber();
        }

        return ++mAutoNameNumber;
    }

    public String getAutoName()
    {
        String numString;

        if (mAutoNumber < 10)
        {
            numString = "0" + mAutoNumber;
        }
        else
        {
            numString = "" + mAutoNumber;
        }

        return mInheritedRoomProperties.mAutoName + numString;
    }

    public int getAutoNameNumber()
    {
        if (mInheritedRoomProperties != null)
        {
            return mInheritedRoomProperties.getAutoNameNumber();
        }

        return mAutoNameNumber;
    }

    public void setAutoName(String aAutoName)
    {
        mAutoName = aAutoName;
    }

    public void setInheritFilename(String aInheritFilename)
    {
        mInheritFilename = aInheritFilename;
    }

    public String getInheritFilename()
    {
        if (mInheritedRoomProperties != null)
        {
            return mInheritedRoomProperties.getInheritFilename();
        }

        return mInheritFilename;
    }

    /**
     * Required for display in InheritBean.
     *
     * @return the display name for the inherit bean
     */
    public String toString()
    {
        String name = null;

        try
        {
            name = (String)getProperty("name",
                RoomProperties.INHERIT_DO_CHECK);
        }
        catch (Exception ex)
        {
            ex.printStackTrace();
        }

        return name;
    }
}