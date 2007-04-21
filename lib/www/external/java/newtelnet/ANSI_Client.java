import java.io.*;
import java.net.*;
import java.util.*;

import java.awt.*;
import java.applet.*;
import java.awt.event.*;

public class ANSI_Client extends Applet implements ActionListener, KeyListener, WindowListener {
    private TextField inputLine = new TextField();
    private Scrollbar scrolly = new Scrollbar();
    private ANSI_ClientDisplay display = new ANSI_ClientDisplay(scrolly);
    private String host = "localhost";
    private int port = 23;
    private boolean buttons = true;
    private boolean isStopped = true;
    private Button connect = new Button("Connect");
    private Button disconnect = new Button("Disconnect");
    private Button cmdLFont = new Button("Large Font");
    private Button cmdSFont = new Button("Small Font");
    private Button cmdDetach = new Button("Detach");
    private Button cmdAttach = new Button("Attach");
    private Font lFont = new Font("Courier", Font.BOLD, 14);
    private Font sFont = new Font("Courier", Font.PLAIN, 12);
    private CommandHistory history = new CommandHistory();
    private Panel p2 = new Panel(new BorderLayout());
    private Panel p1 = new Panel(new GridLayout(1, 5, 1, 1));
    private Panel p0 = new Panel(new BorderLayout());
    private Frame frame = null;

    public static final int MAX_COMMANDS = 32;

    public static boolean isApp = false;

    private class CommandHistory {
        private String[] buf = new String[MAX_COMMANDS];
        private int length = 0;
        private int position = 0;

        public void add(String s) {
            for (int i = 0; i < length; i++) {
                if (buf[i].equals(s)) {
                    if (i < length - 1) {
                        System.arraycopy(buf, i + 1, buf, i, length - 1 - i);
                    }
                    length --;
                    break;
                }
            }
            if (length >= MAX_COMMANDS) {
                System.arraycopy(buf, 1, buf, 0, length - 1);
                buf[MAX_COMMANDS - 1] = s;
            }
            else {
                buf[length++] = s;
            }
            position = length;
        }

        public String up() {
            if (position >= 0) {
                position--;
            }
            if (position < 0) {
                return "";
            }
            return buf[position];
        }

        public String down() {
            if (position < length) {
                position++;
            }
            if (position >= length) {
                return "";
            }
            return buf[position];
        }
    }

    public ANSI_Client() {
        super();

        inputLine.setFont(sFont);
        display.setFont(sFont);

        setLayout(new BorderLayout(0, 1));
        setBackground(Color.white);
        p0.setBackground(Color.lightGray);

        p0.add(display, BorderLayout.CENTER);
        p0.add(inputLine, BorderLayout.SOUTH);
        p0.add(scrolly, BorderLayout.EAST);
        inputLine.addKeyListener(this);
        inputLine.addActionListener(this);

        connect.addActionListener(this);
        connect.setActionCommand("#connect");
        disconnect.addActionListener(this);
        disconnect.setActionCommand("#disconnect");
        cmdLFont.addActionListener(this);
        cmdLFont.setActionCommand("#lfont");
        cmdSFont.addActionListener(this);
        cmdSFont.setActionCommand("#sfont");
        cmdDetach.addActionListener(this);
        cmdDetach.setActionCommand("#detach");
        cmdAttach.addActionListener(this);
        cmdAttach.setActionCommand("#attach");

        p1.setBackground(Color.lightGray);
        p1.add(connect);
        p1.add(disconnect);
        p1.add(cmdLFont);
        p1.add(cmdSFont);
        if (!isApp) {
            p1.add(cmdDetach);
        }

        p2.setBackground(Color.lightGray);
        p2.add(p1, BorderLayout.WEST);

        if (buttons) {
            p0.add(p2, BorderLayout.NORTH);
        }
        add(p0, BorderLayout.CENTER);
    }

    public static void main(String args[]) {
        isApp = true;
        Frame frame = new Frame("Simple MUD Client v1.02");
        ANSI_Client client = new ANSI_Client();
        frame.addWindowListener(client);
        frame.setLayout(new BorderLayout());
        frame.add(client, BorderLayout.CENTER);
        client.init();
        try {
            client.host = "discworld.imaginary.com";//args[0];
            client.port = 4242;//Integer.parseInt(args[1]);
        }
        catch (Exception e) {
        }
        
        client.start();
        frame.pack();
        frame.setVisible(true);
    }

    public void init() {
        try {
            host = getParameter("host");
            port = Integer.parseInt(getParameter("port"));
            buttons = getParameter("buttons").equalsIgnoreCase("Y");
        }
        catch (Exception e) {
        }
        if (buttons) {
            p0.add(p2, BorderLayout.NORTH);
        }
        else {
            p0.remove(p2);
        }
    }

    public void start() {
        if (frame == null) {
            display.connect(host, port);
            inputLine.requestFocus();
            super.start();
        }                 
            
        isStopped = false;
    }

    public void stop() {
        if (frame == null) {
            display.close();
            super.stop();
        }
        isStopped = true;
    }

    public synchronized void actionPerformed(ActionEvent e) {
        String command = e.getActionCommand();
        if (command.equals("#disconnect")) {
            display.close();
            inputLine.requestFocus();
        }
        else if (command.equals("#connect")) {
            display.connect(host, port);
            inputLine.requestFocus();
        }
        else if (command.equals("#lfont")) {
            display.setFont(lFont);
            inputLine.setFont(lFont);
            repaint();
            inputLine.requestFocus();
        }
        else if (command.equals("#sfont")) {
            display.setFont(sFont);
            inputLine.setFont(sFont);
            repaint();
            inputLine.requestFocus();
        }
        else if (command.equals("#detach")) {
            detach();
        }
        else if (command.equals("#attach")) {
            attach();
        }
        else {
            String s = inputLine.getText();
            //inputLine.setText("");
            inputLine.setCaretPosition(s.length());
            inputLine.selectAll();
            display.send(s);
            history.add(s);
        }
    }

    public synchronized void keyPressed(KeyEvent e) {
        if (e.getModifiers() != 0) {
            return;
        }
        switch (e.getKeyCode()) {
            case KeyEvent.VK_PAGE_UP:
                display.pgUp();
                e.consume();
                break;

            case KeyEvent.VK_PAGE_DOWN:
                display.pgDown();
                e.consume();
                break;

            case KeyEvent.VK_NUMPAD1:
                display.send("sw");
                e.consume();
                break;

            case KeyEvent.VK_NUMPAD2:
                display.send("s");
                e.consume();
                break;

            case KeyEvent.VK_NUMPAD3:
                display.send("se");
                e.consume();
                break;

            case KeyEvent.VK_NUMPAD4:
                display.send("w");
                e.consume();
                break;

            case KeyEvent.VK_NUMPAD5:
                display.send("look");
                e.consume();
                break;

            case KeyEvent.VK_NUMPAD6:
                display.send("e");
                e.consume();
                break;

            case KeyEvent.VK_NUMPAD7:
                display.send("nw");
                e.consume();
                break;

            case KeyEvent.VK_NUMPAD8:
                display.send("n");
                e.consume();
                break;

            case KeyEvent.VK_NUMPAD9:
                display.send("ne");
                e.consume();
                break;

            case KeyEvent.VK_NUMPAD0:
                display.send("score");
                e.consume();
                break;

            case KeyEvent.VK_ESCAPE:
                inputLine.setText("");
                e.consume();
                break;

            case KeyEvent.VK_UP:
                String newText = history.up();
                if (newText.equals(inputLine.getText())) {
                    newText = history.up();
                }
                inputLine.setText(newText);
                inputLine.selectAll();
                e.consume();
                break;

            case KeyEvent.VK_DOWN:
                inputLine.setText(history.down());
                inputLine.selectAll();
                e.consume();
                break;

            case KeyEvent.VK_F1:
                display.send("help");
                e.consume();
                break;

            case KeyEvent.VK_F2:
                display.send("inventory");
                e.consume();
                break;

            case KeyEvent.VK_F3:
                display.send("get all from corpse");
                // intentional flowthrough
            case KeyEvent.VK_F4:
                display.send("bury corpse");
                e.consume();
                break;

            case KeyEvent.VK_F5:
                display.send("stop");
                e.consume();
                break;

            case KeyEvent.VK_F6:
                display.send("save");
                e.consume();
                break;

            case KeyEvent.VK_F7:
                display.send("sneak high");
                e.consume();
                break;

            case KeyEvent.VK_F8:
                display.send("sneak off");
                e.consume();
                break;

            case KeyEvent.VK_F9:
                display.send("lose all");
                e.consume();
                break;

            case KeyEvent.VK_F11:
                display.send("health all");
                e.consume();
                break;

            case KeyEvent.VK_F12:
                display.send("consider all");
                e.consume();
                break;

        }
    }

    public void keyReleased(KeyEvent e) {
    }
    public void keyTyped(KeyEvent e) {
    }

    public void detach() {
        p1.remove(cmdDetach);
        p1.add(cmdAttach);
        remove(p0);
        repaint();
        frame = new Frame("Simple MUD Client v1.02");
        frame.addWindowListener(this);
        frame.setLayout(new BorderLayout());
        frame.add(p0, BorderLayout.CENTER);
        frame.pack();
        frame.setVisible(true);
        inputLine.requestFocus();
    }

    public void attach() {
        if (frame == null) {
            return;
        }
        frame.remove(p0);
        add(p0, BorderLayout.CENTER);
        p1.remove(cmdAttach);
        p1.add(cmdDetach);
        frame.dispose();
        frame = null;
        doLayout();
        p0.doLayout();
        repaint();
        inputLine.requestFocus();
        if (isStopped) {
            stop();
        }
    }

    public void windowOpened(WindowEvent e) {
    }

    public void windowClosing(WindowEvent e) {
        if (isApp) {
            System.exit(0);
        }
        else {
            attach();
        }
    }

    public void windowClosed(WindowEvent e) {
    }

    public void windowIconified(WindowEvent e) {
    }

    public void windowDeiconified(WindowEvent e) {
    }

    public void windowActivated(WindowEvent e) {
    }


    public void windowDeactivated(WindowEvent e) {
    }

}
