#include <strixa/math/Integer.h>

#include <assert.h>
#include <string>

using namespace strixa::math;


const size_t SIGN_BIT = 1 << ((sizeof(byte) * CHAR_BIT) - 1);

Integer::Integer(long long initial_value)
{
    this->size = (size_t)ceil((ceil(log(abs(initial_value))/log(2)) + 1) / CHAR_BIT);  //In a nutshell:  Finds out how many bits would be required to express 'initial_value', adds one to that number to account for the sign bit, then divide the whole thing by 8 to find out the number of bytes it would take to express 'initial_value'.
    this->size = this->size == 0 ? 1 : this->size;
    this->value = new ubyte[this->size];
    this->msb = (sbyte*)this->value + (this->size - 1);
    
    memcpy(this->value,&initial_value,this->size);
}

Integer::Integer(const Integer& copy)
{
    this->size = copy.size;
    this->value = new ubyte[this->size];
    this->msb = (sbyte*)(this->value + (this->size - 1));
    memcpy(this->value,copy.value,this->size * sizeof(ubyte));
    
    *this = copy;
}

Integer::Integer(Integer&& move)
{
    this->size = move.size;
    this->value = move.value;
    this->msb = move.msb;

    move.size = 1;
    move.value = new ubyte[move.size];
    move.msb = (sbyte*)(move.value + (move.size - 1));
}

Integer::~Integer()
{
    delete[] this->value;
}

void Integer::resize(size_t new_size)
{
    int    old_size;
    ubyte* old_value;
    
    
    if (this->size == new_size) {
        return;
    }

    old_size = this->size;
    old_value = this->value;
    this->size = new_size;
    this->value = new ubyte[this->size];
    this->msb = (sbyte*)this->value + (this->size - 1);

    if (new_size > 0) {
        memcpy(this->value,old_value,sizeof(byte) * old_size);
        if (new_size > old_size) {
            if ((sbyte)old_value[old_size - 1] < 0) {
                memset(this->value + old_size,-1,new_size - old_size); //Fill the extra bytes that were added with ones if our value is negative.
            } else {
                memset(this->value + old_size,0,new_size - old_size);
            }
        }
    }

    delete[] old_value;
}

Integer& Integer::operator=(const Integer& value)
{
    this->resize(value.size);
    memcpy(this->value,value.value,this->size * sizeof(byte));

    return *this;
}

Integer& Integer::operator=(Integer&& value)
{
    delete[] this->value;
    this->size = value.size;
    this->value = value.value;
    this->msb = value.msb;

    value.size = 1;
    value.value = new ubyte[value.size];

    return *this;
}

Integer Integer::operator+(Integer value) const
{
    ubyte*  addend;
    ubyte*  offset;
    short   result;
    Integer sum = *this;


    if (sum.size == value.size) {
        if ((*sum.msb & SIGN_BIT) == (*value.msb & SIGN_BIT) && (sbyte)(abs(*sum.msb) + abs(*value.msb) + 1) < 0) {
            sum.resize(sum.size + 1);
        }
    } else if (sum.size < value.size) {
        if ((sbyte)abs(*this->msb) + 1 > 0) {
            sum.resize(value.size);
        } else {
            sum.resize(value.size + 1);
        }        
    } else {
        if ((sbyte)abs(*sum.msb) + 1 < 0) {
            sum.resize(sum.size + 1);
        }
    }
    value.resize(sum.size);
    

    result = 0;
    offset = sum.value;
    addend = value.value;
    while (addend <= (ubyte*)value.msb) {
        result = *offset + *addend;
        *offset = (ubyte)result;
        
        result = result >> (sizeof(ubyte) * CHAR_BIT);
        ++offset;
        ++addend;
    }
    while (result != 0 && offset <= (ubyte*)sum.msb) {
        result = *offset + result;
        *offset = (ubyte)result;

        result = result >> (sizeof(ubyte) * CHAR_BIT);
        ++offset;
    }

    if (
        (*sum.msb == 0 && sum.size > 1 && (*(sum.msb - 1) & SIGN_BIT) == 0)
        ||
        (*sum.msb == -1 && sum.size > 1 && *(sum.msb - 1) < 0)
    ) {
        //If the most significant byte of the integer is 0, or the most significant byte is filled with ones and the preceding byte's sign bit is set, decrease the size of the block of memory maintained by the sum integer by one.
        sum.resize(sum.size - 1);
    }

    return sum;
}

Integer& Integer::operator+=(const Integer& value)
{
    *this = *this + value;
    return *this;
}

Integer& Integer::operator++()
{
    return *this += 1;
}

Integer Integer::operator-() const
{
    return (~*this) + 1;
}

Integer Integer::operator-(const Integer& value) const
{
    return *this + -value;
}

Integer& Integer::operator-=(const Integer& value)
{
    return *this = *this + -value;
}

Integer& Integer::operator--()
{
    return *this -= 1;
}

Integer Integer::operator*(Integer value) const
{
    Integer product;


    if (value < 0) {
        while (value < 0) {
            product += *this;
            ++value;
        }

        product = -product;
    } else {
        while (value > 0) {
            product += *this;
            --value;
        }
    }

    return product;
}

Integer& Integer::operator*=(const Integer& value)
{
    return *this = *this * value;
}

Integer Integer::operator/(Integer divisor) const
{
    Integer delta_quotient = (*this->msb & CHAR_BIT) == (*divisor.msb & CHAR_BIT) ? 1 : -1;
    Integer dividend = *this < 0 ? -*this : *this;
    Integer quotient = 0;


    if (divisor < 0) {
        divisor = -divisor;
    }

    while (dividend >= divisor) {
        dividend -= divisor;
        quotient += delta_quotient;
    }

    return quotient;
}

Integer& Integer::operator/=(Integer divisor)
{
    *this = *this / divisor;
    return *this;
}

Integer Integer::operator%(Integer divisor) const
{
    Integer delta_quotient = 0;
    Integer dividend = *this < 0 ? -*this : *this;


    if (divisor < 0) {
        divisor = -divisor;
    }

    while (dividend >= divisor) {
        dividend -= divisor;
    }

    return dividend;
}

Integer& Integer::operator%=(const Integer& divisor)
{
    return *this = *this % divisor;
}

Integer Integer::operator~() const
{
    Integer result(*this);
    
    
    for (size_t offset = 0;offset < result.size;++offset) {
        result.value[offset] = ~result.value[offset];
    }

    return result;
}

bool Integer::operator<(const Integer& value) const
{
    bool true_for_sign;


    if (*this->msb < 0) { //if 'this' is negative
        if (*value.msb >= 0) { //If the comparison value is 0 or positive
            return true;
        } else {
            true_for_sign = false;
        }
    } else { //if 'this' is positive
        if (*value.msb < 0) { //if the comparison value is 0 or negative
            return false;
        } else {
            true_for_sign = true;
        }
    }
    
    if (this->size == value.size) {
        for (int offset = this->size - 1;offset >= 0;--offset) {
            if (this->value[offset] >= value.value[offset]) {
                return !true_for_sign;
            }
        }

        return true_for_sign;
    } else if (this->size < value.size) {
        return true_for_sign;
    } else {
        return !true_for_sign;
    }
}

bool Integer::operator<=(const Integer& value) const
{
    return *this < value || *this == value; 
}

bool Integer::operator==(const Integer& value) const
{
    if (this->size == value.size) {
        for (size_t offset = 0;offset < this->size;++offset) {
            if (this->value[offset] != value.value[offset]) {
                return false;
            }
        }

        return true;
    } else {
        return false;
    }
}

bool Integer::operator>=(const Integer& value) const
{
    return *this > value || *this == value;
}

bool Integer::operator>(const Integer& value) const
{
    bool true_for_sign;


    if (*this->msb < 0) { //if 'this' is negative
        if (*value.msb >= 0) { //If the comparison value is positive
            return false;
        } else {
            true_for_sign = false;
        }
    } else { //if 'this' is positive
        if (*value.msb < 0) { //if the comparison value is negative
            return true;
        } else {
            true_for_sign = true;
        }
    }

    if (this->size == value.size) {
        for (int offset = this->size - 1;offset >= 0;--offset) {
            if (this->value[offset] <= value.value[offset]) {
                return !true_for_sign;
            }
        }

        return true_for_sign;
    } else if (this->size > value.size) {
        return true_for_sign;
    } else {
        return !true_for_sign;
    }
}

std::ostream& strixa::math::operator<<(std::ostream& stream,Integer integer)
{
    char        derp[2];
    bool        negative = false;
    std::string value;

    if (integer < 0) {
        negative = true;
        integer = -integer;
    }

    derp[1] = NULL;
    do {
        derp[0] = (char)((integer % 10).value[0] + '0');
        value = derp + value;
        integer /= 10;
    } while (integer > 0);

    if (negative) {
        value = "-" + value;
    }

    stream << value;

    return stream;
}