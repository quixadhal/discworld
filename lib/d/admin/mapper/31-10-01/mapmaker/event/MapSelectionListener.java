package mapmaker.event;

import java.util.*;

public interface MapSelectionListener extends EventListener
{
    public void mapSelectionChanged(MapSelectionEvent aMapSelectionEvent);
}