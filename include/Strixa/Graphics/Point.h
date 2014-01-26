#ifndef Strixa_Graphics_Point_H
#define Strixa_Graphics_Point_H

namespace Strixa
{
    namespace Graphics
    {
        class Point
        {
            /* Instance Properties */
            public:
                long  raw[3];
                long& x;
                long& y;
                long& z;

            /* Instance Methods */
            public:
                Point(long x = 0,long y = 0,long z = 0);

                Point(const Point& copy);

                bool operator==(const Point& comp) const;

                bool operator!=(const Point& comp) const;

                Point& operator=(const Point& copy);
        };
    }
}

#endif