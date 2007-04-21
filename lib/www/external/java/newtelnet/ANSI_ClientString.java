public class ANSI_ClientString {
    public char[] buf;
    public int length;
    public int visibleLength;
    public int colorIndex;
    /** Own echoed text doesn't change the color following text */
    public boolean isEchoText = false;

    /** The colour used to create new strings */
    public static int currentColorIndex = 7;

    public ANSI_ClientString() {
        buf = new char[128];
        length = 0;
        visibleLength = 0;
        colorIndex = currentColorIndex;
    }

    public ANSI_ClientString(String s) {
        buf = s.toCharArray();
        length = buf.length;
        recomputeVisibleLength();
    }

    public ANSI_ClientString(char src[], int startPos, int newLength, int newColorIndex) {
        buf = new char[Math.max(newLength, 128)];
        System.arraycopy(src, startPos, buf, 0, newLength);
        length = newLength;
        colorIndex = newColorIndex;
        recomputeVisibleLength();
    }

    public void append(char c) {
        if (length >= buf.length) {
            char[] newBuf = new char[length + 64];
            System.arraycopy(buf, 0, newBuf, 0, buf.length);
            buf = newBuf;
        }
        buf[length++] = c;
        // if it's not a colour attribute, increase the number of visible
        // characters
        if ((c & '\uFF00') != '\uFF00') {
            visibleLength ++;
        }
    }

    public int recomputeVisibleLength() {
        visibleLength = 0;
        for (int i = 0; i < length; i++) {
            if ((buf[i] & '\uFF00') != '\uFF00') {
                visibleLength ++;
            }
        }
        return visibleLength;
    }

    public int lastIndexOf(char c, int beforeVisiblePosition) {
        int foundPos = -1;
        int visibleSoFar = 0;
        for (int i = 0; i < length; i++) {
            if ((buf[i] & '\uFF00') != '\uFF00') {
                visibleSoFar ++;
            }
            if (visibleSoFar >= beforeVisiblePosition) {
                break;
            }
            if (buf[i] == c) {
                foundPos = i;
            }
        }
        return foundPos;
    }

    public ANSI_ClientString substring(int startPos, int newLength) {
        int newColorIndex = colorIndex;
        for (int i = 0; i < startPos; i++) {
            if ((buf[i] & '\uFF00') == '\uFF00') {
                newColorIndex = (buf[i] & 0xFF);
            }
        }
        return new ANSI_ClientString(buf, startPos, newLength, newColorIndex);
    }

    public ANSI_ClientString substring(int startPos) {
        return substring(startPos, length - startPos);            
    }
}
