package mapmaker;

import java.awt.*;
import java.awt.event.*;
import java.awt.geom.*;
import javax.swing.undo.*;
import javax.swing.event.*;
import javax.swing.*;
import java.util.*;

import mapmaker.event.*;

import com.sun.glf.goodies.RadialGradientPaint;

public class MapView extends JPanel
{
    // The modes this view can be in.
    public final static int MODE_SELECT_AND_DRAG = 0;
    private SelectAdapter mSelectAdapter = new SelectAdapter(this);

    // The model we're wrapping.
    private MapModel mMapModel;

    // The rectangle dragged out for selecting locations.
    private Rectangle mSelectionRectangle;

    // Store possible link ups.
    private LinkUpStore mLinkUpStore = new LinkUpStore();

    // The currently selected link up group.
    private int mCurrentLinkUpGroup;

    // The selected map locations.
    private MapSelection mMapSelection = new MapSelection();

    // Probably shouldn't need these and do everything with listeners...
    public MainFrame mMainFrame;
    private InheritBean mInheritBean;
    private ExitBean mExitBean;

    // Undo/redo support.
    public UndoManager mUndoManager = new UndoManager()
    {
        public boolean addEdit(UndoableEdit aUndoableEdit)
        {
            boolean retValue = super.addEdit(aUndoableEdit);
            mMainFrame.refreshUndoRedo();
            return retValue;
        }

        public void undo()
        {
            super.undo();
            mMainFrame.refreshUndoRedo();
        }

        public void redo()
        {
            super.redo();
            mMainFrame.refreshUndoRedo();
        }
    };
    public CompoundEdit mCompoundEdit = new CompoundEdit();

    Zone mDisplayZone = null;
    Linker mDisplayLinker = null;

    public MapView(MapModel aMapModel, MainFrame aMainFrame,
        InheritBean aInheritBean, ExitBean aExitBean)
    {
        mMapModel = aMapModel;

        setMapSize(mMapModel.getMapSize());

        mInheritBean = aInheritBean;
        mMainFrame = aMainFrame;
        mExitBean = aExitBean;

        setBackground(Color.white);

        this.addMouseListener(mSelectAdapter);
        this.addMouseMotionListener(mSelectAdapter);
    }

    public void setMapSize(Dimension aMapSize)
    {
        setSize(aMapSize);
        setPreferredSize(aMapSize);
        mMapModel.setMapSize(aMapSize);
    }

    public MapSelection getMapSelection()
    {
        return mMapSelection;
    }

    public void setDisplayZone(Zone aZone)
    {
        mDisplayZone = aZone;
    }

    public void setDisplayLinker(Linker aLinker)
    {
        mDisplayLinker = aLinker;
    }

    public void setMode(int aMode)
    {
        this.removeMouseListener(mSelectAdapter);
        this.removeMouseMotionListener(mSelectAdapter);

        switch (aMode)
        {
            case MODE_SELECT_AND_DRAG :
                 this.addMouseListener(mSelectAdapter);
                 this.addMouseMotionListener(mSelectAdapter);
                 break;
        }
    }

    public MapModel getMapModel()
    {
        return mMapModel;
    }

    public void setSelectionRectangle(Rectangle aSelectionRectangle)
    {
        mSelectionRectangle = aSelectionRectangle;
    }

    /**
     * This method performs an edit and records it in the current compound edit.
     *
     * @param aEdit the edit to perform and record
     */
    public void doEdit(UndoableEdit aEdit)
    {
        aEdit.redo();
        mCompoundEdit.addEdit(aEdit);
    }

    public void addCompoundEdit()
    {
        mCompoundEdit.end();
        mUndoManager.addEdit(mCompoundEdit);
        mCompoundEdit = new CompoundEdit();
    }

    public void clearExits(MapLocation aLocation)
    {
        aLocation.clearExits();
    }

    public void clearExitsToUnselected(MapLocation aLocation)
    {
        Vector exits = aLocation.getExits();

        for (Enumeration e = exits.elements(); e.hasMoreElements();)
        {
            Exit exit = (Exit)e.nextElement();

            // Check if the exit has a destination
            if (exit.isDummy())
            {
                continue;
            }

            MapLocation to = exit.getTo();

            if (!mMapSelection.isSelected(to))
            {
                RemoveExitEdit fromEdit = new RemoveExitEdit(aLocation, exit);
                RemoveExitEdit toEdit = new RemoveExitEdit(to, exit.getOpposite());

                doEdit(fromEdit);
                doEdit(toEdit);
            }
        }
    }

    // Works out what link ups are available to a location.
    public Vector getLinkUpsForLocation(MapLocation aLocation)
    {
        Vector linkUps = new Vector();

        Rectangle linkUpRectangle = aLocation.getLinkUpRectangle();

        Vector mapLocations = mMapModel.getMapLocations();
        int sizeOfMapLocations = mapLocations.size();

        // Store the location's cardinal handles.
        Rectangle[] ourHandles = aLocation.getHandles();

        for (int i = 0; i < sizeOfMapLocations; i++)
        {
            MapLocation location = (MapLocation)mapLocations.elementAt(i);

            // First check if the other location is within linkup range.
            if (linkUpRectangle.intersects(location.getLinkUpRectangle()))
            {
                // Don't check with self.
                if (location == aLocation)
                {
                    continue;
                }

                // Don't check with other selected locations.
                if (mMapSelection.isSelected(location))
                {
                    continue;
                }

                // Now check if any opposing cardinal points are in range.
                Rectangle[] theirHandles = location.getHandles();
                for (int j = 0; j < 8; j++)
                {
                    if (ourHandles[j].intersects(theirHandles[Direction.getOppoisite(j)]))
                    {
                        // Add the link up.
                        linkUps.addElement(new LinkUp(aLocation, location, j));
                    }
                }
            }
        }

        return linkUps;
    }

    public void refreshLinkUpStore()
    {
        mLinkUpStore = this.getLinkUpGroupsForSelection();
    }

    // Works out what link up alternatives are available to a selection.
    public LinkUpStore getLinkUpGroupsForSelection()
    {
        Vector linkUps = new Vector();

        Vector selectedLocations = mMapSelection.getSelected();
        int sizeOfSelectedLocations = selectedLocations.size();

        // Gather together all the link ups.
        for (int i = 0; i < sizeOfSelectedLocations; i++)
        {
            MapLocation location = (MapLocation)selectedLocations.elementAt(i);

            linkUps.addAll(this.getLinkUpsForLocation(location));
        }

        // Sort the link ups into groups based on the translation they require
        // to be peformed.

        LinkUpStore linkUpStore = new LinkUpStore();
        int sizeOfLinkUps = linkUps.size();

        for (int i = 0; i < sizeOfLinkUps; i++)
        {
            LinkUp linkUp = (LinkUp)linkUps.elementAt(i);

            linkUpStore.addLinkUp(linkUp);
        }

        return linkUpStore;
    }

    public void doLinkUps()
    {
        Vector linkUps = mLinkUpStore.getGroupAt(mCurrentLinkUpGroup);

        if (linkUps == null)
        {
            return;
        }

        int sizeOfLinkUps = linkUps.size();

        // Locations which have been moved to link up.
        Vector done = new Vector();
        Point translation = ((LinkUp)linkUps.elementAt(0)).mTranslation;

        for (int i = 0; i < sizeOfLinkUps; i++)
        {
            LinkUp linkUp = (LinkUp)linkUps.elementAt(i);

            connectExits(linkUp);

            if (!done.contains(linkUp.mSelectedLocation))
            {
                performLinkUp(linkUp);
            }

            done.addElement(linkUp.mSelectedLocation);
        }

        // Move the selected locations with no link ups with the link ups.
        Vector selectedLocations = mMapSelection.getSelected();
        int sizeOfSelected = selectedLocations.size();

        for (int i = 0; i < sizeOfSelected; i++)
        {
            MapLocation mapLocation = (MapLocation)selectedLocations.elementAt(i);

            if (!done.contains(mapLocation))
            {
                setLocation(mapLocation, mapLocation.getX() - translation.x,
                    mapLocation.getY() - translation.y);
            }

            done.addElement(mapLocation);
        }

        mExitBean.readSelection();
    }

    public void performLinkUp(LinkUp aLinkUp)
    {
        int translateX = aLinkUp.mTranslation.x;
        int translateY = aLinkUp.mTranslation.y;

        setLocation(aLinkUp.mSelectedLocation, aLinkUp.mSelectedX - translateX,
            aLinkUp.mSelectedY - translateY);
    }

    public void setLocation(MapLocation aLocation, int aX, int aY)
    {
        mCompoundEdit.addEdit(new TranslateLocationEdit(aLocation,
            new Point(aLocation.mTempX, aLocation.mTempY),
            new Point(aX, aY)));

        aLocation.setX(aX);
        aLocation.setY(aY);
    }
/*
    public void setLocation(MapLocation aLocation, int aX, int aY)
    {
        TranslateLocationEdit edit = new TranslateLocationEdit(aLocation,
            new Point(aLocation.mTempX, aLocation.mTempY), new Point(aX, aY));

        doEdit(edit);
    }
*/
    public void connectExits(LinkUp aLinkUp)
    {
        Exit to = new Exit(aLinkUp.mDirection, aLinkUp.mSelectedLocation,
            aLinkUp.mUnselectedLocation, "path");
        Exit from = new Exit(Direction.getOppoisite(aLinkUp.mDirection),
            aLinkUp.mUnselectedLocation, aLinkUp.mSelectedLocation, "path");

        AddExitEdit toEdit = new AddExitEdit(aLinkUp.mSelectedLocation, to);
        AddExitEdit fromEdit = new AddExitEdit(aLinkUp.mUnselectedLocation, from);

        doEdit(toEdit);
        doEdit(fromEdit);
    }

    public void paintComponent(Graphics aGraphics)
    {
        super.paintComponent(aGraphics);

        Graphics2D g = (Graphics2D)aGraphics;

        Vector mapLocations = mMapModel.getMapLocations();
        int sizeOfMapLocations = mapLocations.size();

        // Paint zone
        if (mDisplayZone != null)
        {
            // Paint the zone of the map locations.
            for (int i = 0; i < sizeOfMapLocations; i++)
            {
                MapLocation currentLocation = (MapLocation)mapLocations.elementAt(i);

                if (currentLocation.isInZone(mDisplayZone))
                {
                    RadialGradientPaint paint = new RadialGradientPaint(
                        currentLocation.getZoneRectangle(), mDisplayZone.getColor(),
                        new Color(255, 255, 255, 0));
                    g.setPaint(paint);
                    g.fill(currentLocation.getZoneRectangle());
                }
            }
        }

        // Paint linker
        if (mDisplayLinker != null)
        {
            // Paint the zone of the map locations.
            for (int i = 0; i < sizeOfMapLocations; i++)
            {
                MapLocation currentLocation = (MapLocation)mapLocations.elementAt(i);

                if (currentLocation.isInLinker(mDisplayLinker))
                {
                    g.setPaint(mDisplayLinker.getColor());
                    g.fill(currentLocation.getLinkerRectangle());
                }
            }
        }

        // Paint the background of the map locations.
        for (int i = 0; i < sizeOfMapLocations; i++)
        {
            MapLocation currentLocation = (MapLocation)mapLocations.elementAt(i);

            currentLocation.renderBackground(g);
        }

        // Paint the foreground of the map locations.
        for (int i = 0; i < sizeOfMapLocations; i++)
        {
            MapLocation currentLocation = (MapLocation)mapLocations.elementAt(i);

            currentLocation.renderForeground(g);
/*
            g.setPaint(Color.blue);
            g.setStroke(new BasicStroke(1f));
            g.draw(currentLocation.getRectangle());
*/
        }

        mMapSelection.render(g);

        // Draw preview link ups.
        Vector linkUps = mLinkUpStore.getGroupAt(mCurrentLinkUpGroup);
        if (linkUps != null)
        {
            int sizeOfLinkUps = linkUps.size();

            for (int i = 0; i < sizeOfLinkUps; i++)
            {
                LinkUp linkUp = (LinkUp)linkUps.elementAt(i);

                Point dragPoint = linkUp.mSelectedLocation.getCardinals()
                      [linkUp.mDirection];
                Point linkPoint = (Point)linkUp.mUnselectedLocation.getCardinals()
                      [Direction.getOppoisite(linkUp.mDirection)].clone();

                linkPoint.translate(-2, -2);

                g.setPaint(Color.black);

                g.fill(new Rectangle(linkPoint, new Dimension(4, 4)));
            }
        }

        // Draw the selection rectangle.
        if (mSelectionRectangle != null)
        {
            g.setPaint(Color.gray);
            g.setStroke(new BasicStroke(1.f, BasicStroke.CAP_BUTT,
                BasicStroke.JOIN_BEVEL, 8.f, new float[]{ 2.f, 2.f }, 0.f));
            g.draw(mSelectionRectangle);
        }
    }

    //==========================================================================
    // Cycles through link up groups so link ups can be selected.
    // To be called from the key listener when locations are being dragged.
    //==========================================================================

    public void cycleLinkUpGroup()
    {
        mCurrentLinkUpGroup++;

        if (mCurrentLinkUpGroup >= mLinkUpStore.getNoOfGroups())
        {
            mCurrentLinkUpGroup = 0;
        }

        this.repaint();
    }

    public void deleteSelectedLocations()
    {
        Vector selectedLocations = mMapSelection.getSelected();
        int sizeOfSelectedLocations = selectedLocations.size();

        // Gather together all the link ups.
        for (int i = 0; i < sizeOfSelectedLocations; i++)
        {
            MapLocation location = (MapLocation)selectedLocations.elementAt(i);

            RemoveLocationEdit edit = new RemoveLocationEdit(location);
            doEdit(edit);
        }

        addCompoundEdit();

        mMapSelection.clearSelected();

        this.repaint();
    }

    //==========================================================================
    // Store selected map locations in here.
    //==========================================================================

    class MapSelection
    {
        private Color mSelectionColor = Color.red;
        private Stroke mSelectionStroke = new BasicStroke(1f);
        private Vector mSelectedMapLocations = new Vector();

        public void setSelected(Vector aSelectedMapLocations)
        {
            mSelectedMapLocations = aSelectedMapLocations;

            fireSelectionChanged();
        }

        public void addSelected(MapLocation aMapLocation)
        {
            if (!mSelectedMapLocations.contains(aMapLocation))
            {
                mSelectedMapLocations.addElement(aMapLocation);
                fireSelectionChanged();
            }
        }

        public void removeSelected(MapLocation aMapLocation)
        {
            mSelectedMapLocations.removeElement(aMapLocation);

            fireSelectionChanged();
        }

        public boolean isSelected(MapLocation aMapLocation)
        {
            return mSelectedMapLocations.contains(aMapLocation);
        }

        public Vector getSelected()
        {
            return mSelectedMapLocations;
        }

        public void clearSelected()
        {
            mSelectedMapLocations = new Vector();

            fireSelectionChanged();
        }

        public void addMapSelectionListener(MapSelectionListener listener)
        {
            listenerList.add(MapSelectionListener.class, listener);
        }

        public void removeMapSelectionListener(MapSelectionListener listener)
        {
            listenerList.remove(MapSelectionListener.class, listener);
        }

        protected void fireSelectionChanged()
        {
            Object[] listeners = listenerList.getListenerList();

            MapSelectionEvent mapSelectionEvent = new MapSelectionEvent(this,
                mSelectedMapLocations);

            for (int i = listeners.length - 2; i >= 0; i -= 2)
            {
                if (listeners[i] == MapSelectionListener.class)
                {
                    ((MapSelectionListener)listeners[i + 1]).mapSelectionChanged(mapSelectionEvent);
                }
            }
        }

        public void render(Graphics2D g)
        {
            int sizeOfSelectedMapLocations = mSelectedMapLocations.size();

            for (int i = 0; i < sizeOfSelectedMapLocations; i++)
            {
                MapLocation currentLocation = (MapLocation)mSelectedMapLocations.elementAt(i);
                g.setPaint(mSelectionColor);
                g.setStroke(mSelectionStroke);
                g.draw(currentLocation.getRectangle());
            }
        }
    }

    //==========================================================================
    // Class for storing groups of link ups. These link ups are grouped by the
    // transform which they need to go through to 'link up' with other
    // locations.
    //==========================================================================

    class LinkUpStore
    {
        Hashtable linkUpTable;

        public LinkUpStore()
        {
            linkUpTable = new Hashtable();
        }

        public void addLinkUp(LinkUp aLinkUp)
        {
            Point key = aLinkUp.mTranslation;

            if (!linkUpTable.containsKey(key))
            {
                linkUpTable.put(key, new Vector());
            }

            Vector group = (Vector)linkUpTable.get(key);

            if (!group.contains(aLinkUp))
            {
                group.addElement(aLinkUp);
            }
        }

        public int getNoOfGroups()
        {
            int count = 0;

            for (Enumeration e = linkUpTable.keys(); e.hasMoreElements(); count++)
            {
                e.nextElement();
            }

            return count;
        }

        public Vector getGroupAt(int aIndex)
        {
            int count = 0;

            for (Enumeration e = linkUpTable.keys(); e.hasMoreElements(); count++)
            {
                Point key = (Point)e.nextElement();

                if (count == aIndex)
                {
                    return (Vector)linkUpTable.get(key);
                }
            }

            return null;
        }
    }

    //==========================================================================
    //
    //==========================================================================

    class SelectAdapter extends MouseInputAdapter
    {
        private int mStartX;
        private int mStartY;
        private boolean mLocationPressed;
        private MapView mMapView;

        public SelectAdapter(MapView aMapView)
        {
            mMapView = aMapView;
        }

        public void mouseClicked(MouseEvent e)
        {
            MapLocation location = mMapModel.getLocationAt(e.getX(), e.getY());

            // If they didn't click a location, deselect all selected locations.
            if (location == null)
            {
                mMapSelection.clearSelected();

                if (e.getClickCount() == 2)
                {
                    // Get the current inherit room.
                    RoomProperties inherit = mInheritBean.getSelectedInherit();

                    // Make sure an inherit room is selected.
                    if (inherit == null)
                    {
                        JOptionPane.showMessageDialog(mMainFrame,
                            "Before you add a room to the map, you "
                            + "need to select an inheritable from the "
                            + "Inherit Window.\n",
                            "Warning", JOptionPane.WARNING_MESSAGE);
                        return;
                    }

                    MapLocation newLocation = new MapLocation(inherit,
                        e.getX(), e.getY());

                    doEdit(new AddLocationEdit(newLocation));

                    addCompoundEdit();
                }
            }
            else if (e.getClickCount() == 2)
            {
                RoomDialog.showEditDialog(mMainFrame, location);
            }

            repaint();
        }

        public void mousePressed(MouseEvent e)
        {
            // Needed for moving locations relatively when dragging.
            mStartX = e.getX();
            mStartY = e.getY();

            MapLocation location = mMapModel.getLocationAt(e.getX(), e.getY());

            // Needed for determining whether to start dragging.
            mLocationPressed = (location != null);

            if (location != null)
            {
                if (!mMapSelection.isSelected(location))
                {
                    if (!e.isShiftDown())
                    {
                        Vector selectedLocations = mMapSelection.getSelected();
                        int sizeOfSelectedLocations = selectedLocations.size();

                        mMapSelection.clearSelected();

                        mMapSelection.addSelected(location);
                    }
                    else
                    {
                        mMapSelection.addSelected(location);
                    }
                }
                else if (e.isShiftDown())
                {
                    mMapSelection.removeSelected(location);
                }
            }

            repaint();
        }

        public void mouseReleased(MouseEvent e)
        {
            Vector mapLocations = mMapModel.getMapLocations();
            int sizeOfMapLocations = mapLocations.size();

            for (int i = 0; i < sizeOfMapLocations; i++)
            {
                MapLocation location = (MapLocation)mapLocations.elementAt(i);

                location.mOldX = location.getX();
                location.mOldY = location.getY();
            }

            mMapView.setSelectionRectangle(null);
            mLocationPressed = false;

            // Move to currently selected link up group.
            mMapView.doLinkUps();

            // Clear link ups.
            mLinkUpStore = new LinkUpStore();

            addCompoundEdit();

            mMapView.repaint();
        }

        public void mouseDragged(MouseEvent e)
        {
            Vector mapLocations = mMapModel.getMapLocations();

            MapLocation location = mMapModel.getLocationAt(e.getX(), e.getY());

            if (!mLocationPressed)
            {
                Rectangle selection = new Rectangle(
                    Math.min(mStartX, e.getX()),
                    Math.min(mStartY, e.getY()),
                    Math.abs(mStartX - e.getX()),
                    Math.abs(mStartY - e.getY()));

                mMapView.setSelectionRectangle(selection);

                int sizeOfMapLocations = mapLocations.size();

                Vector selected = new Vector();

                for (int i = 0; i < sizeOfMapLocations; i++)
                {
                    MapLocation loc = (MapLocation)mapLocations.elementAt(i);

                    // Maybe this should be contains?
                    if (selection.intersects(loc.getRectangle()))
                    {
                        selected.addElement(loc);
                    }
                }
                if (e.isShiftDown())
                {
                    selected.addAll(mMapSelection.getSelected());
                }
                mMapSelection.setSelected(selected);
            }
            else
            {
                // Move selected locations with mouse pointer.
                int xShift = e.getX() - mStartX;
                int yShift = e.getY() - mStartY;
                mMapView.setSelectionRectangle(null);
                int sizeOfMapLocations = mapLocations.size();

                for (int i = 0; i < sizeOfMapLocations; i++)
                {
                    MapLocation loc = (MapLocation)mapLocations.elementAt(i);

                    if (mMapSelection.isSelected(loc))
                    {
                        loc.mTempX = loc.getX();
                        loc.mTempY = loc.getY();

                        setLocation(loc, loc.mOldX + xShift,
                            loc.mOldY + yShift);
                    }
                }

                refreshLinkUpStore();

                // Discard exits to unselected rooms.
                Vector selectedMapLocations = mMapSelection.getSelected();
                int sizeOfSelectedMapLocation = selectedMapLocations.size();

                for (int k = 0; k < sizeOfSelectedMapLocation; k++)
                {
                    MapLocation current = (MapLocation)selectedMapLocations.elementAt(k);
                    mMapView.clearExitsToUnselected(current);
                }
            }

            if (mMapView.mLinkUpStore.getNoOfGroups() > 1)
            {
                // Send message to status bar indicating choice of link ups.
            }

            mExitBean.readSelection();
            mMapView.repaint();
        }
    }

    //==========================================================================
    //
    //==========================================================================

    class RemoveLocationEdit extends AbstractUndoableEdit
    {
        private MapLocation mMapLocation;
        private Point mCoords;

        public RemoveLocationEdit(MapLocation aMapLocation)
        {
            mMapLocation = aMapLocation;
        }

        public void undo()
        {
            mMapModel.addMapLocation(mMapLocation);
        }

        public void redo()
        {
            mMapSelection.removeSelected(mMapLocation);
            mMapModel.removeMapLocation(mMapLocation);
        }

        public String getUndoPresentationName()
        {
            return "Undo Remove Location";
        }

        public String getRedoPresentationName()
        {
            return "Redo Remove Location";
        }
    }

    //==========================================================================
    //
    //==========================================================================

    class AddLocationEdit extends AbstractUndoableEdit
    {
        private MapLocation mMapLocation;
        private Point mCoords;

        public AddLocationEdit(MapLocation aMapLocation)
        {
            mMapLocation = aMapLocation;
        }

        public void undo()
        {
            mMapSelection.removeSelected(mMapLocation);
            mMapModel.removeMapLocation(mMapLocation);
        }

        public void redo()
        {
            mMapModel.addMapLocation(mMapLocation);
        }

        public String getUndoPresentationName()
        {
            return "Undo Add Location";
        }

        public String getRedoPresentationName()
        {
            return "Redo Add Location";
        }
    }

    //==========================================================================
    //
    //==========================================================================

    class TranslateLocationEdit extends AbstractUndoableEdit
    {
        private MapLocation mMapLocation;
        private Point mOldCoords;
        private Point mNewCoords;

        public TranslateLocationEdit(MapLocation aMapLocation,
            Point aOldCoords, Point aNewCoords)
        {
            super();

            mMapLocation = aMapLocation;
            mOldCoords = aOldCoords;
            mNewCoords = aNewCoords;
        }

        public void redo()
        {
            mMapLocation.setX(mNewCoords.x);
            mMapLocation.setY(mNewCoords.y);

            mMapLocation.mOldX = mNewCoords.x;
            mMapLocation.mOldY = mNewCoords.y;
        }

        public void undo()
        {
            mMapLocation.setX(mOldCoords.x);
            mMapLocation.setY(mOldCoords.y);

            mMapLocation.mOldX = mOldCoords.x;
            mMapLocation.mOldY = mOldCoords.y;
        }

        public String getUndoPresentationName()
        {
            return "Undo Move Location";
        }

        public String getRedoPresentationName()
        {
            return "Redo Move Location";
        }
    }
}
