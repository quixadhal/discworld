package mapmaker;

import java.awt.*;
import java.awt.geom.*;
import java.awt.font.*;
import java.util.*;
import java.text.*;
import java.io.*;

public class MapLocation extends RoomProperties implements Serializable
{
    public final static int LINK_UP_SIZE = 10;
    public final static int HANDLE_SIZE = 10;
    public final static int SELECTION_MARK_SIZE = 2;

    // The position.
    int mX;
    int mY;

    // For translations.
    int mOldX;
    int mOldY;

    // For undo/redo stuff.
    int mTempX;
    int mTempY;

    // Exit stuff.
    Vector mExits = new Vector();

    // Zone stuff.
    Vector mZones = new Vector();

    // Vector stuff.
    Vector mLinkers = new Vector();

    public MapLocation(int aX, int aY)
    {
        mAutoNumber = incrementAutoNumber();

        mX = aX;
        mY = aY;

        mOldX = aX;
        mOldY = aY;

        mTempX = aX;
        mTempY = aY;
    }

    public MapLocation(RoomProperties aRoomProperties, int aX, int aY)
    {
        setInheritedRoomProperties(aRoomProperties);
/*
        // TODO:
        try
        {
            Integer size = (Integer)aRoomProperties.getProperty("size", false);
            setRoomSize(size.intValue());
        }
        catch (Exception e)
        {
            e.printStackTrace();
        }
*/

        mAutoNumber = incrementAutoNumber();

        mX = aX;
        mY = aY;

        mOldX = aX;
        mOldY = aY;

        mTempX = aX;
        mTempY = aY;
    }

    public int getRoomSize()
    {
        String size = null;

        try
        {
            size = (String)mInheritedRoomProperties.getProperty("size",
                RoomProperties.INHERIT_DO_CHECK);
        }
        catch (Exception e)
        {
            e.printStackTrace();
        }

        return Integer.parseInt(size);
    }


    //==========================================================================
    //  Positioning stuff.
    //==========================================================================

    public int getX()
    {
        return mX;
    }

    public int getY()
    {
        return mY;
    }

    public void setX(int aX)
    {
        mX = aX;
    }

    public void setY(int aY)
    {
        mY = aY;
    }

    //==========================================================================
    //  Linker stuff.
    //==========================================================================

    public boolean isInLinker(Linker aLinker)
    {
        if (mLinkers.contains(aLinker))
        {
            return true;
        }
        return false;
    }

    public void addLinker(Linker aLinker)
    {
        if (!mLinkers.contains(aLinker))
        {
            mLinkers.addElement(aLinker);
        }
    }

    public void removeLinker(Linker aLinker)
    {
        mLinkers.removeElement(aLinker);
    }

    //==========================================================================
    //  Zone stuff.
    //==========================================================================

    public boolean isInZone(Zone aZone)
    {
        if (mZones.contains(aZone))
        {
            return true;
        }
        return false;
    }

    public void addZone(Zone aZone)
    {
        if (!mZones.contains(aZone))
        {
            mZones.addElement(aZone);
        }
    }

    public void removeZone(Zone aZone)
    {
        mZones.removeElement(aZone);
    }

    //==========================================================================
    //  Exit stuff.
    //==========================================================================

    public void addExit(Exit aExit)
    {
        mExits.addElement(aExit);
    }

    public void removeExit(Exit aExit)
    {
        mExits.removeElement(aExit);
    }

    public Exit getOpposite(Exit aFrom)
    {
        for (Enumeration e = mExits.elements(); e.hasMoreElements();)
        {
            Exit exit = (Exit)e.nextElement();

            if (exit.getTo() == aFrom.getFrom())
            {
                return exit;
            }
        }

        return null;
    }

    public Exit getExit(int aDir)
    {
        for (Enumeration e = mExits.elements(); e.hasMoreElements();)
        {
            Exit exit = (Exit)e.nextElement();

            int direction = exit.getDirection();

            if (exit.getDirection() == aDir)
            {
                return exit;
            }
        }

        return null;
    }

    public boolean isExit(int aDir)
    {
        return getExit(aDir) != null;
    }

    public void setExits(Vector aExits)
    {
        mExits = aExits;
    }

    public Vector getExits()
    {
        return mExits;
    }

    public void clearExits()
    {
        mExits = new Vector();
    }

    //==========================================================================
    //  Painting the location.
    //==========================================================================

    public Rectangle getRectangle()
    {
        int roomSize = getRoomSize();
        return new Rectangle(mX - roomSize, mY - roomSize, roomSize * 2, roomSize * 2);
    }

    public Rectangle getZoneRectangle()
    {
        int roomSize = getRoomSize();
        return new Rectangle(mX - roomSize * 2, mY - roomSize * 2,
            roomSize * 4, roomSize * 4);
    }

    public Rectangle getLinkerRectangle()
    {
        int roomSize = getRoomSize();

        Point[] cardinals = getExitPoints();

        Point topLeft = (Point)cardinals[Direction.NW].clone();
        topLeft.translate(-3, -3);

        Rectangle roomShape = new Rectangle(topLeft,
            new Dimension(cardinals[Direction.SE].x - cardinals[Direction.NW].x + 6,
                          cardinals[Direction.SE].y - cardinals[Direction.NW].y + 6));

        return roomShape;
    }

    // This method returns the area around the location to be checked for
    // link ups.
    public Rectangle getLinkUpRectangle()
    {
        int roomSize = getRoomSize();

        return new Rectangle(mX - roomSize - LINK_UP_SIZE,
            mY - roomSize - LINK_UP_SIZE,
            roomSize * 2 + LINK_UP_SIZE,
            roomSize * 2 + LINK_UP_SIZE);
    }

    public Point[] getCardinals()
    {
        int roomSize = getRoomSize();

        return getCardinalsFor(roomSize, roomSize);
    }

    public Point[] getExitPoints()
    {
        int roomSize = getRoomSize();

        return getCardinalsFor((roomSize / 3) * 2, (roomSize / 3) * 2);
    }

    public Point[] getCardinalsFor(int aHoriz, int aVert)
    {
        Point[] cardinals = new Point[8];

        int count = 0;

        for (int i = (mX - aHoriz); i < mX + (2 * aHoriz); i += aHoriz)
        {
            for (int j = (mY - aVert); j < mY + (2 * aVert); j += aVert)
            {
                if (i == mX && j == mY)
                {
                    continue;
                }

                cardinals[count++] = new Point(i, j);
            }
        }

        return cardinals;
    }

    public Rectangle[] getHandles()
    {
        Rectangle[] handles = new Rectangle[8];

        Point[] cardinals = getCardinals();
        Dimension handleSize = new Dimension(HANDLE_SIZE, HANDLE_SIZE);

        for (int i = 0; i < 8; i++)
        {
            cardinals[i].translate(-(HANDLE_SIZE / 2), -(HANDLE_SIZE / 2));

            handles[i] = new Rectangle(cardinals[i], handleSize);
        }

        return handles;
    }

    public Rectangle[] getSelectionMarks()
    {
        Rectangle[] handles = new Rectangle[8];
        Point[] cardinals = getCardinals();
        Dimension handleSize = new Dimension(SELECTION_MARK_SIZE, SELECTION_MARK_SIZE);

        for (int i = 0; i < 8; i++)
        {
            cardinals[i].translate(-(SELECTION_MARK_SIZE / 2), -(SELECTION_MARK_SIZE / 2));

            handles[i] = new Rectangle(cardinals[i], handleSize);
        }

        return handles;
    }

    public void renderBackground(Graphics2D g)
    {
        int roomSize = getRoomSize();

        Point[] cardinals = getExitPoints();

        Point topLeft = (Point)cardinals[Direction.NW].clone();
        topLeft.translate(-1, -1);

        Rectangle roomShape = new Rectangle(topLeft,
            new Dimension(cardinals[Direction.SE].x - cardinals[Direction.NW].x + 2,
                          cardinals[Direction.SE].y - cardinals[Direction.NW].y + 2));

        for (int i = 0; i < 8; i++)
        {
            if (isExit(i))
            {
                Exit exit = getExit(i);

                //if (i == Direction.SW || i == Direction.NE)
                //{
                    g.setStroke(new BasicStroke(roomSize / 5 + 2));
                //}
                //else
                //{
                //    g.setStroke(new BasicStroke(roomSize / 5 + 1));
                //}

                g.setPaint(getForegroundColor());

                MapLocation destination = exit.getTo();

                if (!exit.isDummy())
                {
                    // Don't draw if other room is smaller.
                    if (destination.getRoomSize() >= roomSize)
                    {
                        // Draw exit to other location.
                        g.draw(new Line2D.Double(new Point(mX, mY),
                            new Point(destination.getX(), destination.getY())));
                    }
                }
                else
                {
                    // Draw exit to nowhere.
                    Shape defaultClip = g.getClip();
                    g.setClip(this.getRectangle());
                    g.setPaint(getForegroundColor());
                    g.draw(new Line2D.Double(new Point(mX, mY), this.getCardinals()[i]));
                    g.setClip(defaultClip);
                }
            }
        }

        g.setPaint(getForegroundColor());
        g.fill(roomShape);
    }

    int getDoorAngle(int aDir)
    {
        int angle = 0;

        switch (aDir)
        {
            case Direction.N :
                angle = 180;
                break;
            case Direction.NE :
                angle = 225;
                break;
            case Direction.E :
                angle = 270;
                break;
            case Direction.SE :
                angle = 315;
                break;
            case Direction.S :
                angle = 0;
                break;
            case Direction.SW :
                angle = 45;
                break;
            case Direction.W :
                angle = 90;
                break;
            case Direction.NW :
                angle = 135;
                break;
        }

        return angle;
    }

    public void renderForeground(Graphics2D g)
    {
        int roomSize = getRoomSize();
        Point[] cardinals = getExitPoints();
        Rectangle roomShape = new Rectangle(cardinals[Direction.NW],
            new Dimension(cardinals[Direction.SE].x - cardinals[Direction.NW].x,
                          cardinals[Direction.SE].y - cardinals[Direction.NW].y));

        g.setStroke(new BasicStroke(roomSize / 5));
        g.setPaint(getBackgroundColor());
        g.fill(roomShape);

        for (int i = 0; i < 8; i++)
        {
            if (isExit(i))
            {
                Exit exit = getExit(i);
                MapLocation destination = exit.getTo();

                if (!exit.isDummy())
                {
                    // Don't draw if other room is smaller.
                    if (destination.getRoomSize() >= roomSize)
                    {
                         // Draw exit to other location.
                        Point ourPoint = cardinals[i];
                        Point theirPoint = destination.getExitPoints()[Direction.getOppoisite(i)];

                        g.setPaint(new GradientPaint(ourPoint, this.getBackgroundColor(),
                            theirPoint, destination.getBackgroundColor()));

                        if (i == Direction.SW || i == Direction.NE)
                        {
                            g.setStroke(new BasicStroke(roomSize / 5));
                        }
                        else
                        {
                            g.setStroke(new BasicStroke(roomSize / 5));
                        }

                        // Draw exit to other location.
                        g.draw(new Line2D.Double(new Point(mX, mY),
                            new Point(destination.getX(), destination.getY())));

                        // Indicate special exit types...
                        String ourType = exit.getType();
                        String theirType = exit.getOpposite().getType();

                        double doorWidth = ((roomSize / 5) + 1) * 2;
                        double doorHeight = doorWidth / 2;

                        // Draw a one-way door.
                        if (ourType.equals("door") && !theirType.equals("door"))
                        {
                            Point center = getCardinals()[i];
                            Point topLeft = (Point)center.clone();
                            topLeft.translate(-(int)doorHeight, -(int)doorHeight);

                            Area door = new Area(new Rectangle2D.Double(
                                topLeft.getX(), topLeft.getY(),
                                doorWidth, doorHeight));
                            door.transform(AffineTransform.getRotateInstance(
                                Math.toRadians(getDoorAngle(i)), center.x, center.y));

                            g.setPaint(Color.black);
                            g.fill(door);
                        }
                        else if (ourType.equals("door") && theirType.equals("door"))
                        {
                            Point center = getCardinals()[i];
                            Point topLeft = (Point)center.clone();
                            topLeft.translate(-(int)doorHeight, -(int)doorHeight);

                            Area door = new Area(new Rectangle2D.Double(
                                topLeft.getX(), topLeft.getY(),
                                doorWidth, doorWidth));
                            door.transform(AffineTransform.getRotateInstance(
                                Math.toRadians(getDoorAngle(i)), center.x, center.y));

                            g.setPaint(Color.black);
                            g.fill(door);
                        }

                        else if (ourType.equals("hidden") && theirType.equals("hidden"))
                        {
                            Point center = getCardinals()[i];
                            Point topLeft = (Point)center.clone();
                            topLeft.translate(-(int)doorHeight, -(int)doorHeight);

                            g.setPaint(Color.black);

                            g.transform(AffineTransform.getRotateInstance(
                                Math.toRadians(getDoorAngle(i)),
                                center.x, center.y));

                            AttributedString str = new AttributedString("S");
                            str.addAttribute(TextAttribute.SIZE, new Float(14f));
                            //str.addAttribute(TextAttribute.WEIGHT, TextAttribute.WEIGHT_BOLD);
                            g.drawString(str.getIterator(), center.x, center.y);

                            g.transform(AffineTransform.getRotateInstance(
                                Math.toRadians(360 - getDoorAngle(i)),
                                center.x, center.y));
                            /*
                            Area door = new Area(new Rectangle2D.Double(
                                topLeft.getX(), topLeft.getY(),
                                doorWidth, doorWidth));
                            door.transform(AffineTransform.getRotateInstance(
                                Math.toRadians(getDoorAngle(i)), center.x, center.y));

                            g.setPaint(Color.black);
                            g.fill(door);
                            */
                        }

                    }
                }
                else
                {
                    Shape defaultClip = g.getClip();
                    g.setClip(this.getRectangle());
                    g.setPaint(new GradientPaint(cardinals[i], this.getBackgroundColor(),
                        getCardinals()[i], Color.white));
                    g.draw(new Line2D.Double(new Point(mX, mY), this.getCardinals()[i]));
                    g.setClip(defaultClip);
                }
            }
        }
    }
}
