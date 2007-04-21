package mapmaker;

import java.awt.*;
import java.util.*;
import java.io.*;

public class MapModel implements Serializable
{
    private Vector mMapLocations;
    private Vector mInherits;
    private String mFilename;
    private Vector mZones;
    private Vector mLinkers;
    private Dimension mMapSize;
    private ExportOptions mExportOptions;

    public MapModel(Dimension aMapSize)
    {
        mMapSize = aMapSize;

        mMapLocations = new Vector();
        mInherits = new Vector();
        mZones = new Vector();
        mLinkers = new Vector();

        // TODO: Read default map options.
        mExportOptions = new ExportOptions();
        mExportOptions.setDirectory("");
        mExportOptions.setBraceType(ExportOptions.BRACE_END_OF_LINE);
        mExportOptions.setCols(80);
        mExportOptions.setIndent(2);
    }

    public void setExportOptions(ExportOptions aExportOptions)
    {
        mExportOptions = aExportOptions;
    }

    public ExportOptions getExportOptions()
    {
        return mExportOptions;
    }

    public void addLinker(Linker aLinker)
    {
        if (!mLinkers.contains(aLinker))
        {
            mLinkers.addElement(aLinker);
        }
    }

    public void replaceLinker(Linker aOldLinker, Linker aNewLinker)
    {
        mLinkers.setElementAt(aNewLinker, mLinkers.indexOf(aOldLinker));
    }

    public void removeLinker(Linker aLinker)
    {
        mLinkers.removeElement(aLinker);
    }

    public Vector getLinkers()
    {
        return mLinkers;
    }

    public void addZone(Zone aZone)
    {
        if (!mZones.contains(aZone))
        {
            mZones.addElement(aZone);
        }
    }

    public void replaceZone(Zone aOldZone, Zone aNewZone)
    {
        mZones.setElementAt(aNewZone, mZones.indexOf(aOldZone));
    }

    public void removeZone(Zone aZone)
    {
        mZones.removeElement(aZone);
    }

    public Vector getZones()
    {
        return mZones;
    }

    public void setMapSize(Dimension aMapSize)
    {
        mMapSize = aMapSize;
    }

    public Dimension getMapSize()
    {
        return mMapSize;
    }

    public void setFilename(String aFilename)
    {
        mFilename = aFilename;
    }

    public String getFilename()
    {
        return mFilename;
    }

    public void addMapLocation(MapLocation aMapLocation)
    {
        mMapLocations.add(aMapLocation);
    }

    public void removeMapLocation(MapLocation aMapLocation)
    {
        mMapLocations.remove(aMapLocation);
    }

    public Vector getMapLocations()
    {
        return mMapLocations;
    }

    public MapLocation getLocationAt(int aX, int aY)
    {
        int sizeOfMapLocations = mMapLocations.size();

        for (int i = 0; i < sizeOfMapLocations; i++)
        {
            MapLocation currentLocation = (MapLocation)mMapLocations.elementAt(i);

            Rectangle r = currentLocation.getRectangle();

            if (r.contains(aX, aY))
            {
                return currentLocation;
            }
        }

        return null;
    }

    public void addInheritable(RoomProperties aInherit)
    {
        mInherits.addElement(aInherit);
    }
/*
    public void replaceInheritable(Inherit aOldInherit, Inherit aNewInherit)
    {
        mInherits.setElementAt(aNewInherit, mInherits.indexOf(aOldInherit));
    }
*/
    public Vector getInheritables()
    {
        return mInherits;
    }
}
