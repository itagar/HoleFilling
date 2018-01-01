/**
 * @file Pixel.h
 * @author Itai Tagar
 *
 * @brief A header file for the Pixel Class.
 */


#ifndef PIXEL_H
#define PIXEL_H


/*-----=  Includes  =-----*/


#include <iostream>
#include <vector>


/*-----=  Class Declaration  =-----*/


/**
 * @brief A Class representing a single Pixel with X and Y coordinates.
 *        X refers to row number where X=0 is the topmost row.
 *        Y refers to column number where Y=0 is the leftmost column.
 */
class Pixel
{
public:
    /**
     * @brief A Default Constructor for the Pixel which sets a pixel at (0,0).
     */
    Pixel();

    /**
     * @brief A Constructor for the Pixel, which receive 2 coordinates
     *        and create a new Pixel object with the given coordinates.
     * @param x The X coordinate in the plane.
     * @param y The Y coordinate in the plane.
     */
    Pixel(const int x, const int y);

    /**
     * @brief Returns the X coordinate value.
     * @return The X coordinate value.
     */
    int getX() const { return _x; };

    /**
     * @brief Returns the Y coordinate value.
     * @return The Y coordinate value.
     */
    int getY() const { return _y; };

    /**
     * @brief Returns the Pixel's neighbours.
     * @return The Pixel's neighbours.
     */
    std::vector<Pixel> getNeighbours() const { return _neighbours; };

    /**
     * @brief Set the neighbours of the Pixel according to the given connectivity value.
     *        The function ignore neighbours that are outside of the image boundaries.
     * @param connectivity The pixel connectivity value.
     * @param maxX The max value in the X coordinates.
     * @param maxY The max value in the Y coordinates.
     */
    void setNeighbours(const int connectivity, const int maxX, const int maxY);

    /**
     * @brief operator << for stream insertion.
     * @param os The output stream.
     * @param pixel The Pixel to stream.
     * @return The output stream with the pixel streamed.
     */
    friend std::ostream& operator<<(std::ostream &os, const Pixel &pixel);

private:
    int _x;  // The X coordinate value for the Pixel.
    int _y;  // The Y coordinate value for the Pixel.
    std::vector<Pixel> _neighbours;  // The Pixel's neighbours.

};


#endif