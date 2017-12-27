/**
 * @file HoleFilling.cpp
 * @author Itai Tagar
 * @date 27 Sep 2017
 *
 * @brief A Program that perform hole filling for an image.
 *
 */


/*-----=  Includes  =-----*/


#include <iostream>


/*-----=  Definitions  =-----*/


/**
 * @def VALID_ARGUMENT_COUNT 4
 * @brief A Macro that sets the valid number of arguments for this program.
 */
#define VALID_ARGUMENT_COUNT 4


/*-----=  Main  =-----*/


// TODO: Description.
/**
 * @brief The main function that runs the program.
 * @param argc The number of given arguments.
 * @param argv[] The arguments from the user.
 * @return 0 if the program ended successfully, 1 otherwise.
 */
int main(int argc, char *argv[])
{
    if (argc != VALID_ARGUMENT_COUNT)
    {
        std::cerr << "Usage: HoleFilling <image_path> <epsilon> <z>" << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}