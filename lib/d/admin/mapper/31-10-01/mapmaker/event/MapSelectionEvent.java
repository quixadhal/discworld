package mapmaker.event;

import java.util.*;

public class MapSelectionEvent extends EventObject
{
    Vector mMapSelection = new Vector();

    public MapSelectionEvent(Object aSource, Vector aMapSelection)
    {
        super(aSource);

        mMapSelection = aMapSelection;
    }

    public Vector getMapSelection()
    {
        return mMapSelection;
    }
}