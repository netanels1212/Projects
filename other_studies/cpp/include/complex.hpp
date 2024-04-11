/********************************************************************************
Project: Complex
Name: Netanel Shmuel
Reviewer: Jonathan 
Date: 29.1.2024
Version: 1.0
********************************************************************************/
#ifndef ILRD_HRD33_COMPLEX_HPP
#define ILRD_HRD33_COMPLEX_HPP

#include <iostream> // istream, ostream
#include <cassert> // assert

namespace HRD33
{
// Uses generated CCtor, assignment operator, Dtor
class Complex
{
public:
    Complex(double m_real = 0.0, double m_imag = 0.0) : m_real(m_real), m_imag(m_imag) {};

    void SetReal(double);
    void SetImaginary(double);
    double GetReal() const;
    double GetImaginary() const;

private:
    double m_real;
    double m_imag;
};

Complex& operator+=(Complex& cx1_, const Complex& cx2_);
Complex& operator-=(Complex& cx1_, const Complex& cx2_);
Complex& operator*=(Complex& cx1_, const Complex& cx2_);
Complex& operator/=(Complex& cx1_, const Complex& cx2_);
 
const Complex operator+(Complex cx1_, const Complex& cx2_);
const Complex operator-(Complex cx1_, const Complex& cx2_);
const Complex operator*(Complex cx1_, const Complex& cx2_);
const Complex operator/(Complex cx1_, const Complex& cx2_);
 
bool operator==(Complex& cx1_, const Complex& cx2_);
bool operator!=(Complex& cx1_, const Complex& cx2_);
std::ostream& operator<<(std::ostream &os_, const Complex &cx_);
std::istream& operator>>(std::istream &is_, Complex &cx_);


// Implementation
inline void Complex::SetReal(double value_) 
{ 
    m_real = value_; 
}

inline void Complex::SetImaginary(double value_) 
{
    m_imag = value_;
}

inline double Complex::GetReal() const
{
    return m_real;
}

inline double Complex::GetImaginary() const 
{
    return m_imag;
}

inline bool operator==(Complex& cx1_, const Complex& cx2_)
{
    return (cx1_.GetReal() == cx2_.GetReal()) && (cx2_.GetImaginary() == cx2_.GetImaginary());
}

inline bool operator!=(Complex& cx1_, const Complex& cx2_)
{
    return !(cx1_ == cx2_);
}

inline Complex& operator+=(Complex& cx1_, const Complex& cx2_)
{
    cx1_.SetReal(cx1_.GetReal() + cx2_.GetReal());
    cx1_.SetImaginary(cx1_.GetImaginary() + cx2_.GetImaginary());

    return cx1_;
}

inline const Complex operator+(Complex cx1_, const Complex& cx2_)
{
    return (cx1_ += cx2_);
}

inline Complex& operator-=(Complex& cx1_, const Complex& cx2_)
{
    cx1_.SetReal(cx1_.GetReal() - cx2_.GetReal());
    cx1_.SetImaginary(cx1_.GetImaginary() - cx2_.GetImaginary());

    return cx1_;
}

inline const Complex operator-(Complex cx1_, const Complex& cx2_)
{
    return (cx1_ -= cx2_);
}

inline Complex& operator*=(Complex& cx1_, const Complex& cx2_)
{
    cx1_.SetReal(cx1_.GetReal() * cx2_.GetReal());
    cx1_.SetImaginary(cx1_.GetImaginary() * cx2_.GetImaginary());

    return cx1_;
}

inline const Complex operator*(Complex cx1_, const Complex& cx2_)
{
    return (cx1_ *= cx2_);
}

inline Complex& operator/=(Complex& cx1_, const Complex& cx2_)
{
    assert(0 != cx2_.GetReal());
    assert(0 != cx2_.GetImaginary());

    cx1_.SetReal(cx1_.GetReal() / cx2_.GetReal());
    cx1_.SetImaginary(cx1_.GetImaginary() / cx2_.GetImaginary());

    return cx1_;
}

inline const Complex operator/(Complex cx1_, const Complex& cx2_)
{
    assert(0 != cx2_.GetReal());
    assert(0 != cx2_.GetImaginary());

    return (cx1_ /= cx2_);
}

inline std::ostream& operator<<(std::ostream &os_, const Complex &cx_)
{
    double imag_part = cx_.GetImaginary();

    os_ << cx_.GetReal();

    if (0 <= imag_part)
    {
        os_ << '+';
    }
    
    os_ << imag_part << 'i'; 

    return os_;
}
}// end of namespace HRD33

#endif// ILRD_HRD33_COMPLEX_HPP