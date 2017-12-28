/**
 * @file Hole.h
 * @author Itai Tagar
 *
 * @brief A header file for the Hole Class.
 */


#ifndef HOLE_H
#define HOLE_H


/*-----=  Includes  =-----*/


#include "Pixel.h"


/*-----=  Type Definitions  =-----*/


/**
 * @brief A Type Definition for the hole's pixels set.
 */
typedef std::vector<Pixel> holeSet;


/*-----=  Class Declaration  =-----*/


/**
 * @brief A Class representing a Hole with it's pixels.
 */
class Hole
{
public:
    /**
     * @brief operator << for stream insertion.
     * @param os The output stream.
     * @param hole The Hole to stream.
     * @return The output stream with the hole streamed.
     */
    friend std::ostream& operator<<(std::ostream &os, const Hole &hole);

    const holeSet getHolePixels() const { return _holePixels; }

    const holeSet getHoleBoundary() const { return _holeBoundary; }

    void addHolePixels(const Pixel pixel) { _holePixels.push_back(pixel); }

    void addHoleBoundary(const Pixel pixel) { _holeBoundary.push_back(pixel); }

private:
    holeSet _holePixels;  // The Hole's pixels.
    holeSet _holeBoundary;  // The Hole's boundary pixels.

};


#endif