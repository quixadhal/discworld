package mapmaker.event;

import java.util.*;

import mapmaker.*;

public class ActiveMapEvent extends EventObject
{
    MapWindow mMapWindow;

    public ActiveMapEvent(Object aSource, MapWindow aMapWindow)
    {
        super(aSource);

        mMapWindow = aMapWindow;
    }

    public MapWindow getActiveMapWindow()
    {
        return mMapWindow;
    }
}
