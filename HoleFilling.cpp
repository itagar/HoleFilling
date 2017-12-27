/**
 * @file HoleFilling.cpp
 * @author Itai Tagar
 * @date 27 Sep 2017
 *
 * @brief A Program that perform hole filling for an image.
 */


/*-----=  Includes  =-----*/


#include <iostream>


/*-----=  Definitions  =-----*/


/**
 * @def VALID_ARGUMENT_COUNT 4
 * @brief A Macro that sets the valid number of arguments for this program.
 */
#define VALID_ARGUMENT_COUNT 4

/**
 * @def IMAGE_PATH_ARG_INDEX 1
 * @brief A Macro that sets the index of the image path in the program arguments.
 */
#define IMAGE_PATH_ARG_INDEX 1

/**
 * @def EPSILON_ARG_INDEX 2
 * @brief A Macro that sets the index of the epsilon value in the program arguments.
 */
#define EPSILON_ARG_INDEX 2

/**
 * @def Z_ARG_INDEX 3
 * @brief A Macro that sets the index of the z value in the program arguments.
 */
#define Z_ARG_INDEX 3

/**
 * @def FLOAT_POINT '.'
 * @brief A Macro that represents the character for the float point.
 */
#define FLOAT_POINT '.'


/*-----=  Program Arguments  =-----*/


/**
 * @brief Validate that a given argument (represented as a char *) is a floating number.
 * @param arg The argument to validate.
 * @return 0 if the argument represent a floating number, 1 otherwise.
 */
static int validateNumeric(const char *arg)
{
    while (*arg != '\0')
    {
        if (!(isdigit(*arg)) && (*arg != FLOAT_POINT))
        {
            // The current argument represent something that is not a float number.
            return EXIT_FAILURE;
        }
        arg++;
    }
    return EXIT_SUCCESS;
}

/**
 * @brief Validate that a given program arguments (represented as a char *) is a floating number.
 * @param epsilon The argument that represent the epsilon value.
 * @param z The argument that represent the z value.
 * @return 0 if the both arguments represent a floating number, 1 otherwise.
 */
static int validateNumericArguments(const char *epsilon, const char *z)
{
    if (validateNumeric(epsilon))
    {
        // Invalid epsilon argument.
        std::cerr << "epsilon should be float" << std::endl;
        exit(EXIT_FAILURE);
    }
    if (validateNumeric(z))
    {
        // Invalid z argument.
        std::cerr << "z value should be float" << std::endl;
        exit(EXIT_FAILURE);
    }
    return EXIT_SUCCESS;
}


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
    // Handle program arguments.
    if (argc != VALID_ARGUMENT_COUNT)
    {
        // Invalid number of arguments.
        std::cerr << "Usage: HoleFilling <image_path> <epsilon> <z>" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::string imagePath = argv[IMAGE_PATH_ARG_INDEX];
    char *epsilonArgument = argv[EPSILON_ARG_INDEX];
    char *zArgument = argv[Z_ARG_INDEX];
    // Check that arguments are float numbers.
    validateNumericArguments(epsilonArgument, zArgument);
    float epsilon = std::stof(epsilonArgument);
    float z = std::stof(zArgument);

    return EXIT_SUCCESS;
}