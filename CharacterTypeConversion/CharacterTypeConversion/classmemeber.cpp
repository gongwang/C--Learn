#include <iostream.h>

class point
{
public:
    int x;
    int y;
    void intput(int x ,int y)
    {
        x = x;
        y = y;
    }
    void output()
    {
        cout<<x<<endl<<y<<endl;
    }
protected:

private:

};

void main()
{
    point pt;
    pt.intput(5,10);
    pt.output();
}