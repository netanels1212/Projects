#include "complex.hpp"

namespace HRD33
{
std::istream& operator>>(std::istream &is_, Complex &cx_)
{
    double real_input = 0;
    double imag_input = 0;
    char plusOrMinus = 0;
    char iSymbol = 0;

    is_ >> real_input >> plusOrMinus >> imag_input >> iSymbol;

    if (('+' != plusOrMinus && '-' != plusOrMinus) || 'i' != iSymbol || is_.fail())
    {
        is_.setstate(std::ios_base::failbit);
        
        return is_;
    }

    cx_.SetReal(real_input);

    if ('-' == plusOrMinus)
    {
       cx_.SetImaginary(-1 * imag_input); 
    }

    else
    {
        cx_.SetImaginary(imag_input);
    }

    return is_;
}
}// namespace HRD33