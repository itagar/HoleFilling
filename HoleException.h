/**
 * @file HoleException.h
 * @author Itai Tagar
 *
 * @brief A file which contains several Exception Classes for the Hole Filling program.
 */


#ifndef HOLEEXCEPTION_H
#define HOLEEXCEPTION_H


/*-----=  Includes  =-----*/


#include <exception>


/*-----=  Definitions  =-----*/


/**
 * @def PURE_VIRTUAL_FLAG 0
 * @brief A Macro that sets a flag to determine a Virtual method as Pure Virtual.
 */
#define PURE_VIRTUAL_FLAG 0


/*-----=  Class Definition  =-----*/


/**
 * @brief An Abstract Exception Class for Hole Filling Exceptions.
 */
class HoleException : public std::exception
{
public:

    /**
     * @brief Describe the error that occurred when this Exception was thrown.
     * @return An informative message about the Exception.
     */
    virtual const char * what() const throw() override = PURE_VIRTUAL_FLAG;
};


/**
 * @brief A Hole Exception Class for an error with the no missing pixel.
 *
 */
class NoMissingPixelException : public HoleException
{
public:

    /**
     * @brief Describe the error that occurred when this Exception was thrown.
     * @return An informative message about the Exception.
     */
    virtual const char * what() const throw() override { return "No missing pixel in the image."; };
};


#endif