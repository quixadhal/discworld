package mapmaker;

public class NoSuchRoomPropertyException extends Exception
{
    public NoSuchRoomPropertyException()
    {
        super();
    }

    public NoSuchRoomPropertyException(String aString)
    {
        super(aString);
    }
}