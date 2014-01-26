#ifndef Strixa_Graphics_Dimensions_H
#define Strixa_Graphics_Dimensions_H

namespace Strixa
{
    namespace Graphics
    {
        class Dimensions
        {
            /* Instance Properties */
            public:
                long  raw[3];
                long& depth;
                long& height;
                long& width;

            /* Instance Methods */
            public:
                Dimensions(long width = 0,long height = 0,long depth = 0);

                Dimensions(const Dimensions& copy);

                bool operator==(const Dimensions& comp) const;

                bool operator!=(const Dimensions& comp) const;

                Dimensions& operator=(const Dimensions& copy);
        };
    }
}

#endif