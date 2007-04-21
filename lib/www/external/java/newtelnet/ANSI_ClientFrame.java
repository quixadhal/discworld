import java.io.*;
import java.net.*;
import java.util.*;

import java.awt.*;
import java.awt.event.*;

public class ANSI_ClientFrame extends Frame implements WindowListener {

    public ANSI_ClientFrame(String title) {
        super(title);
        addWindowListener(this);
    }

    public void windowOpened(WindowEvent e) {
    }

    public void windowClosing(WindowEvent e) {
        dispose();
        System.exit(0);
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

