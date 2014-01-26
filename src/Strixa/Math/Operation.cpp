#include <limits>
#include <Strixa/Math/Operation.h>

using namespace Strixa::Math;


/* Function Definitions */
#pragma region Math Function Definitions
Float sin(const Float& radians)
{
    return sin((long double)radians);
}

Float cos(const Float& radians)
{
    return cos((long double)radians);
}

Float tan(const Float& radians)
{
    return tan((long double)radians);
}
#pragma endregion

#pragma region Operator Definitions
#pragma region Operator +
Integer operator+(const Integer& left_operand,const Integer& right_operand)
{
    return (long long)left_operand + (long long)right_operand;
}

Float operator+(const Float& left_operand,const Integer& right_operand)
{
    return (long double)left_operand + (long double)right_operand;
}

Float operator+(const Integer& left_operand,const Float& right_operand)
{
    return (long double)left_operand + (long double)right_operand;
}

Float operator+(const Float& left_operand,const Float& right_operand)
{
    return (long double)left_operand + (long double)right_operand;
}
#pragma endregion

#pragma region Operator -
Integer operator-(const Integer& left_operand,const Integer& right_operand)
{
    return (long long)left_operand - (long long)right_operand;
}

Float operator-(const Float& left_operand,const Integer& right_operand)
{
    return (long double)left_operand - (long double)right_operand;
}

Float operator-(const Integer& left_operand,const Float& right_operand)
{
    return (long double)left_operand - (long double)right_operand;
}

Float operator-(const Float& left_operand,const Float& right_operand)
{
    return (long double)left_operand - (long double)right_operand;
}
#pragma endregion

#pragma region Operator *
Integer operator*(const Integer& left_operand,const Integer& right_operand)
{
    return (long long)left_operand * (long long)right_operand;
}

Float operator*(const Float& left_operand,const Integer& right_operand)
{
    return (long double)left_operand * (long double)right_operand;
}

Float operator*(const Integer& left_operand,const Float& right_operand)
{
    return (long double)left_operand * (long double)right_operand;
}

Float operator*(const Float& left_operand,const Float& right_operand)
{
    return (long double)left_operand * (long double)right_operand;
}
#pragma endregion

#pragma region Operator /
Integer operator/(const Integer& left_operand,const Integer& right_operand)
{
    return (long long)left_operand / (long long)right_operand;
}

Float operator/(const Float& left_operand,const Integer& right_operand)
{
    return (long double)left_operand / (long double)right_operand;
}

Float operator/(const Integer& left_operand,const Float& right_operand)
{
    return (long double)left_operand / (long double)right_operand;
}

Float operator/(const Float& left_operand,const Float& right_operand)
{
    return (long double)left_operand / (long double)right_operand;
}
#pragma endregion

#pragma region Operator <
bool operator<(const Integer& left_operand,const Integer& right_operand)
{
    return (long long)left_operand < (long long)right_operand;
}

bool operator<(const Float& left_operand,const Integer& right_operand)
{
    return (long double)left_operand < (long double)right_operand;
}

bool operator<(const Integer& left_operand,const Float& right_operand)
{
    return (long double)left_operand < (long double)right_operand;
}

bool operator<(const Float& left_operand,const Float& right_operand)
{
    return (long double)left_operand < (long double)right_operand;
}
#pragma endregion

#pragma region Operator <=
bool operator<=(const Integer& left_operand,const Integer& right_operand)
{
    return (long long)left_operand <= (long long)right_operand;
}

bool operator<=(const Float& left_operand,const Integer& right_operand)
{
    return (long double)left_operand <= (long double)right_operand;
}

bool operator<=(const Integer& left_operand,const Float& right_operand)
{
    return (long double)left_operand <= (long double)right_operand;
}

bool operator<=(const Float& left_operand,const Float& right_operand)
{
    return (long double)left_operand <= (long double)right_operand;
}
#pragma endregion

#pragma region Operator >
bool operator>(const Integer& left_operand,const Integer& right_operand)
{
    return (long long)left_operand > (long long)right_operand;
}

bool operator>(const Float& left_operand,const Integer& right_operand)
{
    return (long double)left_operand > (long double)right_operand;
}

bool operator>(const Integer& left_operand,const Float& right_operand)
{
    return (long double)left_operand > (long double)right_operand;
}

bool operator>(const Float& left_operand,const Float& right_operand)
{
    return (long double)left_operand > (long double)right_operand;
}
#pragma endregion

#pragma region Operator >=
bool operator>=(const Integer& left_operand,const Integer& right_operand)
{
    return (long long)left_operand >= (long long)right_operand;
}

bool operator>=(const Float& left_operand,const Integer& right_operand)
{
    return (long double)left_operand >= (long double)right_operand;
}

bool operator>=(const Integer& left_operand,const Float& right_operand)
{
    return (long double)left_operand >= (long double)right_operand;
}

bool operator>=(const Float& left_operand,const Float& right_operand)
{
    return (long double)left_operand >= (long double)right_operand;
}
#pragma endregion

#pragma region Operator ==
bool operator==(const Integer& left_operand,const Integer& right_operand)
{
    return (long long)left_operand == (long long)right_operand;
}

bool operator==(const Float& left_operand,const Integer& right_operand)
{
    return (long double)left_operand == (long double)right_operand;
}

bool operator==(const Integer& left_operand,const Float& right_operand)
{
    return (long double)left_operand == (long double)right_operand;
}

bool operator==(const Float& left_operand,const Float& right_operand)
{
    return (long double)left_operand == (long double)right_operand;
}
#pragma endregion
#pragma endregion

/* Integer Class Definition */
const long long Integer::MAX = LLONG_MAX;
const long long Integer::MIN = LLONG_MIN;

Integer::Integer()
{
    this->value = 0;
}

Integer::Integer(long long value)
{
    this->value = value;
}

Integer& Integer::operator=(const Integer& copy)
{
    this->value = copy.value;

    return *this;
}

Integer Integer::operator-() const
{
    return -this->value;
}

Integer Integer::operator++()
{
    return this->value++;
}

Integer::operator byte() const
{
    return (byte)value;
}

Integer::operator short() const
{
    return (short)value;
}

Integer::operator int() const
{
    return (int)value;
}

Integer::operator long() const
{
    return (long)value;
}

Integer::operator long long() const
{
    return value;
}

Integer::operator float() const
{
    return (float)value;
}

Integer::operator double() const
{
    return (double)value;
}

Integer::operator long double() const
{
    return (long double)value;
}

Integer::operator Float() const
{
    return this->value;
}

/* Float Class Definition */
Float::Float()
{
    this->value = 0.0;
}

Float::Float(long double value)
{
    this->value = value;
}

Float& Float::operator=(const Float& copy)
{
    this->value = copy.value;

    return *this;
}

Float Float::operator-() const
{
    return -this->value;
}

Float::operator byte() const
{
    return (byte)this->value;
}

Float::operator short() const
{
    return (short)this->value;
}

Float::operator int() const
{
    return (int)this->value;
}

Float::operator long() const
{
    return (long)this->value;
}

Float::operator long long() const
{
    return (long long)this->value;
}

Float::operator float() const
{
    return (float)this->value;
}

Float::operator double() const
{
    return (double)this->value;
}

Float::operator long double() const
{
    return this->value;
}

Float::operator Integer() const
{
    return this->value;
}
