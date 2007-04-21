package mapmaker;

import java.awt.*;
import java.net.*;
import java.awt.event.*;
import javax.swing.*;
import java.util.*;
import mapmaker.event.*;

public class ExitBean extends JPanel implements MapSelectionListener, ActiveMapListener
{
    private ExitCheckBox[] checkBoxes = new ExitCheckBox[8];
    private Vector mSelected = new Vector();
    private MapWindow mMapWindow;

    public ExitBean()
    {
        super(new GridLayout(0, 3));

        for (int i = 0; i < 8; i++)
        {
            checkBoxes[i] = new ExitCheckBox(i);

            checkBoxes[i].setAction(new AbstractAction()
            {
                public void actionPerformed(ActionEvent e)
                {
                    MapView mapView = getActiveMapView();

                    int sizeOfSelected = mSelected.size();
                    for (int j = 0; j < sizeOfSelected; j++)
                    {
                        for (int i = 0; i < 8; i++)
                        {
                            if (e.getSource() == checkBoxes[i])
                            {
                                MapLocation loc = (MapLocation)mSelected.elementAt(j);

                                if (checkBoxes[i].isSelected())
                                {
                                    // Check for link ups.
                                    Vector linkUps = mapView.getLinkUpsForLocation(loc);
                                    int sizeOfLinkUps = linkUps.size();
                                    boolean found = false;
                                    for (int k = 0; k < sizeOfLinkUps; k++)
                                    {
                                        LinkUp linkUp = (LinkUp)linkUps.elementAt(k);

                                        if (linkUp.mDirection == i)
                                        {
                                            mapView.connectExits(linkUp);
                                            found = true;
                                            break;
                                        }
                                    }

                                    if (!found)
                                    {
                                        // TODO: read "path" from some place
                                        Exit exit = new Exit(i, loc, "", "path");
                                        AddExitEdit edit = new AddExitEdit(loc, exit);
                                        mapView.doEdit(edit);
                                    }
                                }
                                else
                                {
                                    Exit fromExit = loc.getExit(i);
                                    RemoveExitEdit fromEdit = new RemoveExitEdit(loc, fromExit);
                                    mapView.doEdit(fromEdit);

                                    MapLocation to = fromExit.getTo();

                                    if (to != null)
                                    {
                                        Exit toExit = fromExit.getOpposite();
                                        RemoveExitEdit toEdit = new RemoveExitEdit(to, toExit);
                                        mapView.doEdit(toEdit);
                                    }
                                }

                                readSelection();
                                mapView.repaint();
                            }
                        }
                    }
                    mapView.addCompoundEdit();
                }
            });
        }

        add(checkBoxes[Direction.NW]);
        add(checkBoxes[Direction.N]);
        add(checkBoxes[Direction.NE]);
        add(checkBoxes[Direction.W]);
        add(new JPanel());
        add(checkBoxes[Direction.E]);
        add(checkBoxes[Direction.SW]);
        add(checkBoxes[Direction.S]);
        add(checkBoxes[Direction.SE]);
        //add(new JPanel());
        //add(checkBoxes[Direction.UP]);
        //add(checkBoxes[Direction.DOWN]);
    }

    public MapView getActiveMapView()
    {
        if (mMapWindow == null)
        {
            return null;
        }

        return mMapWindow.getMapView();
    }

    public void activeMapChanged(ActiveMapEvent e)
    {
        MapWindow mapWindow = e.getActiveMapWindow();

        if (mapWindow == null)
        {
            if (mMapWindow != null)
            {
                mMapWindow.getMapView().getMapSelection().removeMapSelectionListener(this);
            }

            reset();
            setEnabled(false);
        }
        else
        {
            mapWindow.getMapView().getMapSelection().addMapSelectionListener(this);
        }

        mMapWindow = mapWindow;
    }

    public void reset()
    {
        for (int i = 0; i < checkBoxes.length; i++)
        {
            checkBoxes[i].setSelected(false);
        }
    }

    public void setEnabled(boolean aEnabled)
    {
        for (int i = 0; i < checkBoxes.length; i++)
        {
            checkBoxes[i].setSelected(false);
            checkBoxes[i].setEnabled(aEnabled);
        }
    }

    public void readSelection()
    {
        // Populate the check boxes.
        for (int i = 0; i < 8; i++)
        {
            checkBoxes[i].setSelected(false);
        }

        if (mSelected.size() != 1)
        {
            setEnabled(false);
            return;
        }
        else
        {
            setEnabled(true);
        }

        MapLocation loc = (MapLocation)mSelected.elementAt(0);
        Vector exits = loc.getExits();

        for (Enumeration e = exits.elements(); e.hasMoreElements();)
        {
            Exit exit = (Exit)e.nextElement();

            if (exit == null)
            {
                continue;
            }

            if (exit.isDirection())
            {
                int dir = exit.getDirection();

                checkBoxes[dir].setSelected(true);
            }
            else
            {
                // TODO: Do something for non-directional exits?
            }
        }
    }

    public void mapSelectionChanged(MapSelectionEvent e)
    {
        mSelected = e.getMapSelection();

        readSelection();
    }

    class ExitCheckBox extends JCheckBox
    {
        ImageIcon mIcon;

        public ExitCheckBox(int aDir)
        {
            mIcon = Utils.getIcon("Direction" + aDir + ".gif");
            setPreferredSize(new Dimension(mIcon.getIconWidth(),
                mIcon.getIconHeight()));
        }

        public void setSelected(boolean aEnabled)
        {
            super.setSelected(aEnabled);

            if (aEnabled)
            {
                setIcon(mIcon);
            }
            else
            {
                setIcon(null);
            }
        }
    }
}