package mapmaker;

import java.awt.*;
import java.awt.event.*;
import java.util.*;
import javax.swing.*;

import mapmaker.event.*;

public class ZoneBean extends JPanel implements MapSelectionListener, ActiveMapListener
{
    JLabel mZoneLabel = new JLabel("Zone: ");
    JComboBox mZoneComboBox = new JComboBox();
    JButton mAddZoneButton = new JButton(new AddZoneAction());
    JButton mRemoveZoneButton = new JButton(new RemoveZoneAction());
    JButton mNewZoneButton = new JButton(new NewZoneAction());
    JButton mEditZoneButton = new JButton(new EditZoneAction());
    JButton mDeleteZoneButton = new JButton("Delete");

    JLabel mLinkerLabel = new JLabel("Linker: ");
    JComboBox mLinkerComboBox = new JComboBox();
    JButton mAddLinkerButton = new JButton(new AddLinkerAction());
    JButton mRemoveLinkerButton = new JButton(new RemoveLinkerAction());
    JButton mNewLinkerButton = new JButton(new NewLinkerAction());
    JButton mEditLinkerButton = new JButton(new EditLinkerAction());
    JButton mDeleteLinkerButton = new JButton("Delete");

    JFrame mFrame;
    Vector mSelected;
    MapWindow mMapWindow;

    public ZoneBean(JFrame aFrame)
    {
        super(new BorderLayout(0, 6));

        mFrame = aFrame;

        ButtonPanel zoneAddRemovePanel = new ButtonPanel(3);
        zoneAddRemovePanel.addButton(mAddZoneButton);
        zoneAddRemovePanel.addButton(mRemoveZoneButton);

        ButtonPanel zoneButtonPanel = new ButtonPanel(3);
        zoneButtonPanel.addButton(mNewZoneButton);
        zoneButtonPanel.addButton(mEditZoneButton);
        zoneButtonPanel.addButton(mDeleteZoneButton);

        PairedComponentPanel zonePanel = new PairedComponentPanel(3);
        zonePanel.addComponentPair(mZoneLabel, mZoneComboBox);
        zonePanel.addAreaComponent(zoneAddRemovePanel, 1);
        zonePanel.addAreaComponent(zoneButtonPanel, 1);

        ButtonPanel linkerAddRemovePanel = new ButtonPanel(3);
        linkerAddRemovePanel.addButton(mAddLinkerButton);
        linkerAddRemovePanel.addButton(mRemoveLinkerButton);

        ButtonPanel linkerButtonPanel = new ButtonPanel(3);
        linkerButtonPanel.addButton(mNewLinkerButton);
        linkerButtonPanel.addButton(mEditLinkerButton);
        linkerButtonPanel.addButton(mDeleteLinkerButton);

        PairedComponentPanel linkerPanel = new PairedComponentPanel(3);
        linkerPanel.addComponentPair(mLinkerLabel, mLinkerComboBox);
        linkerPanel.addAreaComponent(linkerAddRemovePanel, 1);
        linkerPanel.addAreaComponent(linkerButtonPanel, 1);

        add(zonePanel, BorderLayout.NORTH);
        add(linkerPanel, BorderLayout.SOUTH);

        mZoneComboBox.addActionListener(new ActionListener()
        {
            public void actionPerformed(ActionEvent e)
            {
                if (!(mZoneComboBox.getSelectedItem() instanceof Zone))
                {
                    mEditZoneButton.setEnabled(false);
                    mDeleteZoneButton.setEnabled(false);

                    mAddZoneButton.setEnabled(false);
                    mRemoveZoneButton.setEnabled(false);

                    if (mMapWindow != null)
                    {
                        mMapWindow.getMapView().setDisplayZone(null);
                        mMapWindow.getMapView().repaint();
                    }
                    return;
                }

                mEditZoneButton.setEnabled(true);
                mDeleteZoneButton.setEnabled(true);

                readSelection(mSelected);

                mMapWindow.getMapView().setDisplayZone((Zone)mZoneComboBox.getSelectedItem());
                mMapWindow.getMapView().repaint();
            }
        });

        mLinkerComboBox.addActionListener(new ActionListener()
        {
            public void actionPerformed(ActionEvent e)
            {
                if (!(mLinkerComboBox.getSelectedItem() instanceof Linker))
                {
                    mEditLinkerButton.setEnabled(false);
                    mDeleteLinkerButton.setEnabled(false);

                    mAddLinkerButton.setEnabled(false);
                    mRemoveLinkerButton.setEnabled(false);

                    if (mMapWindow != null)
                    {
                        mMapWindow.getMapView().setDisplayLinker(null);
                        mMapWindow.getMapView().repaint();
                    }
                    return;
                }

                mEditLinkerButton.setEnabled(true);
                mDeleteLinkerButton.setEnabled(true);

                readSelection(mSelected);

                mMapWindow.getMapView().setDisplayLinker((Linker)mLinkerComboBox.getSelectedItem());
                mMapWindow.getMapView().repaint();
            }
        });

        Font font = new Font("Arial", Font.BOLD, 9);
        mZoneLabel.setFont(font);
        mZoneComboBox.setFont(font);
        mAddZoneButton.setFont(font);
        mRemoveZoneButton.setFont(font);
        mNewZoneButton.setFont(font);
        mEditZoneButton.setFont(font);
        mDeleteZoneButton.setFont(font);
        mLinkerLabel.setFont(font);
        mLinkerComboBox.setFont(font);
        mAddLinkerButton.setFont(font);
        mRemoveLinkerButton.setFont(font);
        mNewLinkerButton.setFont(font);
        mEditLinkerButton.setFont(font);
        mDeleteLinkerButton.setFont(font);

        mNewZoneButton.setBorder(BorderFactory.createEtchedBorder());
        mEditZoneButton.setBorder(BorderFactory.createEtchedBorder());
        mDeleteZoneButton.setBorder(BorderFactory.createEtchedBorder());
        mNewLinkerButton.setBorder(BorderFactory.createEtchedBorder());
        mEditLinkerButton.setBorder(BorderFactory.createEtchedBorder());
        mDeleteLinkerButton.setBorder(BorderFactory.createEtchedBorder());
        mAddLinkerButton.setBorder(BorderFactory.createEtchedBorder());
        mRemoveLinkerButton.setBorder(BorderFactory.createEtchedBorder());
        mAddZoneButton.setBorder(BorderFactory.createEtchedBorder());
        mRemoveZoneButton.setBorder(BorderFactory.createEtchedBorder());
    }

    public void reset()
    {
        Vector zones = new Vector();
        Vector linkers = new Vector();
        zones.addElement("None");
        linkers.addElement("None");
        mZoneComboBox.setModel(new DefaultComboBoxModel(zones));
        mLinkerComboBox.setModel(new DefaultComboBoxModel(linkers));

        mEditLinkerButton.setEnabled(false);
        mEditZoneButton.setEnabled(false);
    }

    public void setEnabled(boolean aEnabled)
    {
        mZoneLabel.setEnabled(aEnabled);
        mZoneComboBox.setEnabled(aEnabled);
        mAddZoneButton.setEnabled(aEnabled);
        mRemoveZoneButton.setEnabled(aEnabled);
        mNewZoneButton.setEnabled(aEnabled);
        mEditZoneButton.setEnabled(aEnabled);
        mDeleteZoneButton.setEnabled(aEnabled);
        mLinkerLabel.setEnabled(aEnabled);
        mLinkerComboBox.setEnabled(aEnabled);
        mAddLinkerButton.setEnabled(aEnabled);
        mRemoveLinkerButton.setEnabled(aEnabled);
        mNewLinkerButton.setEnabled(aEnabled);
        mEditLinkerButton.setEnabled(aEnabled);
        mDeleteLinkerButton.setEnabled(aEnabled);
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

            System.out.println("Bing...");

            reset();
            setEnabled(false);
        }
        else
        {
            mapWindow.getMapView().getMapSelection().addMapSelectionListener(this);
            setEnabled(true);
            reset();

            MapModel mapModel = mapWindow.getMapView().getMapModel();

            Vector zones = mapModel.getZones();
            zones.removeElement("None");
            zones.addElement("None");
            mZoneComboBox.setModel(new DefaultComboBoxModel(zones));

            Vector linkers = mapModel.getLinkers();
            linkers.removeElement("None");
            linkers.addElement("None");
            mLinkerComboBox.setModel(new DefaultComboBoxModel(linkers));
        }

        mMapWindow = mapWindow;
    }

    public void readSelection(Vector aSelection)
    {
        mAddZoneButton.setEnabled(false);
        mRemoveZoneButton.setEnabled(false);
        mAddLinkerButton.setEnabled(false);
        mRemoveLinkerButton.setEnabled(false);

        if (aSelection != null)
        {
            Object zone = mZoneComboBox.getSelectedItem();

            if (zone instanceof Zone)
            {
                boolean notInZone = false;

                for (int i = 0; i < aSelection.size(); i++)
                {
                    MapLocation location = (MapLocation)aSelection.elementAt(i);

                    if (!location.isInZone((Zone)mZoneComboBox.getSelectedItem()))
                    {
                        mAddZoneButton.setEnabled(true);
                    }
                    else
                    {
                        mRemoveZoneButton.setEnabled(true);
                    }
                }
            }

            Object linker = mLinkerComboBox.getSelectedItem();

            if (linker instanceof Linker)
            {
                boolean notInLinker = false;

                for (int i = 0; i < aSelection.size(); i++)
                {
                    MapLocation location = (MapLocation)aSelection.elementAt(i);

                    if (!location.isInLinker((Linker)mLinkerComboBox.getSelectedItem()))
                    {
                        mAddLinkerButton.setEnabled(true);
                    }
                    else
                    {
                        mRemoveLinkerButton.setEnabled(true);
                    }
                }
            }
        }
    }

    public void mapSelectionChanged(MapSelectionEvent e)
    {
        mSelected = e.getMapSelection();

        readSelection(mSelected);
    }

    class AddZoneAction extends AbstractAction
    {
        private static final String NAME = "Add";
        private static final String SHORT_DESCRIPTION = "Add";
        private static final String LONG_DESCRIPTION = "Add current selection to zone";

        public AddZoneAction()
        {
            putValue(Action.NAME, NAME);
            putValue(Action.SHORT_DESCRIPTION, SHORT_DESCRIPTION);
            putValue(Action.LONG_DESCRIPTION, LONG_DESCRIPTION);
        }

        public void actionPerformed(ActionEvent e)
        {
            for (int i = 0; i < mSelected.size(); i++)
            {
                MapLocation location = (MapLocation)mSelected.elementAt(i);
                Zone zone = (Zone)mZoneComboBox.getSelectedItem();
                location.addZone(zone);
                readSelection(mSelected);
                mMapWindow.getMapView().repaint();
            }
        }
    }

    class RemoveZoneAction extends AbstractAction
    {
        private static final String NAME = "Remove";
        private static final String SHORT_DESCRIPTION = "Remove";
        private static final String LONG_DESCRIPTION = "Remove current selection from zone";

        public RemoveZoneAction()
        {
            putValue(Action.NAME, NAME);
            putValue(Action.SHORT_DESCRIPTION, SHORT_DESCRIPTION);
            putValue(Action.LONG_DESCRIPTION, LONG_DESCRIPTION);
        }

        public void actionPerformed(ActionEvent e)
        {
            for (int i = 0; i < mSelected.size(); i++)
            {
                MapLocation location = (MapLocation)mSelected.elementAt(i);
                Zone zone = (Zone)mZoneComboBox.getSelectedItem();
                location.removeZone(zone);
                readSelection(mSelected);
                mMapWindow.getMapView().repaint();
            }
        }
    }

    class NewZoneAction extends AbstractAction
    {
        private static final String NAME = "New...";
        private static final String SHORT_DESCRIPTION = "New";
        private static final String LONG_DESCRIPTION = "Create a new zone";

        public NewZoneAction()
        {
            putValue(Action.NAME, NAME);
            putValue(Action.SHORT_DESCRIPTION, SHORT_DESCRIPTION);
            putValue(Action.LONG_DESCRIPTION, LONG_DESCRIPTION);
        }

        public void actionPerformed(ActionEvent e)
        {
            Zone newZone = ZoneDialog.showNewDialog(mFrame);

            if (newZone != null)
            {
                MapModel mapModel = mMapWindow.getMapView().getMapModel();
                mapModel.addZone(newZone);

                Vector zones = mapModel.getZones();
                zones.removeElement("None");
                zones.addElement("None");

                mZoneComboBox.setModel(new DefaultComboBoxModel(zones));
                mZoneComboBox.setSelectedItem(newZone);
                readSelection(mSelected);
            }
        }
    }

    class EditZoneAction extends AbstractAction
    {
        private static final String NAME = "Edit...";
        private static final String SHORT_DESCRIPTION = "Edit";
        private static final String LONG_DESCRIPTION = "Edit the selected zone";

        public EditZoneAction()
        {
            putValue(Action.NAME, NAME);
            putValue(Action.SHORT_DESCRIPTION, SHORT_DESCRIPTION);
            putValue(Action.LONG_DESCRIPTION, LONG_DESCRIPTION);
        }

        public void actionPerformed(ActionEvent e)
        {
            Zone zone = (Zone)mZoneComboBox.getSelectedItem();
            Zone newZone = ZoneDialog.showEditDialog(mFrame, zone);

            if (newZone != null)
            {
                MapModel mapModel = mMapWindow.getMapView().getMapModel();
                mapModel.replaceZone(zone, newZone);
                mZoneComboBox.setModel(new DefaultComboBoxModel(mapModel.getZones()));
                mZoneComboBox.setSelectedItem(newZone);
            }
        }
    }

    class AddLinkerAction extends AbstractAction
    {
        private static final String NAME = "Add";
        private static final String SHORT_DESCRIPTION = "Add";
        private static final String LONG_DESCRIPTION = "Add current selection to linker";

        public AddLinkerAction()
        {
            putValue(Action.NAME, NAME);
            putValue(Action.SHORT_DESCRIPTION, SHORT_DESCRIPTION);
            putValue(Action.LONG_DESCRIPTION, LONG_DESCRIPTION);
        }

        public void actionPerformed(ActionEvent e)
        {
            for (int i = 0; i < mSelected.size(); i++)
            {
                MapLocation location = (MapLocation)mSelected.elementAt(i);
                Linker linker = (Linker)mLinkerComboBox.getSelectedItem();
                location.addLinker(linker);
            }
            readSelection(mSelected);
            mMapWindow.getMapView().repaint();
        }
    }

    class RemoveLinkerAction extends AbstractAction
    {
        private static final String NAME = "Remove";
        private static final String SHORT_DESCRIPTION = "Remove";
        private static final String LONG_DESCRIPTION = "Remove current selection from linker";

        public RemoveLinkerAction()
        {
            putValue(Action.NAME, NAME);
            putValue(Action.SHORT_DESCRIPTION, SHORT_DESCRIPTION);
            putValue(Action.LONG_DESCRIPTION, LONG_DESCRIPTION);
        }

        public void actionPerformed(ActionEvent e)
        {
            for (int i = 0; i < mSelected.size(); i++)
            {
                MapLocation location = (MapLocation)mSelected.elementAt(i);
                Linker linker = (Linker)mLinkerComboBox.getSelectedItem();
                location.removeLinker(linker);
            }
            readSelection(mSelected);
            mMapWindow.getMapView().repaint();
        }
    }

    class NewLinkerAction extends AbstractAction
    {
        private static final String NAME = "New...";
        private static final String SHORT_DESCRIPTION = "New";
        private static final String LONG_DESCRIPTION = "Create a new linker";

        public NewLinkerAction()
        {
            putValue(Action.NAME, NAME);
            putValue(Action.SHORT_DESCRIPTION, SHORT_DESCRIPTION);
            putValue(Action.LONG_DESCRIPTION, LONG_DESCRIPTION);
        }

        public void actionPerformed(ActionEvent e)
        {
            Linker newLinker = LinkerDialog.showNewDialog(mFrame);

            if (newLinker != null)
            {
                MapModel mapModel = mMapWindow.getMapView().getMapModel();
                mapModel.addLinker(newLinker);

                Vector linkers = mapModel.getLinkers();
                linkers.removeElement("None");
                linkers.addElement("None");

                mLinkerComboBox.setModel(new DefaultComboBoxModel(linkers));
                mLinkerComboBox.setSelectedItem(newLinker);
                readSelection(mSelected);
            }
        }
    }

    class EditLinkerAction extends AbstractAction
    {
        private static final String NAME = "Edit...";
        private static final String SHORT_DESCRIPTION = "Edit";
        private static final String LONG_DESCRIPTION = "Edit the selected linker";

        public EditLinkerAction()
        {
            putValue(Action.NAME, NAME);
            putValue(Action.SHORT_DESCRIPTION, SHORT_DESCRIPTION);
            putValue(Action.LONG_DESCRIPTION, LONG_DESCRIPTION);
        }

        public void actionPerformed(ActionEvent e)
        {
            Linker linker = (Linker)mLinkerComboBox.getSelectedItem();
            Linker newLinker = LinkerDialog.showEditDialog(mFrame, linker);

            if (newLinker != null)
            {
                MapModel mapModel = mMapWindow.getMapView().getMapModel();
                mapModel.replaceLinker(linker, newLinker);
                mLinkerComboBox.setModel(new DefaultComboBoxModel(mapModel.getLinkers()));
                mLinkerComboBox.setSelectedItem(newLinker);
            }
        }
    }
}