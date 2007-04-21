package mapmaker;

import java.awt.*;

//==========================================================================
// Class used to record potential link ups between map locations.
//==========================================================================

class LinkUp
{
    // The location being moved (or added).
    public MapLocation mSelectedLocation;

    // The location we want to link up with.
    public MapLocation mUnselectedLocation;

    // The 'direction' from the selected location to the unselected
    // location.
    public int mDirection;

    // The translation needed to be applied to the selected location to
    // link up the two locations.
    public Point mTranslation;

    public int mSelectedX;
    public int mSelectedY;

    public LinkUp(MapLocation aSelected, MapLocation aUnselected, int aDir)
    {
        mSelectedLocation = aSelected;
        mUnselectedLocation = aUnselected;
        mDirection = aDir;

        mSelectedX = aSelected.getX();
        mSelectedY = aSelected.getY();

        // Find the coordinates of the exit on the selected location.
        Point selectedPoint = mSelectedLocation.getCardinals()[mDirection];

        // Find the coordinates of the exit on the unselected location.
        Point unselectedPoint = mUnselectedLocation.getCardinals()
              [Direction.getOppoisite(mDirection)];

        // Work out the translation that needs to be applied to link up
        // the locations.
        mTranslation = new Point(selectedPoint.x - unselectedPoint.x,
                     selectedPoint.y - unselectedPoint.y);
    }

    public String toString()
    {
        return mTranslation.toString() + ": " + mSelectedLocation.toString()
               + ", " + mUnselectedLocation.toString();
    }
}