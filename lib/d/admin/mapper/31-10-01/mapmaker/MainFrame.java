package mapmaker;

import javax.swing.*;
import javax.swing.border.*;
import java.awt.event.*;
import javax.swing.event.*;
import java.awt.*;
import java.net.*;
import java.io.*;
import java.text.*;
import java.util.*;

import mapmaker.event.*;

public class MainFrame extends JFrame
{
    public final static int TOOL_PALETTE_HEIGHT = 180;
    public final static int TOOL_PALETTE_WIDTH = 160;

    static String PROJECT_COMMENT = "// Project: {0}";
    static String AUTHOR_COMMENT = "// Author: {0}";
    static String DATE_COMMENT = "// Date: {0}\n";
    static String SETUP_HEADER = "void setup()";
    static String SET_LIGHT = "set_light({0});\n";
    static String SET_ROOM_SIZE = "set_room_size({0});\n";
    static String SET_DETERMINATE = "add_property(\"determinate\", \"{0} \");\n";
    static String SET_SHORT = "set_short(\"{0}\");\n";
    static String SET_LONG = "set_long(\"{0}\");\n";
    static String SET_DAY_LONG = "set_day_long(\"{0}\");\n";
    static String SET_NIGHT_LONG = "set_night_long(\"{0}\");\n";
    static String ADD_ITEM = "add_item({0}, \"{1}\");";
    static String ADD_DAY_ITEM = "add_day_item({0}, \"{1}\");";
    static String ADD_NIGHT_ITEM = "add_night_item({0}, \"{1}\");";
    static String ADD_EXIT = "add_exit(\"{0}\", PATH + \"{1}\", \"{2}\");";

    EventListenerList mListenerList = new EventListenerList();

    JDesktopPane mDesktop;
    JToolBar mToolBar;
    JLabel mStatusBar;
    MainFrame mFrame;

    MapWindow mActiveMapWindow;

    InheritBean mInheritBean;
    ExitBean mExitBean;
    BriefRoomBean mBriefRoomBean;
    ZoneBean mZoneBean;

    JFileChooser mFileChooser;

    NewAction mNewAction;
    OpenAction mOpenAction;
    CloseAction mCloseAction;
    SaveAction mSaveAction;
    SaveAsAction mSaveAsAction;
    SaveAllAction mSaveAllAction;
    UndoAction mUndoAction;
    RedoAction mRedoAction;
    CutAction mCutAction;
    CopyAction mCopyAction;
    PasteAction mPasteAction;
    ExitAction mExitAction;
    ResizeAction mResizeAction;
    ExportAction mExportAction;
    ExportOptionsAction mExportOptionsAction;

    public MainFrame()
    {
        super("Discworld Mapper");

        mFrame = this;

        mFileChooser = new JFileChooser();

        // Set window dimensions.
        Dimension screenSize = Toolkit.getDefaultToolkit().getScreenSize();
        setBounds(screenSize.width / 6, screenSize.height / 6,
            (screenSize.width / 6) * 4, (screenSize.height / 6) * 4);

        // Add save dialog HERE.
        addWindowListener(new WindowAdapter()
        {
            public void windowClosing(WindowEvent e)
            {
                System.exit(0);
            }
        });

        JPanel mainPanel = new JPanel();
        getContentPane().setLayout(new BorderLayout());
        getContentPane().add(mainPanel, BorderLayout.CENTER);

        createActions();

        getContentPane().add(createToolBar(), BorderLayout.NORTH);

        mainPanel.setLayout(new BorderLayout());

        mStatusBar = new JLabel(" ");
        mainPanel.add(mStatusBar, BorderLayout.SOUTH);

        mDesktop = new JDesktopPane();
        mainPanel.add(new JScrollPane(mDesktop), BorderLayout.CENTER);

        setJMenuBar(createMenuBar());

        addKeyListener(new KeyAdapter()
        {
            public void keyPressed(KeyEvent e)
            {
                MapView mapView = getActiveMapView();

                if (mapView != null)
                {
                    switch (e.getKeyChar())
                    {
                        case 'c' :
                            mapView.cycleLinkUpGroup();
                            break;
                        case KeyEvent.VK_DELETE :
                            mapView.deleteSelectedLocations();
                            break;
                    }
                }
            }
        });

        mCloseAction.setEnabled(false);
        mSaveAction.setEnabled(false);
        mSaveAsAction.setEnabled(false);
        mSaveAllAction.setEnabled(false);
        mCutAction.setEnabled(false);
        mCopyAction.setEnabled(false);
        mPasteAction.setEnabled(false);
        mResizeAction.setEnabled(false);
        mUndoAction.setEnabled(false);
        mRedoAction.setEnabled(false);
        mExportAction.setEnabled(false);
        mExportOptionsAction.setEnabled(false);

        createToolPalette();
    }

    public void setStatus(String aStatus)
    {
        mStatusBar.setText(aStatus);
    }

    protected void createActions()
    {
        mNewAction = new NewAction();
        mOpenAction = new OpenAction();
        mCloseAction = new CloseAction();
        mSaveAction = new SaveAction();
        mSaveAsAction = new SaveAsAction();
        mSaveAllAction = new SaveAllAction();
        mUndoAction = new UndoAction();
        mRedoAction = new RedoAction();
        mCutAction = new CutAction();
        mCopyAction = new CopyAction();
        mPasteAction = new PasteAction();
        mExitAction = new ExitAction();
        mResizeAction = new ResizeAction();
        mExportAction = new ExportAction();
        mExportOptionsAction = new ExportOptionsAction();
    }

    protected JToolBar createToolBar()
    {
        JToolBar toolBar = new JToolBar();

        toolBar.add(new ToolBarButton(mNewAction));
        toolBar.add(new ToolBarButton(mOpenAction));
        toolBar.add(new ToolBarButton(mSaveAction));

        toolBar.add(new ToolBarButton(mExportAction));

        toolBar.addSeparator();
        toolBar.add(new ToolBarButton(mUndoAction));
        toolBar.add(new ToolBarButton(mRedoAction));
        //toolBar.addSeparator();
        //toolBar.add(new ToolBarButton(mCutAction));
        //toolBar.add(new ToolBarButton(mCopyAction));
        //toolBar.add(new ToolBarButton(mPasteAction));

        return toolBar;
    }

    protected JMenuBar createMenuBar()
    {
        JMenuBar menuBar = new JMenuBar();

        JMenu fileMenu = new JMenu("File");
        fileMenu.setMnemonic(KeyEvent.VK_F);
        fileMenu.add(new JMenuItem(mNewAction));
        fileMenu.add(new JMenuItem(mOpenAction));
        fileMenu.add(new JMenuItem(mCloseAction));
        fileMenu.addSeparator();
        fileMenu.add(new JMenuItem(mSaveAction));
        fileMenu.add(new JMenuItem(mSaveAsAction));
        //fileMenu.add(new JMenuItem(mSaveAllAction));
        fileMenu.addSeparator();
        fileMenu.add(new JMenuItem(mExitAction));

        JMenu editMenu = new JMenu("Edit");
        editMenu.setMnemonic(KeyEvent.VK_E);
        editMenu.add(new JMenuItem(mUndoAction));
        editMenu.add(new JMenuItem(mRedoAction));
        //editMenu.addSeparator();
        //editMenu.add(new JMenuItem(mCutAction));
        //editMenu.add(new JMenuItem(mCopyAction));
        //editMenu.add(new JMenuItem(mPasteAction));
        editMenu.addSeparator();
        editMenu.add(new JMenuItem(mResizeAction));

        JMenu mapMenu = new JMenu("Map");
        mapMenu.setMnemonic(KeyEvent.VK_M);
        mapMenu.add(new JMenuItem(mExportAction));
        mapMenu.add(new JMenuItem(mExportOptionsAction));

        menuBar.add(fileMenu);
        menuBar.add(editMenu);
        menuBar.add(mapMenu);

        return menuBar;
    }

    public void createToolPalette()
    {
        JInternalFrame toolPalette = new ToolPalette(this);
        toolPalette.setVisible(true);
        mDesktop.add(toolPalette);
    }

    public MapWindow createMapWindow(MapModel aMapModel, String aTitle)
    {
        MapWindow mapWindow = new MapWindow(this, aTitle);
        mapWindow.setMapView(new MapView(aMapModel, this, mInheritBean, mExitBean));
        mapWindow.setVisible(true);
        mDesktop.add(mapWindow);

        return mapWindow;
    }

    public MapWindow createMapWindow(MapModel aMapModel)
    {
        return createMapWindow(aMapModel, null);
    }

    public void setActiveWindow(MapWindow aMapWindow)
    {
        mActiveMapWindow = aMapWindow;

        if (mActiveMapWindow == null)
        {
            mCloseAction.setEnabled(false);
            mSaveAction.setEnabled(false);
            mExportAction.setEnabled(false);
            mExportOptionsAction.setEnabled(false);
            mSaveAsAction.setEnabled(false);
            mSaveAllAction.setEnabled(false);
            mCutAction.setEnabled(false);
            mCopyAction.setEnabled(false);
            mPasteAction.setEnabled(false);
            mResizeAction.setEnabled(false);
        }
        else
        {
            mCloseAction.setEnabled(true);
            mSaveAction.setEnabled(true);
            mExportAction.setEnabled(true);
            mExportOptionsAction.setEnabled(true);
            mSaveAsAction.setEnabled(true);
            mSaveAllAction.setEnabled(true);
            mCutAction.setEnabled(true);
            mCopyAction.setEnabled(true);
            mPasteAction.setEnabled(true);
            mResizeAction.setEnabled(true);
        }

        fireActiveMapChanged();
    }

    public void addActiveMapListener(ActiveMapListener listener)
    {
        mListenerList.add(ActiveMapListener.class, listener);
    }

    public void removeActiveMapListener(ActiveMapListener listener)
    {
        mListenerList.remove(ActiveMapListener.class, listener);
    }

    protected void fireActiveMapChanged()
    {
        Object[] listeners = mListenerList.getListenerList();

        ActiveMapEvent activeMapEvent = new ActiveMapEvent(this,
            getActiveMapWindow());

        for (int i = listeners.length - 2; i >= 0; i -= 2)
        {
            if (listeners[i] == ActiveMapListener.class)
            {
                ((ActiveMapListener)listeners[i + 1]).activeMapChanged(activeMapEvent);
            }
        }
    }

    public MapWindow getActiveMapWindow()
    {
        return mActiveMapWindow;
    }

    public MapView getActiveMapView()
    {
        MapWindow mapWindow = getActiveMapWindow();

        if (mapWindow == null)
        {
            return null;
        }

        return mapWindow.getMapView();
    }

    public void refreshUndoRedo()
    {
        MapView mapView = getActiveMapView();

        if (mapView != null)
        {
            mUndoAction.setEnabled(mapView.mUndoManager.canUndo());
            mUndoAction.putValue(Action.LONG_DESCRIPTION,
                mapView.mUndoManager.getUndoPresentationName());

            mRedoAction.setEnabled(mapView.mUndoManager.canRedo());
            mRedoAction.putValue(Action.LONG_DESCRIPTION,
                mapView.mUndoManager.getRedoPresentationName());
        }
    }

    static int mPaletteNo = 0;

    class ToolPalette extends JInternalFrame
    {
        public ToolPalette(JFrame aFrame)
        {
            super("Tool Frame", false, false, false, false);

            setDefaultCloseOperation(WindowConstants.HIDE_ON_CLOSE);

            setSize(MainFrame.TOOL_PALETTE_WIDTH, MainFrame.TOOL_PALETTE_HEIGHT);

            setLayer(JDesktopPane.PALETTE_LAYER);

            if (getUI() instanceof javax.swing.plaf.metal.MetalInternalFrameUI)
            {
                ((javax.swing.plaf.metal.MetalInternalFrameUI)getUI()).setPalette(true);
            }

            setLocation(aFrame.getWidth() - MainFrame.TOOL_PALETTE_WIDTH - 30,
                30 + (mPaletteNo++ * MainFrame.TOOL_PALETTE_HEIGHT));

            JTabbedPane pane = new JTabbedPane(JTabbedPane.TOP);

            // Inherit Bean
            mInheritBean = new InheritBean(mFrame);
            addActiveMapListener(mInheritBean);
            mInheritBean.setEnabled(false);
            pane.add(mInheritBean, "Inherit");

            // Exit Bean
            mExitBean = new ExitBean();
            addActiveMapListener(mExitBean);
            mExitBean.setEnabled(false);

            // Brief Room Bean
            mBriefRoomBean = new BriefRoomBean();
            addActiveMapListener(mBriefRoomBean);
            mBriefRoomBean.setEnabled(false);
            JPanel roomPanel = new JPanel(new BorderLayout());
            roomPanel.setBorder(BorderFactory.createEmptyBorder(6, 6, 5, 5));
            JPanel exitPanel = new JPanel(new FlowLayout(FlowLayout.CENTER, 0, 0));
            exitPanel.add(mExitBean);
            roomPanel.add(exitPanel, BorderLayout.CENTER);
            roomPanel.add(mBriefRoomBean, BorderLayout.SOUTH);
            pane.add(roomPanel, "Room");

            // Zone Bean
            mZoneBean = new ZoneBean(mFrame);
            addActiveMapListener(mZoneBean);
            mZoneBean.setEnabled(false);
            JPanel zonePanel = new JPanel(new BorderLayout());
            zonePanel.add(mZoneBean, BorderLayout.CENTER);
            zonePanel.setBorder(BorderFactory.createEmptyBorder(6, 6, 5, 5));
            pane.add(zonePanel, "Area");

            pane.setFont(new Font("Arial", Font.BOLD, 9));
            pane.setOpaque(true);
            setContentPane(pane);
        }

        public void setSelected(boolean aSelection)
        {
            // This stops the internal frame competing with active windows
            // for focus.
        }
    }

    class ToolBarButton extends JButton
    {
        Border mEmptyBorder = BorderFactory.createEmptyBorder(3, 3, 3, 3);
        Border mEtchedBorder = BorderFactory.createCompoundBorder(
            BorderFactory.createEtchedBorder(),
            BorderFactory.createEmptyBorder(1, 1, 1, 1));

        public ToolBarButton(Action aAction)
        {
            super(aAction);

            setText("");

            //setIcon((Icon)aAction.getValue("largeIcon"));

            //setBorder(mEmptyBorder);

            final Action action = aAction;

            addMouseListener(new MouseAdapter()
            {
                public void mouseEntered(MouseEvent e)
                {
                    //setBorder(mEtchedBorder);
                    setStatus((String)action.getValue(Action.LONG_DESCRIPTION));
                }

                public void mouseExited(MouseEvent e)
                {
                    //setBorder(mEmptyBorder);
                    setStatus(" ");
                }
            });
        }
    }

    class NewAction extends AbstractAction
    {
        private static final String NAME = "New";
        private static final String SMALL_ICON = "New16.gif";
        private static final String LARGE_ICON = "New24.gif";
        private static final String SHORT_DESCRIPTION = "New";
        private static final String LONG_DESCRIPTION = "Create a new map";
        private static final int MNEMONIC_KEY = 'N';

        public NewAction()
        {
            putValue(Action.NAME, NAME);
            putValue(Action.SMALL_ICON, Utils.getIcon(SMALL_ICON));
            putValue("largeIcon", Utils.getIcon(LARGE_ICON));
            putValue(Action.SHORT_DESCRIPTION, SHORT_DESCRIPTION);
            putValue(Action.LONG_DESCRIPTION, LONG_DESCRIPTION);
            putValue(Action.MNEMONIC_KEY, new Integer(MNEMONIC_KEY));
            putValue(Action.ACCELERATOR_KEY, KeyStroke.getKeyStroke(KeyEvent.VK_N, KeyEvent.CTRL_MASK));
            putValue(Action.ACTION_COMMAND_KEY, ACTION_COMMAND_KEY);
        }

        public void actionPerformed(ActionEvent e)
        {
            Dimension mapSize = MapSizeDialog.showNewRoomDialog(mFrame, new Dimension(500, 500));

            if (mapSize == null)
            {
                return;
            }

            MapWindow mapWindow = createMapWindow(new MapModel(mapSize));

            try
            {
                mapWindow.setSelected(true);
            }
            catch (java.beans.PropertyVetoException ex)
            {
                ex.printStackTrace();
            }
        }
    }

    class OpenAction extends AbstractAction
    {
        private static final String NAME = "Open";
        private static final String SMALL_ICON = "Open16.gif";
        private static final String LARGE_ICON = "Open24.gif";
        private static final String SHORT_DESCRIPTION = "Open";
        private static final String LONG_DESCRIPTION = "Open a map";
        private static final int MNEMONIC_KEY = 'O';

        public OpenAction()
        {
            putValue(Action.NAME, NAME);
            putValue(Action.SMALL_ICON, Utils.getIcon(SMALL_ICON));
            putValue("largeIcon", Utils.getIcon(LARGE_ICON));
            putValue(Action.SHORT_DESCRIPTION, SHORT_DESCRIPTION);
            putValue(Action.LONG_DESCRIPTION, LONG_DESCRIPTION);
            putValue(Action.MNEMONIC_KEY, new Integer(MNEMONIC_KEY));
            putValue(Action.ACTION_COMMAND_KEY, ACTION_COMMAND_KEY);
        }

        public void actionPerformed(ActionEvent e)
        {
            MapModel mapModel = null;

            int returnVal = mFileChooser.showOpenDialog(mFrame);

            if (returnVal != JFileChooser.APPROVE_OPTION)
            {
                return;
            }

            File file = mFileChooser.getSelectedFile();
            String filename = file.getAbsolutePath();

            try
            {
                FileInputStream in = new FileInputStream(filename);
                ObjectInputStream s = new ObjectInputStream(in);
                mapModel = (MapModel)s.readObject();
            }
            catch (Exception ex)
            {
                ex.printStackTrace();
            }

            MapWindow mapWindow = createMapWindow(mapModel, filename);

            try
            {
                mapWindow.setSelected(true);
            }
            catch (java.beans.PropertyVetoException ex)
            {
                ex.printStackTrace();
            }
        }
    }

    class CloseAction extends AbstractAction
    {
        private static final String NAME = "Close";
        private static final String SMALL_ICON = "Blank16.gif";
        private static final String LARGE_ICON = "Blank24.gif";
        private static final String SHORT_DESCRIPTION = "Close";
        private static final String LONG_DESCRIPTION = "Close the current map";
        private static final int MNEMONIC_KEY = 'C';

        public CloseAction()
        {
            putValue(Action.NAME, NAME);
            putValue(Action.SMALL_ICON, Utils.getIcon(SMALL_ICON));
            putValue("largeIcon", Utils.getIcon(LARGE_ICON));
            putValue(Action.SHORT_DESCRIPTION, SHORT_DESCRIPTION);
            putValue(Action.LONG_DESCRIPTION, LONG_DESCRIPTION);
            putValue(Action.MNEMONIC_KEY, new Integer(MNEMONIC_KEY));
            putValue(Action.ACTION_COMMAND_KEY, ACTION_COMMAND_KEY);
        }

        public void actionPerformed(ActionEvent e)
        {

        }
    }

    class SaveAction extends AbstractAction
    {
        private static final String NAME = "Save";
        private static final String SMALL_ICON = "Save16.gif";
        private static final String LARGE_ICON = "Save24.gif";
        private static final String SHORT_DESCRIPTION = "Save";
        private static final String LONG_DESCRIPTION = "Save the current map";
        private static final int MNEMONIC_KEY = 'S';

        public SaveAction()
        {
            putValue(Action.NAME, NAME);
            putValue(Action.SMALL_ICON, Utils.getIcon(SMALL_ICON));
            putValue("largeIcon", Utils.getIcon(LARGE_ICON));
            putValue(Action.SHORT_DESCRIPTION, SHORT_DESCRIPTION);
            putValue(Action.LONG_DESCRIPTION, LONG_DESCRIPTION);
            putValue(Action.MNEMONIC_KEY, new Integer(MNEMONIC_KEY));
            putValue(Action.ACTION_COMMAND_KEY, ACTION_COMMAND_KEY);
        }

        public void actionPerformed(ActionEvent e)
        {
            MapWindow mapWindow = getActiveMapWindow();
            MapModel mapModel = getActiveMapView().getMapModel();

            String filename = mapModel.getFilename();

            if (mapModel.getFilename() == null)
            {
                int returnVal = mFileChooser.showSaveDialog(mFrame);

                if (returnVal == JFileChooser.APPROVE_OPTION)
                {
                    File file = mFileChooser.getSelectedFile();
                    filename = file.getAbsolutePath();
                    mapModel.setFilename(filename);
                    mapWindow.setTitle(filename);
                }
                else
                {
                    return;
                }
            }

            try
            {
                FileOutputStream out = new FileOutputStream(filename);
                ObjectOutputStream s = new ObjectOutputStream(out);
                s.writeObject(getActiveMapView().getMapModel());
                s.flush();
            }
            catch (Exception ex)
            {
                ex.printStackTrace();
            }
        }
    }

    class ExportOptionsAction extends AbstractAction
    {
        private static final String NAME = "Export Options...";
        private static final String SMALL_ICON = "Blank16.gif";
        private static final String LARGE_ICON = "Blank24.gif";
        private static final String SHORT_DESCRIPTION = "Export Options";
        private static final String LONG_DESCRIPTION = "Change the map's export options";
        private static final int MNEMONIC_KEY = 'P';

        public ExportOptionsAction()
        {
            putValue(Action.NAME, NAME);
            putValue(Action.SMALL_ICON, Utils.getIcon(SMALL_ICON));
            putValue("largeIcon", Utils.getIcon(LARGE_ICON));
            putValue(Action.SHORT_DESCRIPTION, SHORT_DESCRIPTION);
            putValue(Action.LONG_DESCRIPTION, LONG_DESCRIPTION);
            putValue(Action.MNEMONIC_KEY, new Integer(MNEMONIC_KEY));
            putValue(Action.ACTION_COMMAND_KEY, ACTION_COMMAND_KEY);
        }

        public void actionPerformed(ActionEvent e)
        {
            MapWindow mapWindow = getActiveMapWindow();
            MapModel mapModel = getActiveMapView().getMapModel();

            ExportOptions exportOptions = mapModel.getExportOptions();
            exportOptions = ExportOptionsDialog.showExportOptionsDialog(mFrame,
                exportOptions);
            if (exportOptions != null)
            {
                mapModel.setExportOptions(exportOptions);
            }
        }
    }

    class ExportAction extends AbstractAction
    {
        private static final String NAME = "Export";
        private static final String SMALL_ICON = "Export16.gif";
        private static final String LARGE_ICON = "Export24.gif";
        private static final String SHORT_DESCRIPTION = "Export";
        private static final String LONG_DESCRIPTION = "Export the current map as LPC";
        private static final int MNEMONIC_KEY = 'X';

        Hashtable mFormatHashtable = new Hashtable();

        String mIndent = "";

/*
        public String columnFormat(String aString, int aLeading, int aCols,
            int aIndent)
        {
            int firstLineMax = aCols - aLeading;
            String str;
            int lastSpace;
            int nextLastSpace;

            str = aString.substring(0, firstLineMax);
            lastSpace = str.lastIndexOf(' ');
            str = str.substring(0, lastSpace + 1);

            System.out.println("    set_long(\"" + str + "\"");

            str = aString.substring(lastSpace + 1, lastSpace + aCols);
            nextLastSpace = lastSpace + str.lastIndexOf(' ') + 2;
            lastSpace = str.lastIndexOf(' ');
            str = str.substring(0, lastSpace + 1);

            System.out.println("        \"" + str + "\"");

            str = aString.substring(nextLastSpace, nextLastSpace + aCols);
            nextLastSpace = nextLastSpace + str.lastIndexOf(' ') + 2;
            lastSpace = str.lastIndexOf(' ');
            str = str.substring(0, lastSpace + 1);

            System.out.println("        \"" + str + "\"");

            try
            {
                str = aString.substring(nextLastSpace, nextLastSpace + aCols);
            }
            catch (StringIndexOutOfBoundsException ex)
            {
                str = aString.substring(nextLastSpace, aString.length());
            }

            nextLastSpace = nextLastSpace + str.lastIndexOf(' ') + 2;
            lastSpace = str.lastIndexOf(' ');

            try
            {
                str = str.substring(0, lastSpace + 1);
            }
            catch (StringIndexOutOfBoundsException ex)
            {
                lastSpace = str.length();
                str = str.substring(0, lastSpace);
            }

            System.out.println("        \"" + str + "\"");

            return "";
        }
*/

        public ExportAction()
        {
            putValue(Action.NAME, NAME);
            putValue(Action.SMALL_ICON, Utils.getIcon(SMALL_ICON));
            putValue("largeIcon", Utils.getIcon(LARGE_ICON));
            putValue(Action.SHORT_DESCRIPTION, SHORT_DESCRIPTION);
            putValue(Action.LONG_DESCRIPTION, LONG_DESCRIPTION);
            putValue(Action.MNEMONIC_KEY, new Integer(MNEMONIC_KEY));
            putValue(Action.ACTION_COMMAND_KEY, ACTION_COMMAND_KEY);

            mFormatHashtable.put("light", SET_LIGHT);
            mFormatHashtable.put("size", SET_ROOM_SIZE);
            mFormatHashtable.put("determinate", SET_DETERMINATE);
            mFormatHashtable.put("short", SET_SHORT);
            mFormatHashtable.put("long", SET_LONG);
            mFormatHashtable.put("dayLong", SET_DAY_LONG);
            mFormatHashtable.put("nightLong", SET_NIGHT_LONG);
        }

        public String getFormat(String aProperty)
        {
            return (String)mFormatHashtable.get(aProperty);
        }

        public void setIndent(int aIndentWidth)
        {
            String indent = "";

            for (int i = 0; i < aIndentWidth; i++)
            {
                indent += " ";
            }

            mIndent = indent;
        }

        public void writeLine(FileWriter aWriter, String aLine)
            throws IOException
        {
            aWriter.write(mIndent);
            aWriter.write(aLine);
        }

        public void writeProperty(FileWriter aWriter, MapLocation aLocation,
            String aProperty) throws IOException, NoSuchRoomPropertyException
        {
            if (aLocation.isPropertyInherited(aProperty))
            {
                return;
            }

            String property = (String)aLocation.getProperty(aProperty,
                RoomProperties.INHERIT_DO_CHECK);

            if (property == null)
            {
                return;
            }

            property = MessageFormat.format(getFormat(aProperty),
                new Object[] { property });

            writeLine(aWriter, property);
        }

        public void writeExit(FileWriter aWriter, Exit aExit)
            throws IOException
        {
            String toFilename;
            MapLocation to = aExit.getTo();

            if (to.isUseAutoName())
            {
                toFilename = to.getAutoName();
            }
            else
            {
                toFilename = to.getFileName();
            }

            // Need to add support for up/down and non-standard exits.
            String direction = Direction.getDirectionName(aExit.getDirection());
            String type = aExit.getType();
            Object[] params = new Object[] { direction, toFilename, type };
            String exitString = MessageFormat.format(ADD_EXIT, params);

            writeLine(aWriter, exitString);
        }

        public void writeRoom(File aFile, MapLocation aLocation)
        {
            FileWriter writer = null;

            try
            {
                writer = new FileWriter(aFile);

                writeProperty(writer, aLocation, "size");
                writeProperty(writer, aLocation, "light");

                // check for "None", allow empty
                writeProperty(writer, aLocation, "determinate");

                writeProperty(writer, aLocation, "short");
                writeProperty(writer, aLocation, "long");
                writeProperty(writer, aLocation, "dayLong");
                writeProperty(writer, aLocation, "nightLong");

                // Need to sort these.
                Vector exitsVector = aLocation.getExits();
                Exit[] exits = new Exit[exitsVector.size()];
                exitsVector.toArray(exits);
                Arrays.sort(exits);

                for (int i = 0; i < exits.length; i++)
                {
                    writeExit(writer, exits[i]);
                }

                writer.close();
            }
            catch (Exception ex)
            {
                ex.printStackTrace();
            }
        }

        public void actionPerformed(ActionEvent aActionEvent)
        {
/*
            columnFormat("This is a path through Lancre Forest.  There are "
                + "lots of trees here.  It's really very, very nice.  Lovely "
                + "in fact.  Really very, very lovely.  I could go on all day "
                + "about how nice and lovely it is.  It really is.  Nice.  "
                + "Lovely.\n", 13, 80, 4);
*/

            ExportDialog exportDialog = ExportDialog.showExportDialog(mFrame);

            if (!exportDialog.isOkay())
            {
                return;
            }

            int exportType = exportDialog.getExportType();
            String directory = exportDialog.getDirectory();

            switch (exportType)
            {
                case ExportDialog.EXPORT_LPC :
                    SliderDialog sliderDialog = new SliderDialog(mFrame,
                        "Exporting LPC", "Exporting file: ", "Progress: ");

                    sliderDialog.setVisible(true);

                    // Maybe need a thread in here.

                    MapWindow mapWindow = getActiveMapWindow();
                    MapModel mapModel = getActiveMapView().getMapModel();
                    Vector inherits = mapModel.getInheritables();
                    Vector mapLocations = mapModel.getMapLocations();

                    int total = inherits.size() + mapLocations.size();

                    for (Enumeration e = mapLocations.elements(); e.hasMoreElements();)
                    {
                        MapLocation location = (MapLocation)e.nextElement();

                        String filename = location.getFileName();

                        File file = new File(directory, filename + ".c");

                        writeRoom(file, location);
                    }

                    for (Enumeration e = mapLocations.elements(); e.hasMoreElements();)
                    {
                        MapLocation location = (MapLocation)e.nextElement();

                        String filename;

                        if (location.isUseAutoName())
                        {
                            filename = location.getAutoName();
                        }
                        else
                        {
                            filename = location.getFileName();
                        }

                        File file = new File(directory, filename + ".c");

                        writeRoom(file, location);
                    }

                    //sliderDialog.setVisible(false);
                    break;
                }
            }

/*



                String light = location.getLight();
                String roomShort = location.getShort();
                String determinate = location.getDeterminate();
                int roomSize = location.getRoomSize();
                String roomLong = location.getLong();
                String dayLong = location.getDayLong();
                String nightLong = location.getNightLong();

                String str;

                if (!location.isLightInherited())
                {
                    if (light != null && light.compareTo("") != 0)
                    {
                        str = getLine(SET_LIGHT, light);
                        System.out.println(str);
                    }
                }

                if (!location.isShortInherited())
                {
                    if (roomShort != null && roomShort.compareTo("") != 0)
                    {
                        str = getLine(SET_SHORT, roomShort);
                        System.out.println(str);
                    }
                }

                if (!location.isDeterminateInherited())
                {
                    if (determinate != null
                        && determinate.toLowerCase().compareTo("none") != 0)
                    {
                        str = getLine(SET_DETERMINATE, determinate);
                        System.out.println(str);
                    }
                }


            }
*/

        public String getLine(String aFormat, String aParam)
        {
            String str = MessageFormat.format(aFormat, new Object[] { aParam });

            return str;
        }

    }


    class SaveAsAction extends AbstractAction
    {
        private static final String NAME = "Save As";
        private static final String SMALL_ICON = "SaveAs16.gif";
        private static final String LARGE_ICON = "SaveAs24.gif";
        private static final String SHORT_DESCRIPTION = "Save As";
        private static final String LONG_DESCRIPTION = "Save the current map as a specified filename";
        private static final int MNEMONIC_KEY = 'V';

        public SaveAsAction()
        {
            putValue(Action.NAME, NAME);
            putValue(Action.SMALL_ICON, Utils.getIcon(SMALL_ICON));
            putValue("largeIcon", Utils.getIcon(LARGE_ICON));
            putValue(Action.SHORT_DESCRIPTION, SHORT_DESCRIPTION);
            putValue(Action.LONG_DESCRIPTION, LONG_DESCRIPTION);
            putValue(Action.MNEMONIC_KEY, new Integer(MNEMONIC_KEY));
            putValue(Action.ACTION_COMMAND_KEY, ACTION_COMMAND_KEY);
        }

        public void actionPerformed(ActionEvent e)
        {
            MapWindow mapWindow = getActiveMapWindow();
            MapModel mapModel = getActiveMapView().getMapModel();

            String filename;

                int returnVal = mFileChooser.showSaveDialog(mFrame);

            if (returnVal == JFileChooser.APPROVE_OPTION)
            {
                File file = mFileChooser.getSelectedFile();
                filename = file.getAbsolutePath();
                mapModel.setFilename(filename);
                mapWindow.setTitle(filename);
            }
            else
            {
                return;
            }

            try
            {
                FileOutputStream out = new FileOutputStream(filename);
                ObjectOutputStream s = new ObjectOutputStream(out);
                s.writeObject(getActiveMapView().getMapModel());
                s.flush();
            }
            catch (Exception ex)
            {
                ex.printStackTrace();
            }
        }
    }

    class SaveAllAction extends AbstractAction
    {
        private static final String NAME = "Save All";
        private static final String SMALL_ICON = "SaveAll16.gif";
        private static final String LARGE_ICON = "SaveAll24.gif";
        private static final String SHORT_DESCRIPTION = "Save All";
        private static final String LONG_DESCRIPTION = "Save all open maps";
        private static final int MNEMONIC_KEY = 'A';

        public SaveAllAction()
        {
            putValue(Action.NAME, NAME);
            putValue(Action.SMALL_ICON, Utils.getIcon(SMALL_ICON));
            putValue("largeIcon", Utils.getIcon(LARGE_ICON));
            putValue(Action.SHORT_DESCRIPTION, SHORT_DESCRIPTION);
            putValue(Action.LONG_DESCRIPTION, LONG_DESCRIPTION);
            putValue(Action.MNEMONIC_KEY, new Integer(MNEMONIC_KEY));
            putValue(Action.ACTION_COMMAND_KEY, ACTION_COMMAND_KEY);
        }

        public void actionPerformed(ActionEvent e)
        {

        }
    }

    class UndoAction extends AbstractAction
    {
        private static final String NAME = "Undo";
        private static final String SMALL_ICON = "Undo16.gif";
        private static final String LARGE_ICON = "Undo24.gif";
        private static final String SHORT_DESCRIPTION = "Undo";
        private static final String LONG_DESCRIPTION = "Undo";
        private static final int MNEMONIC_KEY = 'U';

        public UndoAction()
        {
            putValue(Action.NAME, NAME);
            putValue(Action.SMALL_ICON, Utils.getIcon(SMALL_ICON));
            putValue("largeIcon", Utils.getIcon(LARGE_ICON));
            putValue(Action.SHORT_DESCRIPTION, SHORT_DESCRIPTION);
            putValue(Action.LONG_DESCRIPTION, LONG_DESCRIPTION);
            putValue(Action.MNEMONIC_KEY, new Integer(MNEMONIC_KEY));
            putValue(Action.ACTION_COMMAND_KEY, ACTION_COMMAND_KEY);
        }

        public void actionPerformed(ActionEvent e)
        {
            getActiveMapView().mUndoManager.undo();
            getActiveMapView().repaint();
            refreshUndoRedo();
            setStatus((String)getValue(Action.LONG_DESCRIPTION));
        }
    }

    class RedoAction extends AbstractAction
    {
        private static final String NAME = "Redo";
        private static final String SMALL_ICON = "Redo16.gif";
        private static final String LARGE_ICON = "Redo24.gif";
        private static final String SHORT_DESCRIPTION = "Redo";
        private static final String LONG_DESCRIPTION = "Redo";
        private static final int MNEMONIC_KEY = 'R';

        public RedoAction()
        {
            putValue(Action.NAME, NAME);
            putValue(Action.SMALL_ICON, Utils.getIcon(SMALL_ICON));
            putValue("largeIcon", Utils.getIcon(LARGE_ICON));
            putValue(Action.SHORT_DESCRIPTION, SHORT_DESCRIPTION);
            putValue(Action.LONG_DESCRIPTION, LONG_DESCRIPTION);
            putValue(Action.MNEMONIC_KEY, new Integer(MNEMONIC_KEY));
            putValue(Action.ACTION_COMMAND_KEY, ACTION_COMMAND_KEY);
        }

        public void actionPerformed(ActionEvent e)
        {
            getActiveMapView().mUndoManager.redo();
            getActiveMapView().repaint();
            refreshUndoRedo();
            setStatus((String)getValue(Action.LONG_DESCRIPTION));
        }
    }

    class CutAction extends AbstractAction
    {
        private static final String NAME = "Cut";
        private static final String SMALL_ICON = "Cut16.gif";
        private static final String LARGE_ICON = "Cut24.gif";
        private static final String SHORT_DESCRIPTION = "Cut";
        private static final String LONG_DESCRIPTION = "Cut selection to clip board";
        private static final int MNEMONIC_KEY = 'T';

        public CutAction()
        {
            putValue(Action.NAME, NAME);
            putValue(Action.SMALL_ICON, Utils.getIcon(SMALL_ICON));
            putValue("largeIcon", Utils.getIcon(LARGE_ICON));
            putValue(Action.SHORT_DESCRIPTION, SHORT_DESCRIPTION);
            putValue(Action.LONG_DESCRIPTION, LONG_DESCRIPTION);
            putValue(Action.MNEMONIC_KEY, new Integer(MNEMONIC_KEY));
            putValue(Action.ACTION_COMMAND_KEY, ACTION_COMMAND_KEY);
        }

        public void actionPerformed(ActionEvent e)
        {

        }
    }

    class CopyAction extends AbstractAction
    {
        private static final String NAME = "Copy";
        private static final String SMALL_ICON = "Copy16.gif";
        private static final String LARGE_ICON = "Copy24.gif";
        private static final String SHORT_DESCRIPTION = "Copy";
        private static final String LONG_DESCRIPTION = "Copy selection to clip board";
        private static final int MNEMONIC_KEY = 'C';

        public CopyAction()
        {
            putValue(Action.NAME, NAME);
            putValue(Action.SMALL_ICON, Utils.getIcon(SMALL_ICON));
            putValue("largeIcon", Utils.getIcon(LARGE_ICON));
            putValue(Action.SHORT_DESCRIPTION, SHORT_DESCRIPTION);
            putValue(Action.LONG_DESCRIPTION, LONG_DESCRIPTION);
            putValue(Action.MNEMONIC_KEY, new Integer(MNEMONIC_KEY));
            putValue(Action.ACTION_COMMAND_KEY, ACTION_COMMAND_KEY);
        }

        public void actionPerformed(ActionEvent e)
        {

        }
    }

    class PasteAction extends AbstractAction
    {
        private static final String NAME = "Paste";
        private static final String SMALL_ICON = "Paste16.gif";
        private static final String LARGE_ICON = "Paste24.gif";
        private static final String SHORT_DESCRIPTION = "Paste";
        private static final String LONG_DESCRIPTION = "Paste from clip board";
        private static final int MNEMONIC_KEY = 'P';

        public PasteAction()
        {
            putValue(Action.NAME, NAME);
            putValue(Action.SMALL_ICON, Utils.getIcon(SMALL_ICON));
            putValue("largeIcon", Utils.getIcon(LARGE_ICON));
            putValue(Action.SHORT_DESCRIPTION, SHORT_DESCRIPTION);
            putValue(Action.LONG_DESCRIPTION, LONG_DESCRIPTION);
            putValue(Action.MNEMONIC_KEY, new Integer(MNEMONIC_KEY));
            putValue(Action.ACTION_COMMAND_KEY, ACTION_COMMAND_KEY);
        }

        public void actionPerformed(ActionEvent e)
        {

        }
    }

    class ExitAction extends AbstractAction
    {
        private static final String NAME = "Exit";
        private static final String SMALL_ICON = "Blank16.gif";
        private static final String LARGE_ICON = "Blank24.gif";
        private static final String SHORT_DESCRIPTION = "Exit";
        private static final String LONG_DESCRIPTION = "Exit this application";
        private static final int MNEMONIC_KEY = 'X';

        public ExitAction()
        {
            putValue(Action.NAME, NAME);
            putValue(Action.SMALL_ICON, Utils.getIcon(SMALL_ICON));
            putValue("largeIcon", Utils.getIcon(LARGE_ICON));
            putValue(Action.SHORT_DESCRIPTION, SHORT_DESCRIPTION);
            putValue(Action.LONG_DESCRIPTION, LONG_DESCRIPTION);
            putValue(Action.MNEMONIC_KEY, new Integer(MNEMONIC_KEY));
            putValue(Action.ACTION_COMMAND_KEY, ACTION_COMMAND_KEY);
        }

        public void actionPerformed(ActionEvent e)
        {

        }
    }

    class ResizeAction extends AbstractAction
    {
        private static final String NAME = "Resize";
        private static final String SMALL_ICON = "Blank16.gif";
        private static final String LARGE_ICON = "Blank24.gif";
        private static final String SHORT_DESCRIPTION = "Resize";
        private static final String LONG_DESCRIPTION = "Resize the current map";
        private static final int MNEMONIC_KEY = 'X';

        public ResizeAction()
        {
            putValue(Action.NAME, NAME);
            putValue(Action.SMALL_ICON, Utils.getIcon(SMALL_ICON));
            putValue("largeIcon", Utils.getIcon(LARGE_ICON));
            putValue(Action.SHORT_DESCRIPTION, SHORT_DESCRIPTION);
            putValue(Action.LONG_DESCRIPTION, LONG_DESCRIPTION);
            putValue(Action.MNEMONIC_KEY, new Integer(MNEMONIC_KEY));
            putValue(Action.ACTION_COMMAND_KEY, ACTION_COMMAND_KEY);
        }

        public void actionPerformed(ActionEvent e)
        {
            MapWindow mapWindow = getActiveMapWindow();
            MapView mapView = getActiveMapView();
            Dimension mapSize = MapSizeDialog.showNewRoomDialog(mFrame, mapView.getSize());

            if (mapSize != null)
            {
                mapView.setMapSize(mapSize);
            }

            mapWindow.repaint();
            mapWindow.revalidate();
        }
    }
}

class MapFileFilter implements FileFilter
{
    public boolean accept(File aFile)
    {
        String name = aFile.getName();

        int i = name.lastIndexOf('.');

        if (i > 0 &&  i < name.length() - 1)
        {
            String extension = name.substring(i + 1).toLowerCase();

            if (extension.equals("map"))
            {
                return true;
            }
        }
        return false;
    }
}
