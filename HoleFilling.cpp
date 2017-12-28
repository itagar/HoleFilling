/**
 * @file HoleFilling.cpp
 * @author Itai Tagar
 *
 * @brief A program that perform hole filling for an image.
 */


/*-----=  Includes  =-----*/


#include <iostream>
#include <deque>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <cmath>
#include "Pixel.h"
#include "Hole.h"
#include "HoleException.h"


/*-----=  Definitions  =-----*/


/**
 * @def VALID_ARGUMENT_COUNT 5
 * @brief A Macro that sets the valid number of arguments for this program.
 */
#define VALID_ARGUMENT_COUNT 5

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
 * @def CONNECTIVITY_ARG_INDEX 4
 * @brief A Macro that sets the index of the pixel connectivity value in the program arguments.
 */
#define CONNECTIVITY_ARG_INDEX 4

/**
 * @def FLOAT_POINT '.'
 * @brief A Macro that represents the character for the float point.
 */
#define FLOAT_POINT '.'

/**
 * @def MISSING_VALUE -1
 * @brief A Macro that sets the value of a missing pixel.
 */
#define MISSING_VALUE -1

/**
 * @def INITIAL_ROW 0
 * @brief A Macro that sets the value of the first row coordinate in the image.
 */
#define INITIAL_ROW 0

/**
 * @def INITIAL_COLUMN 0
 * @brief A Macro that sets the value of the first column coordinate in the image.
 */
#define INITIAL_COLUMN 0


/*-----=  Program Arguments Functions  =-----*/


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
static int validateNumericArguments(const char *epsilon, const char *z, const char *connectivity)
{
    if (validateNumeric(epsilon))
    {
        // Invalid epsilon argument.
        std::cerr << "Error: epsilon should be float" << std::endl;
        exit(EXIT_FAILURE);
    }
    if (validateNumeric(z))
    {
        // Invalid z argument.
        std::cerr << "Error: z value should be float" << std::endl;
        exit(EXIT_FAILURE);
    }
    if (*connectivity != '4' && *connectivity != '8')
    {
        // Invalid connectivity argument.
        std::cerr << "Error: pixel connectivity value should be 4 or 8" << std::endl;
        exit(EXIT_FAILURE);
    }
    return EXIT_SUCCESS;
}


/*-----=  Hole Filling Functions  =-----*/


/**
 * @brief Finds a missing pixel in the given image.
 *        If there is no missing pixel the function throws an exception.
 * @param image The image to search in.
 * @param rows The number of rows in the image.
 * @param cols The number of columns in the image.
 * @return A Pixel class represent a missing pixel in the image.
 */
static Pixel findMissingPixel(float **image, const size_t rows, const size_t cols)
{
    for (unsigned int x = INITIAL_ROW ; x < rows; ++x)
    {
        for (unsigned int y = INITIAL_COLUMN; y < cols; ++y)
        {
            if (image[x][y] == MISSING_VALUE)
            {
                // Found a missing pixel.
                return Pixel(x, y);
            }
        }
    }
    throw NoMissingPixelException();
}

/**
 * @brief Calculate the hole in the image from a given missing pixel using BFS.
 * @param image The image containing the hole.
 * @param rows The number of rows in the image.
 * @param cols The number of columns in the image.
 * @param missingPixel A missing pixel inside the hole.
 * @param connectivity The pixel connectivity value.
 * @return A Hole class representing the hole in the image.
 */
static Hole calculateHole(float **image, const size_t rows, const size_t cols,
                          Pixel missingPixel, const int connectivity)
{
    Hole hole;

    // Set data for the BFS algorithm.
    std::deque<Pixel> pixelQueue;
    // TODO: Change the visited array.
    bool **visited = new bool *[rows];
    for (unsigned int i = 0; i < rows; ++i)
    {
        visited[i] = new bool[cols];
        for (unsigned int j = 0; j < cols; ++j)
        {
            visited[i][j] = false;
        }
    }

    // Update data according to the given missing pixel.
    pixelQueue.push_back(missingPixel);

    while(!pixelQueue.empty())
    {
        // Get the next pixel from the queue.
        Pixel currentPixel = pixelQueue.front();
        pixelQueue.pop_front();
        // Mark as visited.
        visited[currentPixel.getX()][currentPixel.getY()] = true;
        // Add the current pixel to the hole.
        hole.addHolePixels(currentPixel);

        // Get the pixel neighbours according the the pixel connectivity.
        currentPixel.setNeighbours(connectivity, (const int) rows, (const int) cols);
        // Traverse the neighbours.
        for (Pixel neighbour : currentPixel.getNeighbours())
        {
            int currentX = neighbour.getX();
            int currentY = neighbour.getY();
            if (!visited[currentX][currentY])
            {
                // If this pixel hasn't been visited we process it and mark as visited.
                visited[currentX][currentY] = true;
                if (image[currentX][currentY] == MISSING_VALUE)
                {
                    // Add this pixel to the queue for next iterations.
                    pixelQueue.push_back(Pixel(currentX, currentY));
                }
                else
                {
                    // Don't add this pixel to the queue, instead add it to the boundary.
                    hole.addHoleBoundary(Pixel(currentX, currentY));
                }
            }
        }
    }

    // Clear resources.
    for (unsigned int i = 0; i < rows; ++i)
    {
        delete[] visited[i];
        visited[i] = nullptr;
    }
    delete[] visited;
    visited = nullptr;

    return hole;
}

static float defaultWeightedFunction(const Pixel lhs, const Pixel rhs, const float z, const float epsilon)
{
    return 1;
}

static void fillImageHole(float ***image, const Hole hole, const float z, const float epsilon,
                          float (*weightedFunction)(const Pixel, const Pixel, const float, const float))
{

}


/*-----=  Main  =-----*/


/**
 * @brief The main function that runs the program. It receives parameters from the user
 *        and runs the Hole Filling on the image given by the user.
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
    const char *imagePath = argv[IMAGE_PATH_ARG_INDEX];
    const char *epsilonArgument = argv[EPSILON_ARG_INDEX];
    const char *zArgument = argv[Z_ARG_INDEX];
    const char *connectivityArgument = argv[CONNECTIVITY_ARG_INDEX];
    validateNumericArguments(epsilonArgument, zArgument, connectivityArgument);
    const float epsilon = std::stof(epsilonArgument);
    const float z = std::stof(zArgument);
    const int connectivity = std::stoi(connectivityArgument);

    float **image = new float *[3];
    size_t rows = 3;
    size_t cols = 4;
    for (int i = 0; i < 3; ++i)
    {
        image[i] = new float[4];
        for (int j = 0; j < 4; ++j)
        {
            image[i][j] = 0;
        }
    }
    image[1][1] = -1;
    image[1][2] = -1;
    image[2][2] = -1;
    image[2][0] = -1;

    try
    {
        // Find the first missing pixel in the hole.
        Pixel missingPixel = findMissingPixel(image, 3, 4);
        std::cout << missingPixel << std::endl;
        std::cout << std::endl;
        // From this pixel. calculate the hole using BFS.
        Hole hole = calculateHole(image, 3, 4, missingPixel, connectivity);
        std::cout << hole << std::endl;

        // Copy the original image and fill the copy.
        float **filledImage = new float *[rows];
        for (int i = 0; i < rows; ++i)
        {
            filledImage[i] = new float[cols];
            for (int j = 0; j < cols; ++j)
            {
                filledImage[i][j] = image[i][j];
            }
        }
        fillImageHole(&filledImage, hole, z, epsilon, defaultWeightedFunction);

        // Clear resources.
        for (unsigned int i = 0; i < rows; ++i)
        {
            delete[] filledImage[i];
            filledImage[i] = nullptr;
        }
        delete[] filledImage;
        filledImage = nullptr;

        return EXIT_SUCCESS;
    }
    catch (HoleException& exception)
    {
        std::cout << exception.what() << std::endl;
        return EXIT_SUCCESS;
    }
}