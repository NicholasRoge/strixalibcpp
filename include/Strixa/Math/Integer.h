#ifndef Strixa_Math_Integer
#define Strixa_Math_Integer

#include <iostream>

typedef char byte;
typedef signed char sbyte;
typedef unsigned char ubyte;

namespace strixa
{
    namespace math
    {
        class Integer
        {
            /* Member Properties */
            private:
                sbyte* msb;
                size_t size;
                ubyte* value;

            /* Member Methods */
            private:
                void resize(size_t new_size);

            public:
                Integer(long long initial_value = 0);

                Integer(const Integer& copy);

                Integer(Integer&& move);

                ~Integer();

                Integer& operator=(const Integer& value);

                Integer& operator=(Integer&& value);

                Integer operator+(Integer value) const;

                Integer& operator+=(const Integer& value);

                Integer& operator++();

                Integer operator-() const;

                Integer operator-(const Integer& value) const;

                Integer& operator-=(const Integer& value);

                Integer& operator--();

                Integer operator*(Integer value) const;

                Integer& operator*=(const Integer& value);

                Integer operator/(Integer value) const;

                Integer& operator/=(Integer value);

                Integer operator%(Integer value) const;

                Integer& operator%=(const Integer& value);

                Integer operator~() const;

                bool operator<(const Integer& value) const;

                bool operator<=(const Integer& value) const;

                bool operator==(const Integer& value) const;

                bool operator>=(const Integer& value) const;

                bool operator>(const Integer& value) const;

                friend std::ostream& operator<<(std::ostream& stream,Integer integer);
        };
    }
}

#endif