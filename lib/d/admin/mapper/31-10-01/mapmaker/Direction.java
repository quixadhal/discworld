package mapmaker;

public class Direction
{
    public final static int NW = 0;
    public final static int W = 1;
    public final static int SW = 2;
    public final static int N = 3;
    public final static int S = 4;
    public final static int NE = 5;
    public final static int E = 6;
    public final static int SE = 7;
    public final static int UP = 8;
    public final static int DOWN = 9;

    public final static int ORDER_N = 0;
    public final static int ORDER_S = 1;
    public final static int ORDER_E = 2;
    public final static int ORDER_W = 3;
    public final static int ORDER_NE = 4;
    public final static int ORDER_SE = 5;
    public final static int ORDER_SW = 6;
    public final static int ORDER_NW = 7;
    public final static int ORDER_UP = 8;
    public final static int ORDER_DOWN = 9;
    public final static int ORDER_UNKNOWN = 10;

    public static int getOppoisite(int aDir)
    {
        int opposite = -1;

        switch (aDir)
        {
            case NW :
                 opposite = SE;
                 break;
            case W :
                 opposite = E;
                 break;
            case SW :
                 opposite = NE;
                 break;
            case N :
                 opposite = S;
                 break;
            case S :
                 opposite = N;
                 break;
            case NE :
                 opposite = SW;
                 break;
            case E :
                 opposite = W;
                 break;
            case SE :
                 opposite = NW;
                 break;
            case UP :
                 opposite = DOWN;
                 break;
            case DOWN :
                 opposite = UP;
                 break;
        }
        return opposite;
    }

    public static int getExitOrder(int aDir)
    {
        int order = -1;

        switch (aDir)
        {
            case NW :
                 order = ORDER_NW;
                 break;
            case W :
                 order = ORDER_W;
                 break;
            case SW :
                 order = ORDER_SW;
                 break;
            case N :
                 order = ORDER_N;
                 break;
            case S :
                 order = ORDER_S;
                 break;
            case NE :
                 order = ORDER_NE;
                 break;
            case E :
                 order = ORDER_E;
                 break;
            case SE :
                 order = ORDER_SE;
                 break;
            case UP :
                 order = ORDER_UP;
                 break;
            case DOWN :
                 order = ORDER_DOWN;
                 break;
            default :
                 order = ORDER_UNKNOWN;
                 break;
        }
        return order;
    }

    public static String getDirectionName(int aDir)
    {
        String name = null;

        switch (aDir)
        {
            case NW :
                 name = "northwest";
                 break;
            case W :
                 name = "west";
                 break;
            case SW :
                 name = "southwest";
                 break;
            case N :
                 name = "north";
                 break;
            case S :
                 name = "south";
                 break;
            case NE :
                 name = "northeast";
                 break;
            case E :
                 name = "east";
                 break;
            case SE :
                 name = "southeast";
                 break;
            case UP :
                 name = "down";
                 break;
            case DOWN :
                 name = "up";
                 break;
        }
        return name;
    }
}