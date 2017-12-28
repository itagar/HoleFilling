/**
 * @file Hole.cpp
 * @author Itai Tagar
 *
 * @brief A file for the Hole Class implementation.
 */


/*-----=  Includes  =-----*/


#include "Hole.h"


/*-----=  Class Implementation  =-----*/


/**
 * @brief operator << for stream insertion.
 * @param os The output stream.
 * @param hole The Hole to stream.
 * @return The output stream with the hole streamed.
 */
std::ostream& operator<<(std::ostream &os, const Hole &hole)
{
    os << "Hole:" << std::endl;
    for (Pixel pixel : hole.getHolePixels())
    {
        os << pixel << '\t';
    }
    os << std::endl;
    os << "Hole Boundary:" << std::endl;
    for (Pixel pixel : hole.getHoleBoundary())
    {
        os << pixel << '\t';
    }
    return os;
}