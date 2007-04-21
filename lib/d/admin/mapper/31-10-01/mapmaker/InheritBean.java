package mapmaker;

import java.awt.*;
import java.awt.event.*;
import java.util.*;
import javax.swing.*;
import javax.swing.event.*;

import mapmaker.event.*;

public class InheritBean extends JPanel implements ActiveMapListener
{
    JList mInheritList = new JList();
    EditAction mEditAction = new EditAction();
    JButton mEditButton = new JButton(mEditAction);
    JButton mNewButton = new JButton(new NewAction());
    JButton mDeleteButton = new JButton("Delete");

    MapWindow mMapWindow;
    MainFrame mMainFrame;

    public InheritBean(MainFrame aFrame)
    {
        mMainFrame = aFrame;

        setLayout(new BorderLayout(6, 6));
        setBorder(BorderFactory.createEmptyBorder(6, 6, 5, 5));

        ButtonPanel buttonPanel = new ButtonPanel(3);
        buttonPanel.addButton(mNewButton);
        buttonPanel.addButton(mEditButton);
        buttonPanel.addButton(mDeleteButton);

        add(new JScrollPane(mInheritList), BorderLayout.CENTER);
        add(buttonPanel, BorderLayout.SOUTH);

        mNewButton.setBorder(BorderFactory.createEtchedBorder());
        mEditButton.setBorder(BorderFactory.createEtchedBorder());
        mDeleteButton.setBorder(BorderFactory.createEtchedBorder());
        mNewButton.setFont(new Font("Arial", Font.BOLD, 9));
        mEditButton.setFont(new Font("Arial", Font.BOLD, 9));
        mDeleteButton.setFont(new Font("Arial", Font.BOLD, 9));

        mInheritList.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
        mInheritList.setCellRenderer(new InheritListEditor());

        ListSelectionModel model = mInheritList.getSelectionModel();
        model.addListSelectionListener(new ListSelectionListener()
        {
            public void valueChanged(ListSelectionEvent e)
            {
                boolean enabled = (mInheritList.getSelectedIndex() != -1);
                mEditButton.setEnabled(enabled);
                mDeleteButton.setEnabled(enabled);

                if (enabled)
                {
                    MapView mapView = mMapWindow.getMapView();
                    Vector selected = mapView.getMapSelection().getSelected();

                    for (Enumeration enum = selected.elements(); enum.hasMoreElements();)
                    {
                        MapLocation loc = (MapLocation)enum.nextElement();

                        // TODO: Fix this...
                        //loc.setInherit((Inherit)mInheritList.getSelectedValue());
                    }

                    mapView.repaint();
                }
            }
        });

        mInheritList.addMouseListener(new MouseAdapter()
        {
            public void mouseClicked(MouseEvent e)
            {
                if (e.getClickCount() == 2)
                {
                    ActionEvent dummyEvent = new ActionEvent(this, 0, "");
                    mEditAction.actionPerformed(dummyEvent);
                }
            }
        });
    }

    public RoomProperties getSelectedInherit()
    {
        return (RoomProperties)mInheritList.getSelectedValue();
    }

    public void setEnabled(boolean aEnabled)
    {
        mInheritList.setEnabled(aEnabled);
        mNewButton.setEnabled(aEnabled);
        mEditButton.setEnabled(aEnabled);
        mDeleteButton.setEnabled(aEnabled);
    }

    public void reset()
    {
        mInheritList.setListData(new Vector());
    }

    public void activeMapChanged(ActiveMapEvent e)
    {
        MapWindow mapWindow = e.getActiveMapWindow();

        if (mapWindow == null)
        {
            reset();
            setEnabled(false);
        }
        else
        {
            MapView mapView = mapWindow.getMapView();
            mInheritList.setListData(mapView.getMapModel().getInheritables());

            // TODO: Retrieve old selection.

            setEnabled(true);
            mEditButton.setEnabled(false);
            mDeleteButton.setEnabled(false);
        }

        mMapWindow = mapWindow;
    }

    class NewAction extends AbstractAction
    {
        private static final String NAME = "New...";
        private static final String SHORT_DESCRIPTION = "New";
        private static final String LONG_DESCRIPTION = "Create a new inheritable";

        public NewAction()
        {
            putValue(Action.NAME, NAME);
            putValue(Action.SHORT_DESCRIPTION, SHORT_DESCRIPTION);
            putValue(Action.LONG_DESCRIPTION, LONG_DESCRIPTION);
        }

        public void actionPerformed(ActionEvent e)
        {
            RoomProperties inherit = RoomDialog.showNewDialog(mMainFrame);

            if (inherit != null)
            {
                MapView mapView = mMapWindow.getMapView();
                MapModel mapModel = mapView.getMapModel();
                mapModel.addInheritable(inherit);
                mInheritList.setListData(mapModel.getInheritables());
                mInheritList.setSelectedValue(inherit, true);
                mapView.repaint();
            }
        }
    }

    class EditAction extends AbstractAction
    {
        private static final String NAME = "Edit...";
        private static final String SHORT_DESCRIPTION = "Edit";
        private static final String LONG_DESCRIPTION = "Edit the selected inheritable";

        public EditAction()
        {
            putValue(Action.NAME, NAME);
            putValue(Action.SHORT_DESCRIPTION, SHORT_DESCRIPTION);
            putValue(Action.LONG_DESCRIPTION, LONG_DESCRIPTION);
        }

        public void actionPerformed(ActionEvent e)
        {
            /*
            Inherit inherit = (Inherit)mInheritList.getSelectedValue();
            Inherit newInherit = InheritDialog.showEditDialog(mMainFrame, inherit);

            if (newInherit != null)
            {
                MapView mapView = mMapWindow.getMapView();
                MapModel mapModel = mapView.getMapModel();
                mapModel.replaceInheritable(inherit, newInherit);
                mInheritList.setListData(mapModel.getInheritables());
                mInheritList.setSelectedValue(newInherit, true);
                mapView.repaint();
            }
            */
        }
    }
}

class InheritListEditor extends DefaultListCellRenderer
{
    public Component getListCellRendererComponent(JList aList, Object aValue,
        int aIndex, boolean aIsSelected, boolean aCellHasFocus)
    {
        JLabel defaultRenderer = (JLabel)super.getListCellRendererComponent(aList,
            aValue, aIndex, aIsSelected, aCellHasFocus);

        JPanel panel = new JPanel(new BorderLayout(3, 0));
        JLabel label = new JLabel(defaultRenderer.getText());

        label.setForeground(Color.black);

        RoomLabel roomLabel = new RoomLabel((RoomProperties)aValue);
        roomLabel.setBackground(defaultRenderer.getBackground());

        panel.setBackground(defaultRenderer.getBackground());
        panel.add(roomLabel, BorderLayout.WEST);
        panel.add(label, BorderLayout.CENTER);

        return panel;
    }
}

class RoomLabel extends JLabel
{
    RoomProperties mInherit;

    public RoomLabel(RoomProperties aInherit)
    {
        mInherit = aInherit;
        setPreferredSize(new Dimension(24, 24));
    }

    public void paintComponent(Graphics g)
    {
        super.paintComponent(g);

        Dimension d = getSize();

        int height = (int)d.getHeight() - 8;
        int width = (int)d.getWidth() - 8;

        g.setColor(mInherit.mBackgroundColor);
        g.fillRect(4, 4, width, height);

        g.setColor(mInherit.mForegroundColor);
        g.drawRect(4, 4, width, height);
    }
}