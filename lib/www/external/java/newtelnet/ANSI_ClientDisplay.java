import java.io.*;
import java.net.*;
import java.util.*;
import java.util.zip.*;

import java.awt.*;
import java.awt.event.*;

public class ANSI_ClientDisplay extends Canvas implements AdjustmentListener {
    public int lineHeight;
    public int charWidth;
    public int offset;
    int numColumns = 80;
    int numRows = 24;

    public static Color[] colors = {
        Color.black, new Color(192, 0, 0), new Color(0, 192, 0), new Color(192, 192, 0),
        new Color(0, 0, 192), new Color(192, 0, 192), new Color(0, 192, 192), new Color(192, 192, 192),
        new Color(64, 64, 64), new Color(255, 64, 64), new Color(64, 255, 64), new Color(255, 255, 64),
        new Color(64, 64, 255), new Color(255, 64, 255), new Color(64, 255, 255), new Color(255, 255, 255)
    };

    private Socket inputSocket;
    private final static int MAX_LINES = 500;
    private ANSI_ClientString[] lines = new ANSI_ClientString[MAX_LINES];
    private int numLines = 0;
    private Scrollbar scrolly;
    private InputStreamHandler in;

    private Image offScrImg = null;
    private Graphics offScrGc = null;
    private Dimension dim = null;

    private char ANSI_buffer[] = new char[80];
    private char ANSI_bufferLength = 0;
    private boolean escFound = false;
    private boolean bracketFound = false;
    boolean reverse = false;
    int newColorIndex = 7;
    int newBackIndex = 0;

    private boolean foundIAC = false;
    private int termCount = 0;

    public final static char IS = 0;
    public final static char SEND = 1;
    public final static char NAWS = 31;
    public final static char TERM = 24;
    public final static char COMPRESS = 85;
    public final static char COMPRESS2 = 86;
    public final static char GA = 239;
    public final static char SE = 240;
    public final static char SB = 250;
    public final static char WILL = 251;
    public final static char WONT = 252;
    public final static char DO = 253;
    public final static char DONT = 254;
    public final static char IAC = 255;

    /** Means we're allowed to send window size each time it's changed.
     *  Set to true after the first IAC DO NAWS is received from the
     *  server. (I.e., after we know the server is willing to accept
     *  NAWS.)
     */
    private boolean enabledNAWS = false;

    private class InputStreamHandler extends Thread {
        private Socket sock;
        private InputStream in; 
	     private InflaterInputStream comp;
        private OutputStream out;
        private boolean closed = false;
        private boolean enabledCOMP = false;

        public InputStreamHandler(String host, int port) {
            try {
                sock = new Socket(host, port);
                in = sock.getInputStream();
                out = sock.getOutputStream();
                setDaemon(true);
                start();
            }
            catch (Exception e) {
                e.printStackTrace();
                add(e.toString());
                close();
                repaint();
            }
            
        }

        public synchronized void close() {
            closed = true;
            try {
                in.close();
            }
            catch (Exception e) {
            }
            try {
                out.close();
            }
            catch (Exception e) {
            }
            try {
                sock.close();
            }
            catch (Exception e) {
            }
            try {
            	comp.close();
	         }
            catch (Exception e) {
            }
        }
        
        public void compressedStream() {        	                      
			  enabledCOMP = true;
			  comp = new InflaterInputStream (in);
			}

        public void run() {
            String line;
            int c=0 ;
            int len;

            try {
					while (!closed)
					{
			            if (enabledCOMP) {
			            	if (comp.available() == 0) {
			 			   	   repaint();
							   }
								c = comp.read();								
							}				
						   else 
						   {
						   	if (in.available() == 0) {
							   	repaint();
								}
			                  c = in.read();
			            }
			            
	                 if (c == -1) {
	                        add("Connection lost.");
	                        break;
	                 }
	                 
	                 add((char)c);
	             }
				}
				   
		      catch (IOException e) {
               add("Connection lost.");
            }
            finally {
                close();
                repaint();
            }

        }

        public synchronized void send(byte s[]) {
            if (!closed) {
                try {
                    out.write(s);
                }
                catch (Exception e) {
                    e.printStackTrace();
                }
            }
            repaint();
        }

        public synchronized void send(byte s[], int length) {
            if (!closed) {
                try {
                    out.write(s, 0, length);
                }
                catch (Exception e) {
                    e.printStackTrace();
                }
            }
            repaint();
        }

        public synchronized void send(String s) {
            send(s.getBytes());
            repaint();
        }
    }
    
    public ANSI_ClientDisplay(String host, int port, Scrollbar myScrollBar) {
        this(myScrollBar);
        connect(host, port);
    }

    public ANSI_ClientDisplay(Scrollbar myScrollBar) {
        super();
        setBackground(Color.black);

        scrolly = myScrollBar;
        scrolly.addAdjustmentListener(this);

        add("Simple Java MUD Client v1.02, by Cristian Golumbovici (JagdTiger/Moraelin)");
    }

    public void setFont(Font f) {
        super.setFont(f);
        FontMetrics fm = getFontMetrics(f);
        charWidth = fm.charWidth('w');
        lineHeight = fm.getHeight();
        numColumns = getSize().width / charWidth; 
    }

    public synchronized void connect(String host, int port) {
        close();
        in = new InputStreamHandler(host, port);
    }

    public synchronized void close() {
        if (in != null) {
            in.close();
        }
    }

    public Dimension getPreferredSize() {
        return new Dimension(80 * charWidth, 24 * lineHeight);
    }

    public Dimension getMinimumSize() {
        return new Dimension(80 * charWidth, 24 * lineHeight);
    }

    public synchronized void addLine(ANSI_ClientString s) {
        if (numLines == MAX_LINES) {
            System.arraycopy(lines, 1, lines, 0, MAX_LINES - 1);
            lines[MAX_LINES - 1] = s;
            repaint();
        }
        else {
            lines[numLines++] = s;
            scrolly.setMaximum(numLines * lineHeight);
        }
        scrolly.setValue(Math.max(0, numLines * lineHeight - getSize().height));
    }

    public synchronized void appendChar(char c) {
        ANSI_ClientString line = lines[numLines - 1];
        line.append(c);
        if (line.visibleLength > numColumns) {
            int breakPos = line.lastIndexOf(' ', numColumns);
            if (breakPos > 0) {
                breakPos++;
                ANSI_ClientString newLine = line.substring(breakPos);
                lines[numLines - 1] = line.substring(0, breakPos);
                addLine(newLine);
            }
        }
    }

    public synchronized void add(String s) {
        if (numLines > 0) {
            if (lines[numLines - 1].length == 0) {
                numLines--;
            }
        }
        ANSI_ClientString s1 = new ANSI_ClientString("\uFF0B" + s);
        s1.isEchoText = true;
        addLine(s1);
        addLine(new ANSI_ClientString());
    }

    public synchronized void add(char c) {
        if (numLines == 0) {
            addLine(new ANSI_ClientString());
        }
        repaint();
        
        // here I deal with the telnet RFC's
        if (foundIAC) {
            if (ANSI_bufferLength == 0) {
                // twice IAC means I actually have received a printable character
                // 255, not a command
                if (c == IAC) {
                    appendChar(c);
                    foundIAC = false;
                }
                else if (c == DO || c == WILL || c == WONT || c == SB ||
                	c == COMPRESS2) {
                    ANSI_buffer[ANSI_bufferLength++] = c;
                }
                else {
                    foundIAC = false;
                }
            }
            else if (ANSI_bufferLength == 1) {
                if (ANSI_buffer[0] == WILL) {                    
                		if (c == COMPRESS2) {
		                  in.send ("" + IAC + DO + COMPRESS2);
		                  
	                 }
                
                    in.send("" + IAC + DONT + c);
                    foundIAC = false;
        
                }
                else if (ANSI_buffer[0] == DO) {

        					if (c == NAWS) {
                        in.send("" + IAC + WILL + NAWS);
                        in.send("" + IAC + SB + NAWS +
                             (char)0 + 
                             (char)numColumns +
                             (char)0 +
                             (char)numRows + 
                             IAC + SE);
                        enabledNAWS = true;
                    }
                    else if (c == TERM) {                   
                        in.send("" + IAC + WILL + TERM);
                        termCount = 0;
                    }
                    else {
                        in.send("" + IAC + WONT + c);
                    }
                    foundIAC = false;
                }
                else if (ANSI_buffer[0] != SB) {
                    foundIAC = false;
                }
                else {
                    ANSI_buffer[ANSI_bufferLength++] = c;      
                                  
                }
            }
			               
            else if (c == SE) {
                if (ANSI_bufferLength == 4 &&
                    ANSI_buffer[0] == SB &&
                    ANSI_buffer[1] == TERM &&
                    ANSI_buffer[2] == SEND &&
                    ANSI_buffer[3] == IAC) {

                    if (termCount == 0) {
                        in.send("" + IAC + SB + TERM + IS + "ANSI" + IAC + SE);
                        termCount = 1;
                    }
                    else if (termCount == 1) {
                        in.send("" + IAC + SB + TERM + IS + "VT100" + IAC + SE);
                        termCount = 2;
                    }
                    else {
                        in.send("" + IAC + SB + TERM + IS + "UNKNOWN" + IAC + SE);
                        termCount = 0;
                    }
                }

            	if (ANSI_bufferLength == 3 &&
                        ANSI_buffer[0] == SB &&
		                  ANSI_buffer[1] == COMPRESS2 &&
		                  ANSI_buffer[2] == IAC)
		                  {
		                  	in.compressedStream();
			               }

                foundIAC = false;
            }
            else if (ANSI_bufferLength < 80) {
                ANSI_buffer[ANSI_bufferLength++] = c;
            }
        }
        // and here starts ANSI/VT100 support
        else if (escFound) {
            escFound = false;
            if (c == '[') {
                bracketFound = true;
                ANSI_bufferLength = 0;
            }
            // basically, ESC Z means "are you a VT100?", and I say "yes"
            else if (c == 'Z') {
                send("\u001b[?1;0c");
            }
        }
        else if (bracketFound) {
            // some terminal querying. It's not entirely accurate, but
            // it should make DW recognize this as an ANSI terminal.
            // (Later: it doesn't. *SIGH*)
            if (c == 'n') {
                // request terminal status -- answer: no malfunction
                if (ANSI_bufferLength == 1 && ANSI_buffer[0] == '5') {
                    send("\u001b[0n");
                }
                // request cursor position -- answer: a bogus position
                else if (ANSI_bufferLength == 1 && ANSI_buffer[0] == '6') {
                    send("\u001b[24;0R");
                }
                else if (ANSI_bufferLength == 3 && 
                         ANSI_buffer[0] == '2' &&
                         ANSI_buffer[1] == '5' &&
                         ANSI_buffer[2] == '5') {
                    send("\u001b[" + (getSize().height / Math.max(1, lineHeight)) + ";80R");
                }
            }
            // color codes
            else if (c == 'm') {
                if (ANSI_bufferLength == 0) {
                    newColorIndex = 7;
                    newBackIndex = 0;
                    reverse = false;
                }
                else if (ANSI_bufferLength == 1) {
                    if (ANSI_buffer[0] == '1') {
                        newColorIndex |= 8;
                    }
                    else if (ANSI_buffer[0] == '2') {
                        newColorIndex &= 7;
                    }
                    else if (ANSI_buffer[0] == '7') {
                        reverse = true;
                    }
                    else if (ANSI_buffer[0] == '8') {
                        newColorIndex = newBackIndex;
                    }
                    else if (ANSI_buffer[0] == '0') {
                        //newColorIndex &= 7;
                        newColorIndex = 7;
                        newBackIndex = 0;
                        reverse = false;
                    }
                }
                else if (ANSI_bufferLength == 2 && ANSI_buffer[0] == '3') {
                    int col = ANSI_buffer[1] - '0';
                    if (col > 7) {
                        col = 7;
                    }
                    newColorIndex = (newColorIndex & 8) | col;
                }
                else if (ANSI_bufferLength == 2 && ANSI_buffer[0] == '4') {
                    newBackIndex = ANSI_buffer[1] - '0';
                    if (newBackIndex > 7) {
                        newBackIndex = 0;
                    }
                }
                bracketFound = false;
                ANSI_ClientString.currentColorIndex = newColorIndex + 16 * newBackIndex + (reverse ? 128 : 0);
                add((char)('\uFF00' + ANSI_ClientString.currentColorIndex));
            }
            else if (c == ';') {
                if (ANSI_bufferLength == 1) {
                    if (ANSI_buffer[0] == '1') {
                        newColorIndex |= 8;
                    }
                    else if (ANSI_buffer[0] == '2') {
                        newColorIndex &= 7;
                    }
                    else if (ANSI_buffer[0] == '7') {
                        reverse = true;
                    }
                    else if (ANSI_buffer[0] == '8') {
                        newColorIndex = newBackIndex;
                    }
                    else if (ANSI_buffer[0] == '0') {
                        //newColorIndex &= 7;
                        newColorIndex = 7;
                        newBackIndex = 0;
                        reverse = false;
                    }
                }
                else if (ANSI_bufferLength == 2 && ANSI_buffer[0] == '3') {
                    int col = ANSI_buffer[1] - '0';
                    if (col > 7) {
                        col = 7;
                    }
                    newColorIndex = (newColorIndex & 8) | col;
                }
                else if (ANSI_bufferLength == 2 && ANSI_buffer[0] == '4') {
                    newBackIndex = ANSI_buffer[1] - '0';
                    if (newBackIndex > 7) {
                        newBackIndex = 0;
                    }
                }
                ANSI_bufferLength = 0;
            }
            else if (c >= '0' && c <= '9') {
                if (ANSI_bufferLength < 80) {
                    ANSI_buffer[ANSI_bufferLength++] = c;
                }
            }
            else {
                bracketFound = false;
            }
        }
        else if (c == '\u001B') {
            escFound = true;
            newColorIndex = ANSI_ClientString.currentColorIndex & 15;
            newBackIndex = (ANSI_ClientString.currentColorIndex >> 4) & 7;
            reverse = (ANSI_ClientString.currentColorIndex & 128) != 0;
        }
        else if (c == '\n') {
            addLine(new ANSI_ClientString());
        }
        else if (c < ' ' && c != '\u001b') {
        }
        else if (c == IAC) {
            ANSI_bufferLength = 0;
            foundIAC = true;
        }
        else {
            appendChar(c);
        }
    }

    public void update(Graphics g) {
        paint(g);
    }

    public synchronized void paint(Graphics g) {
        Dimension d = getSize();
        if (dim == null ||
            offScrGc == null ||
            dim.width != d.width ||
            dim.height != d.height) {

            offScrImg = createImage(d.width, d.height);
            offScrGc = offScrImg.getGraphics();
        }

        Font f = getFont();
        offScrGc.setFont(f);
        FontMetrics fm = getFontMetrics(f);
        lineHeight = fm.getHeight();
        charWidth = fm.charWidth('w');
        int prevRows = numRows;
        int prevColumns = numColumns;
        numColumns = Math.min(127, d.width / charWidth);
        numRows = Math.min(127, d.height / charWidth);

        if (enabledNAWS && (numColumns != prevColumns || numRows != prevRows)) {
            // screen size has chaged, and the server requested NAWS.
            // (Negotiate About Window Size)
            in.send("" + IAC + SB + NAWS +
                    (char)0 +
                    (char)numColumns+
                    (char)0 +
                    (char)numRows +
                    IAC + SE);
        }

        scrolly.setMaximum(numLines * lineHeight);
        scrolly.setVisibleAmount(d.height);
        scrolly.setBlockIncrement(Math.max(lineHeight, d.height - lineHeight));
        scrolly.setUnitIncrement(lineHeight);

        offScrGc.setColor(Color.black);
        offScrGc.fillRect(0, 0, d.width, d.height);

        offset = scrolly.getValue();
        int ascent = offScrGc.getFontMetrics(f).getAscent();
        int i = Math.max(0, offset / lineHeight);
        int y = ascent - offset + i * lineHeight;
        for (; i < numLines; i ++, y += lineHeight) {
            if (y < 0) {
                continue;
            }
            drawANSI_ClientString(offScrGc, lines[i], y - ascent, y);
        }
        g.drawImage(offScrImg, 0, 0, d.width, d.height, null);
    }

    private void drawANSI_ClientString(Graphics g, ANSI_ClientString ms, int y, int baselineY) {
        int colorIndex = ms.colorIndex & 15;
        int backIndex = (ms.colorIndex >> 4) & 8;
        boolean reverse = (ms.colorIndex & 128) == 128;
        int x = 0;
        int prevIndex = 0;

        for (int i = 0; i < ms.length; i++) {
            if ((ms.buf[i] & '\uFF00') == '\uFF00') {
                if (backIndex != 0 || reverse) {
                    g.setColor(colors[reverse ? colorIndex : backIndex]);
                    g.fillRect(x, y, (i - prevIndex) * charWidth, lineHeight);
                }
                g.setColor(colors[reverse ? backIndex : colorIndex]);
                g.drawChars(ms.buf, prevIndex, i - prevIndex, x, baselineY);
                x += (i - prevIndex) * charWidth;
                prevIndex = i + 1;

                colorIndex = ms.buf[i] & 15;
                backIndex = (ms.buf[i] >> 4) & 8;
                reverse = (ms.buf[i] & 128) == 128;
            }
        }
        if (ms.length > prevIndex) {
            if (backIndex != 0 || reverse) {
                g.setColor(colors[reverse ? colorIndex : backIndex]);
                g.fillRect(x, y, (ms.length - prevIndex) * charWidth, lineHeight);
            }
            g.setColor(colors[reverse ? backIndex : colorIndex]);
            g.drawChars(ms.buf, prevIndex, ms.length - prevIndex, x, baselineY);
        }

        /*
        if (ms.isEchoText) {
            return ms.colorIndex;
        }
        return colorIndex + (backIndex << 4) + (reverse ? 128 : 0);
        */
    }

    // basically, here I take care of doubling any instance of character 255,
    // as required by RFC 854 (telnet protocol)
    public synchronized void send(char src[]) {
        if (in == null) {
            return;
        }
        // since worst case is having all 255's, i.e., all doubled,
        // twice the original number of chars is guaranteed to be enough.
        byte dst[] = new byte[2 * src.length + 1];
        int dstIndex = 0;
        for (int i = 0; i < src.length; i++) {
            char c = src[i];
            if (c > 255) {
                c = ' ';
            }
            dst[dstIndex++] = (byte)c;
            if (c == 255) {
                dst[dstIndex++] = (byte)255;
            }
        }
        dst[dstIndex++] = (byte)'\n';
        in.send(dst, dstIndex);
        repaint();
    }

    public synchronized void send(String s) {
        add(s);
        send(s.toCharArray());
        repaint();
    }

    public void adjustmentValueChanged(AdjustmentEvent e) {
        int newOffset = scrolly.getValue();
        if (offset != newOffset) {
            repaint();
        }
    }

    public synchronized void pgUp() {
        scrolly.setValue(Math.max(0, scrolly.getValue() - getSize().height));
        repaint();
    }

    public synchronized void pgDown() {
        scrolly.setValue(Math.max(0, Math.min(numLines * lineHeight - getSize().height, scrolly.getValue() + getSize().height)));
        repaint();
    }
}