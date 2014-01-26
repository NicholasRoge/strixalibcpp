#ifndef Strixa_Math_Operation_H
#define Strixa_Math_Operation_H


typedef unsigned char byte;


namespace Strixa
{
    namespace Math
    {
        class ArithmeticValue;
        class Integer;
        class Float;


        class ArithmeticValue
        {
            /* Member Methods */
            public:
                virtual operator byte() const = 0;

                virtual operator short() const = 0;

                virtual operator int() const = 0;

                virtual operator long() const = 0;

                virtual operator long long() const = 0;

                virtual operator float() const = 0;

                virtual operator double() const = 0;

                virtual operator long double() const = 0;
        };

        class Integer : public ArithmeticValue
        {
            /* Static Properties */
            public:
                static const long long MAX;
                static const long long MIN;

            /* Member Properties */
            private:
                long long value;

            /* Member Methods */
            public:
                Integer();

                Integer(long long value);

                Integer& operator =(const Integer& copy);

                Integer operator -() const;

                Integer operator ++();

                Integer operator --();

                operator byte() const;

                operator short() const;

                operator int() const;

                operator long() const;

                operator long long() const;

                operator float() const;

                operator double() const;

                operator long double() const;

                operator Float() const;
        };

        class Float : public ArithmeticValue
        {
            /* Member Properties */
            private:
                long double value;

            /* Member Methods */
            public:
                Float();

                Float(long double value);

                Float& operator =(const Float& cast);

                Float operator -() const;

                virtual operator byte() const;

                virtual operator short() const;

                virtual operator int() const;

                virtual operator long() const;

                virtual operator long long() const;

                virtual operator float() const;

                virtual operator double() const;

                virtual operator long double() const;

                virtual operator Integer() const;
        };
    }
}

Strixa::Math::Float sin(const Strixa::Math::Float& radians);
Strixa::Math::Float tan(const Strixa::Math::Float& radians);
Strixa::Math::Float cos(const Strixa::Math::Float& radians);

Strixa::Math::Integer operator+(const Strixa::Math::Integer& left_operand,const Strixa::Math::Integer& right_operand);
Strixa::Math::Float operator+(const Strixa::Math::Float& left_operand,const Strixa::Math::Integer& right_operand);
Strixa::Math::Float operator+(const Strixa::Math::Integer& left_operand,const Strixa::Math::Float& right_operand);
Strixa::Math::Float operator+(const Strixa::Math::Float& left_operand,const Strixa::Math::Float& right_operand);

Strixa::Math::Integer operator-(const Strixa::Math::Integer& left_operand,const Strixa::Math::Integer& right_operand);
Strixa::Math::Float operator-(const Strixa::Math::Float& left_operand,const Strixa::Math::Integer& right_operand);
Strixa::Math::Float operator-(const Strixa::Math::Integer& left_operand,const Strixa::Math::Float& right_operand);
Strixa::Math::Float operator-(const Strixa::Math::Float& left_operand,const Strixa::Math::Float& right_operand);

Strixa::Math::Integer operator*(const Strixa::Math::Integer& left_operand,const Strixa::Math::Integer& right_operand);
Strixa::Math::Float operator*(const Strixa::Math::Float& left_operand,const Strixa::Math::Integer& right_operand);
Strixa::Math::Float operator*(const Strixa::Math::Integer& left_operand,const Strixa::Math::Float& right_operand);
Strixa::Math::Float operator*(const Strixa::Math::Float& left_operand,const Strixa::Math::Float& right_operand);

Strixa::Math::Integer operator/(const Strixa::Math::Integer& left_operand,const Strixa::Math::Integer& right_operand);
Strixa::Math::Float operator/(const Strixa::Math::Float& left_operand,const Strixa::Math::Integer& right_operand);
Strixa::Math::Float operator/(const Strixa::Math::Integer& left_operand,const Strixa::Math::Float& right_operand);
Strixa::Math::Float operator/(const Strixa::Math::Float& left_operand,const Strixa::Math::Float& right_operand);

bool operator<(const Strixa::Math::Integer& left_operand,const Strixa::Math::Integer& right_operand);
bool operator<(const Strixa::Math::Float& left_operand,const Strixa::Math::Integer& right_operand);
bool operator<(const Strixa::Math::Integer& left_operand,const Strixa::Math::Float& right_operand);
bool operator<(const Strixa::Math::Float& left_operand,const Strixa::Math::Float& right_operand);

bool operator<=(const Strixa::Math::Integer& left_operand,const Strixa::Math::Integer& right_operand);
bool operator<=(const Strixa::Math::Float& left_operand,const Strixa::Math::Integer& right_operand);
bool operator<=(const Strixa::Math::Integer& left_operand,const Strixa::Math::Float& right_operand);
bool operator<=(const Strixa::Math::Float& left_operand,const Strixa::Math::Float& right_operand);

bool operator>(const Strixa::Math::Integer& left_operand,const Strixa::Math::Integer& right_operand);
bool operator>(const Strixa::Math::Float& left_operand,const Strixa::Math::Integer& right_operand);
bool operator>(const Strixa::Math::Integer& left_operand,const Strixa::Math::Float& right_operand);
bool operator>(const Strixa::Math::Float& left_operand,const Strixa::Math::Float& right_operand);

bool operator>=(const Strixa::Math::Integer& left_operand,const Strixa::Math::Integer& right_operand);
bool operator>=(const Strixa::Math::Float& left_operand,const Strixa::Math::Integer& right_operand);
bool operator>=(const Strixa::Math::Integer& left_operand,const Strixa::Math::Float& right_operand);
bool operator>=(const Strixa::Math::Float& left_operand,const Strixa::Math::Float& right_operand);

bool operator==(const Strixa::Math::Integer& left_operand,const Strixa::Math::Integer& right_operand);
bool operator==(const Strixa::Math::Float& left_operand,const Strixa::Math::Integer& right_operand);
bool operator==(const Strixa::Math::Integer& left_operand,const Strixa::Math::Float& right_operand);
bool operator==(const Strixa::Math::Float& left_operand,const Strixa::Math::Float& right_operand);


#endif