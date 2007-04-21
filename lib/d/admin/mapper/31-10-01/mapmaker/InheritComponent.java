package mapmaker;

public interface InheritComponent
{
    public void setValues(String aInheritedValue, String aValue, boolean aInherited);

    public String getInheritedValue();

    public void setInherited(boolean aInherited);

    public boolean isInherited();

    public void setText(String aText);

    public String getText();
}