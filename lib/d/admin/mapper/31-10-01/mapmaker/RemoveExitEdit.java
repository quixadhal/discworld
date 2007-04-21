package mapmaker;

import javax.swing.undo.*;

class RemoveExitEdit extends AbstractUndoableEdit
{
    private MapLocation mLocation;
    private Exit mExit;

    public RemoveExitEdit(MapLocation aLocation, Exit aExit)
    {
        mLocation = aLocation;
        mExit = aExit;
    }

    public void undo()
    {
        mLocation.addExit(mExit);
    }

    public void redo()
    {
        mLocation.removeExit(mExit);
    }

    public String getUndoPresentationName()
    {
        return "Undo Remove Exit";
    }

    public String getRedoPresentationName()
    {
        return "Redo Remove Exit";
    }
}
