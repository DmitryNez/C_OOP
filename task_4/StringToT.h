#pragma once
#include <string>
#include <sstream>
#include <conio.h>
#include <stdlib.h>
#include <boost/lexical_cast.hpp>
struct Call
{
    template<class T>
    T operator()(std::string element)
    {
        T result = boost::lexical_cast<T>(element);
        return result;
    }
};



