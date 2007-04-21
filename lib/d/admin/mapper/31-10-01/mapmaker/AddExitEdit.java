package mapmaker;

import javax.swing.undo.*;

public class AddExitEdit extends AbstractUndoableEdit
{
    private MapLocation mLocation;
    private Exit mExit;

    public AddExitEdit(MapLocation aLocation, Exit aExit)
    {
        mLocation = aLocation;
        mExit = aExit;
    }

    public void undo()
    {
        mLocation.removeExit(mExit);
    }

    public void redo()
    {
        mLocation.addExit(mExit);
    }

    public String getUndoPresentationName()
    {
        return "Undo Add Exit";
    }

    public String getRedoPresentationName()
    {
        return "Redo Add Exit";
    }
}