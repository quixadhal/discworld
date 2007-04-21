package mapmaker;

import javax.swing.*;
import javax.swing.event.*;

public class MapWindow extends JInternalFrame
{
    static int mWindowNo = 0;

    MapView mMapView;
    MainFrame mFrame;
    JPanel mMapPanel = new JPanel();

    public MapWindow(MainFrame aFrame, String aTitle)
    {
        super("Map " + (mWindowNo + 1), true, true, true, true);

        if (aTitle != null)
        {
            setTitle(aTitle);
        }

        setDefaultCloseOperation(WindowConstants.DISPOSE_ON_CLOSE);

        setSize(300, 200);

        setLayer(JDesktopPane.DEFAULT_LAYER);

        setLocation(30 + 30 * (mWindowNo % 6), 30 + 30 * (mWindowNo % 6));

        setContentPane(new JScrollPane(mMapPanel));

        final MapWindow thisWindow = this;

        addInternalFrameListener(new InternalFrameAdapter()
        {
            public void internalFrameActivated(InternalFrameEvent e)
            {
                mFrame.setActiveWindow(thisWindow);
            }

            public void internalFrameDeactivated(InternalFrameEvent e)
            {
                mFrame.setActiveWindow(null);
            }
        });

        mFrame = aFrame;
        mWindowNo++;
    }

    public void setMapView(MapView aMapView)
    {
        mMapView = aMapView;
        mMapPanel.removeAll();
        mMapPanel.add(mMapView);
    }

    public void setSelected(boolean aValue) throws java.beans.PropertyVetoException
    {
        super.setSelected(aValue);

        ((MainFrame)mFrame).refreshUndoRedo();
    }

    public MapView getMapView()
    {
        return mMapView;
    }
}