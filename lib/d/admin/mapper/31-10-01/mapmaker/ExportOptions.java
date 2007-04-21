package mapmaker;

/**
 * This class stores the various export options that users have selected.
 */
public class ExportOptions
{
    public static final int BRACE_END_OF_LINE = 0;
    public static final int BRACE_NEXT_LINE = 1;

    int mCols;
    int mIndent;
    int mBraceType;
    String mDirectory;

    public void setDirectory(String aDirectory)
    {
        mDirectory = aDirectory;
    }

    public String getDirectory()
    {
        return mDirectory;
    }

    public void setCols(int aCols)
    {
        mCols = aCols;
    }

    public int getCols()
    {
        return mCols;
    }

    public void setIndent(int aIndent)
    {
        mIndent = aIndent;
    }

    public int getIndent()
    {
        return mIndent;
    }

    public void setBraceType(int aBraceType)
    {
        mBraceType = aBraceType;
    }

    public int getBraceType()
    {
        return mBraceType;
    }
}
