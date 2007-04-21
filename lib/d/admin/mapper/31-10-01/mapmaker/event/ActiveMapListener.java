package mapmaker.event;

import java.util.*;

public interface ActiveMapListener extends EventListener
{
    public void activeMapChanged(ActiveMapEvent e);
}