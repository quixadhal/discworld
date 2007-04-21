package mapmaker;

import java.awt.*;
import java.awt.event.*;
import java.util.*;
import javax.swing.*;

import mapmaker.event.*;

public class BriefRoomBean extends JPanel implements MapSelectionListener, ActiveMapListener
{
    JCheckBox mUseCheckBox = new JCheckBox("Use Autoname?");
    JLabel mShortLabel = new JLabel("Short: ");
    JTextField mShortField = new JTextField();
    JLabel mFileNameLabel = new JLabel("File: ");
    JTextField mFileNameField = new JTextField();
    JButton mEditButton = new JButton(new EditAction());
    JButton mDeleteButton = new JButton(new DeleteAction());

    MapWindow mMapWindow;
    MapLocation mCurrentMapLocation;

    public BriefRoomBean()
    {
        super(new BorderLayout());

        PairedComponentPanel mainPanel = new PairedComponentPanel(3);
        ButtonPanel buttonPanel = new ButtonPanel(3);

        mainPanel.addComponentPair(mShortLabel, mShortField);
        mainPanel.addComponentPair(mFileNameLabel, mFileNameField);
        mainPanel.addHangingComponent(mUseCheckBox);
        buttonPanel.addButton(mEditButton);
        buttonPanel.addButton(mDeleteButton);

        add(mainPanel, BorderLayout.CENTER);
        add(buttonPanel, BorderLayout.SOUTH);

        mUseCheckBox.addActionListener(new ActionListener()
        {
            public void actionPerformed(ActionEvent e)
            {
                boolean selected = mUseCheckBox.isSelected();

                if (selected)
                {
                    mFileNameField.setText(mCurrentMapLocation.getAutoName());
                    mCurrentMapLocation.setUseAutoName(true);
                    mFileNameField.setEnabled(false);
                }
                else
                {
                    mFileNameField.setText(mCurrentMapLocation.getFileName());
                    mCurrentMapLocation.setUseAutoName(false);
                    mFileNameField.setEnabled(true);
                }
            }
        });

        mFileNameField.addActionListener(new ActionListener()
        {
            public void actionPerformed(ActionEvent e)
            {
                try
                {
                    mCurrentMapLocation.setProperty("short", mShortField.getText());
                }
                catch (Exception ex)
                {
                    ex.printStackTrace();
                }
            }
        });

        mShortField.addFocusListener(new FocusAdapter()
        {
            public void focusLost(FocusEvent e)
            {
                try
                {
                    mCurrentMapLocation.setProperty("short", mShortField.getText());
                }
                catch (Exception ex)
                {
                    ex.printStackTrace();
                }
            }
        });

        mFileNameField.addActionListener(new ActionListener()
        {
            public void actionPerformed(ActionEvent e)
            {
                if (Utils.isValidFileName(mFileNameField))
                {
                    mCurrentMapLocation.setFileName(mFileNameField.getText());
                }
                else
                {
                    System.out.println("Opps...");
                }
            }
        });

        mFileNameField.addFocusListener(new FocusAdapter()
        {
            public void focusLost(FocusEvent e)
            {
                if (Utils.isValidFileName(mFileNameField))
                {
                    mCurrentMapLocation.setFileName(mFileNameField.getText());
                }
                else
                {
                    System.out.println("Opps...");
                }
            }
        });

        Font font = new Font("Arial", Font.BOLD, 9);

        mUseCheckBox.setFont(font);
        mShortLabel.setFont(font);
        mShortField.setFont(font);
        mFileNameLabel.setFont(font);
        mFileNameField.setFont(font);
        mEditButton.setFont(font);
        mDeleteButton.setFont(font);

        mEditButton.setBorder(BorderFactory.createEtchedBorder());
        mDeleteButton.setBorder(BorderFactory.createEtchedBorder());
    }

    public void reset()
    {
        mShortField.setEnabled(true);
        mUseCheckBox.setSelected(true);
        mFileNameField.setEnabled(false);
        mFileNameField.setText("");
        mShortField.setText("");
    }

    public void setEnabled(boolean aEnabled)
    {
        mShortField.setEnabled(aEnabled);
        mUseCheckBox.setEnabled(aEnabled);
        mFileNameField.setEnabled(aEnabled);
        mEditButton.setEnabled(aEnabled);
        mDeleteButton.setEnabled(aEnabled);
    }

    public void activeMapChanged(ActiveMapEvent e)
    {
        Vector selection;
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

            selection = mapWindow.getMapView().getMapSelection().getSelected();
            setFor(selection);
        }

        mMapWindow = mapWindow;
    }

    public void setFor(Vector aSelection)
    {
        reset();

        if (aSelection.size() != 1)
        {
            setEnabled(false);
            return;
        }

        setEnabled(true);

        mCurrentMapLocation = (MapLocation)aSelection.elementAt(0);

        try
        {
            mShortField.setEnabled(!mCurrentMapLocation.isPropertyInherited("short"));
            mShortField.setText((String)mCurrentMapLocation.getProperty("short",
                RoomProperties.INHERIT_DO_CHECK));
        }
        catch (Exception ex)
        {
            ex.printStackTrace();
        }

        if (mCurrentMapLocation.isUseAutoName())
        {
            mUseCheckBox.setSelected(true);
            mFileNameField.setEnabled(false);
            mFileNameField.setText(mCurrentMapLocation.getAutoName());
        }
        else
        {
            mUseCheckBox.setSelected(false);
            mFileNameField.setEnabled(true);
            mFileNameField.setText(mCurrentMapLocation.getFileName());
        }
    }

    public void mapSelectionChanged(MapSelectionEvent e)
    {
        setFor(e.getMapSelection());
    }

    class EditAction extends AbstractAction
    {
        private static final String NAME = "Edit...";
        private static final String SHORT_DESCRIPTION = "Edit";
        private static final String LONG_DESCRIPTION = "Edit the selected room";

        public EditAction()
        {
            putValue(Action.NAME, NAME);
            putValue(Action.SHORT_DESCRIPTION, SHORT_DESCRIPTION);
            putValue(Action.LONG_DESCRIPTION, LONG_DESCRIPTION);
        }

        public void actionPerformed(ActionEvent e)
        {

        }
    }

    class DeleteAction extends AbstractAction
    {
        private static final String NAME = "Delete";
        private static final String SHORT_DESCRIPTION = "Delete";
        private static final String LONG_DESCRIPTION = "Delete the selected rooms";

        public DeleteAction()
        {
            putValue(Action.NAME, NAME);
            putValue(Action.SHORT_DESCRIPTION, SHORT_DESCRIPTION);
            putValue(Action.LONG_DESCRIPTION, LONG_DESCRIPTION);
        }

        public void actionPerformed(ActionEvent e)
        {

        }
    }
}