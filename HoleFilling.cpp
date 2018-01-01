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
#include <random>
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
#define MISSING_VALUE (-1)

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

/**
 * @def EPSILON_INITIAL -1
 * @brief A Macro that sets the initial value for the epsilon parameter.
 */
#define EPSILON_INITIAL (-1)

/**
 * @def Z_INITIAL -1
 * @brief A Macro that sets the initial value for the z parameter.
 */
#define Z_INITIAL (-1)

/**
 * @def DEFAULT_MARK_COLOR 1
 * @brief A Macro that sets the default color value for boundary mark.
 */
#define DEFAULT_MARK_COLOR 1

/**
 * @def NORMALIZATION_FACTOR 255
 * @brief A Macro that sets the value of the normalization factor of the image to the range [0,1].
 */
#define NORMALIZATION_FACTOR 255


/*-----=  Program Arguments Functions  =-----*/


/**
 * @brief The epsilon value used in the default weighted function.
 */
float epsilon = EPSILON_INITIAL;

/**
 * @brief The z value used in the default weighted function.
 */
float z = Z_INITIAL;


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
static Pixel findMissingPixel(float **image, const int rows, const int cols)
{
    for (int x = INITIAL_ROW ; x < rows; ++x)
    {
        for (int y = INITIAL_COLUMN; y < cols; ++y)
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
static Hole calculateHole(float **image, const int rows, const int cols,
                          Pixel &missingPixel, const int connectivity)
{
    Hole hole;

    // Set data for the BFS algorithm.
    std::deque<Pixel> pixelQueue;
    // TODO: Change the visited array.
    auto **visited = new bool *[rows];
    for (int i = 0; i < rows; ++i)
    {
        visited[i] = new bool[cols];
        for (int j = 0; j < cols; ++j)
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
        assert(currentPixel.getNeighbours().empty());
        currentPixel.setNeighbours(connectivity, rows, cols);
        hole.addHolePixels(currentPixel);

        // Get the pixel neighbours according the the pixel connectivity.
        currentPixel.setNeighbours(connectivity, rows, cols);
        // Traverse the neighbours.
        for (const Pixel &neighbour : currentPixel.getNeighbours())
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
                    pixelQueue.emplace_back(currentX, currentY);
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
    for (int i = 0; i < rows; ++i)
    {
        delete[] visited[i];
        visited[i] = nullptr;
    }
    delete[] visited;
    visited = nullptr;

    return hole;
}

/**
 * @brief Apply the default weighted function on the given Pixels.
 * @param lhs The first pixel.
 * @param rhs The second pixel.
 * @return The weighted value of the given pixels.
 */
static float defaultWeightedFunction(const Pixel &lhs, const Pixel &rhs)
{
    double norm = std::pow(lhs.getX() - rhs.getX(), 2) + std::pow(lhs.getY() - rhs.getY(), 2);
    norm = std::sqrt(norm);

    auto weightValue = (float) (1 / (std::pow(norm, z) + epsilon));
    return weightValue;
}

/**
 * @brief Fill the image hole of the given image.
 * @param image A pointer to the image to fix.
 * @param hole The hole in the image.
 * @param weightedFunction The weighted function used in the fill process.
 */
static void fillImageHole(float ***image, const Hole &hole,
                          float (*weightedFunction)(const Pixel&, const Pixel&))
{
    for (const Pixel &x : hole.getHolePixels())
    {
        // For every pixel x in the hole we update it's value using the
        // weighted function and all the pixels in the hole boundary.
        float numerator = 0;
        float denominator = 0;
        for (const Pixel &y : hole.getHoleBoundary())
        {
            float yValue = (*image)[y.getX()][y.getY()];
            float weightedValue = weightedFunction(x, y);
            numerator += weightedValue * yValue;
            denominator += weightedValue;
        }
        assert(denominator != 0);
        float newValue = numerator / denominator;
        (*image)[x.getX()][x.getY()] = newValue;
    }
}

/**
 * @brief Fill the image hole of the given image.
 * @param image A pointer to the image to fix.
 * @param hole The hole in the image.
 * @param weightedFunction The weighted function used in the fill process.
 */
static void neighboursFillImageHole(float ***image, const Hole &hole,
                                    float (*weightedFunction)(const Pixel&, const Pixel&))
{
    for (const Pixel &x : hole.getHolePixels())
    {
        // For every pixel x in the hole we update it's value using the
        // weighted function and all the pixels in the hole boundary.
        float numerator = 0;
        float denominator = 0;

        assert(!x.getNeighbours().empty());
        for (const Pixel &y : x.getNeighbours())
        {
            float yValue = (*image)[y.getX()][y.getY()];
            if (yValue == MISSING_VALUE)
            {
                continue;
            }

            float weightedValue = weightedFunction(x, y);
            numerator += weightedValue * yValue;
            denominator += weightedValue;
        }
        assert(denominator != 0);
        float newValue = numerator / denominator;
        (*image)[x.getX()][x.getY()] = newValue;
    }
}

/*-----=  Image Handling Functions  =-----*/


/**
 * @brief Receive an image from the given image path.
 * @param imagePath The path of the image.
 * @return A Mat object normalized to the range [0,1] of the image.
 */
static cv::Mat receiveImage(const char *imagePath)
{
    cv::Mat image = cv::imread(imagePath, cv::IMREAD_GRAYSCALE);
    if (image.empty())
    {
        // Invalid image argument.
        std::cerr << "Error: invalid image" << std::endl;
        exit(EXIT_FAILURE);
    }
    // Normalize image to the values in [0,1] range.
    image.convertTo(image, CV_32F);
    image = image / NORMALIZATION_FACTOR;
    return image;
}

/**
 * @brief Convert a given CV Mat image representation to a 2D-array.
 * @param cvImage The given image to convert, represented as a CV Mat object with
 *                values in the range [0,1].
 * @param rows The number of rows in the image.
 * @param cols The number of columns in the image.
 * @return A 2D-array of type float representing the image.
 */
static float **convertImageToArray(cv::Mat &cvImage, const int rows, const int cols)
{
    auto **image = new float *[rows];
    for (int i = 0; i < rows; ++i)
    {
        image[i] = new float[cols];
        auto *cvP = cvImage.ptr<float>(i);
        for (int j = 0; j < cols; ++j)
        {
            image[i][j] = cvP[j];
        }
    }

    return image;
}

/**
 * @brief Convert a given 2D-array to a CV Mat image representation.
 * @param image The given image to convert, represented as a 2D-array with
 *        values in the range [0,1].
 * @param rows The number of rows in the image.
 * @param cols The number of columns in the image.
 * @return A CV Mat of type float representing the image.
 */
static cv::Mat convertArrayToImage(float **image, const int rows, const int cols)
{
    cv::Mat cvImage(rows, cols, CV_32F);
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            cvImage.at<float>(i, j) = image[i][j];
        }
    }
    return cvImage;
}

/**
 * @brief Mark the image hole boundaries of the given image.
 * @param image A pointer to the image to mark.
 * @param hole The hole in the image.
 */
static void markBoundaries(float ***image, const Hole &hole, const float markColor)
{
    for (const Pixel &x : hole.getHoleBoundary())
    {
        (*image)[x.getX()][x.getY()] = markColor;
    }
}

/**
 * @brief Display a given image with a given window name.
 * @param image The image to display.
 * @param windowName The window name in the displayed image window.
 */
static void displayImage(const cv::Mat &image, const char *windowName)
{
    cv::namedWindow(windowName, CV_WINDOW_NORMAL);
    cv::imshow(windowName, image);
    cv::waitKey(0);
}

/**
 * @brief Display the results of the program.
 * @param originalImage The original image with the hole in it.
 * @param markedImage The original image with the boundary marked.
 * @param filledImage The filled image.
 */
static void displayResults(const cv::Mat &originalImage, const cv::Mat &markedImage,
                           const cv::Mat &filledImage)
{
    displayImage(originalImage, "Original");
    displayImage(markedImage, "Boundary");
    displayImage(filledImage, "Filled");
}


/*-----=  Generate Hole Functions  =-----*/


/**
 * @brief Generates a random number from the range [lowerBound,upperBound].
 * @param lowerBoundRow The lower bound of the range.
 * @param upperBoundRow The upper bound of the range.
 * @return a random integer from the range [lowerBound,upperBound].
 */
static int generateRandomNumber(const int lowerBound, const int upperBound)
{
    std::random_device                  rand_dev;
    std::mt19937                        generator(rand_dev());
    std::uniform_int_distribution<int>  dist(lowerBound, upperBound);
    return dist(generator);
}

/**
 * @brief Generates random hole in a shape of a rectangle.
 * @param image The image to corrupt.
 * @param rows The number of rows in the image.
 * @param cols The number of columns in the image.
 */
static void generateRandomHole(float ***image, const int rows, const int cols)
{
    // Pick a random indices in the image.
    const int randomRow1 = generateRandomNumber(INITIAL_ROW, rows - 1);
    const int randomCol1 = generateRandomNumber(INITIAL_ROW, cols - 1);
    const int randomRow2 = generateRandomNumber(INITIAL_ROW, rows - 1);
    const int randomCol2 = generateRandomNumber(INITIAL_ROW, cols - 1);
    // Set top left and bottom right points.
    const int topLeftRow = (randomRow1 < randomRow2) ? randomRow1 : randomRow2;
    const int topLeftCol = (randomCol1 < randomCol2) ? randomCol1 : randomCol2;
    const int bottomRightRow = (randomRow1 < randomRow2) ? randomRow2 : randomRow1;
    const int bottomRightCol = (randomCol1 < randomCol2) ? randomCol2 : randomCol1;
    // Corrupt the image.
    int currentRow = topLeftRow;
    while (currentRow <= bottomRightRow)
    {
        int currentCol = topLeftCol;
        while (currentCol <= bottomRightCol)
        {
            (*image)[currentRow][currentCol] = MISSING_VALUE;
            currentCol++;
        }
        currentRow++;
    }
}

/**
 * @brief Generate hole in the image from a given array of pixels.
 * @param image The image to corrupt.
 * @param holePixels The array of pixels which describe the hole.
 * @param holeSize The number of pixels in the hole.
 */
static void generateDefinedHole(float ***image, const Pixel *holePixels, const int holeSize)
{
    for (int i = 0; i < holeSize; ++i)
    {
        Pixel pixel = holePixels[i];
        (*image)[pixel.getX()][pixel.getY()] = MISSING_VALUE;
    }
}


/*-----=  Resources Functions  =-----*/


/**
 * @brief Copy a given image.
 * @param image The image to copy.
 * @param rows The number of rows in that image.
 * @param cols The number of columns in that image.
 * @return
 */
static float **copyImage(float **image, const int rows, const int cols)
{
    auto **copiedImage = new float *[rows];
    for (int i = 0; i < rows; ++i)
    {
        copiedImage[i] = new float[cols];
        for (int j = 0; j < cols; ++j)
        {
            copiedImage[i][j] = image[i][j];
            copiedImage[i][j] = image[i][j];
        }
    }
    return copiedImage;
}

/**
 * @brief Clear memory allocation for the given image.
 * @param image The image memory to free.
 * @param rows The number of rows in that image.
 */
static void clearResources(float ***image, const int rows)
{
    for (int i = 0; i < rows; ++i)
    {
        delete[] (*image)[i];
        (*image)[i] = nullptr;
    }
    delete[] *image;
    *image = nullptr;
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
    epsilon = std::stof(epsilonArgument);
    z = std::stof(zArgument);
    assert(epsilon != EPSILON_INITIAL && z != Z_INITIAL);
    const int connectivity = std::stoi(connectivityArgument);

    // Read the given image and set up data.
    cv::Mat originalImage = receiveImage(imagePath);
    int rows = originalImage.rows;
    int cols = originalImage.cols;

    // Create a corresponding 2D-array of the image.
    float **image = convertImageToArray(originalImage, rows, cols);
    // Generate hole in this image.
    Pixel pixelArray[20] = {Pixel(20, 20), Pixel(20, 21), Pixel(20, 22),
                            Pixel(20, 23), Pixel(20, 24), Pixel(21, 20),
                            Pixel(21, 21), Pixel(21, 22), Pixel(21, 23),
                            Pixel(21, 24), Pixel(22, 20), Pixel(22, 21),
                            Pixel(22, 22), Pixel(22, 23), Pixel(22, 24),
                            Pixel(23, 20), Pixel(23, 21), Pixel(23, 22),
                            Pixel(23, 23), Pixel(24, 20)};
    generateDefinedHole(&image, pixelArray, 20);
    cv::Mat cvImage = convertArrayToImage(image, rows, cols);

    try
    {
        // Find the first missing pixel in the hole.
        Pixel missingPixel = findMissingPixel(image, rows, cols);
        // From this pixel calculate the hole using BFS.
        Hole hole = calculateHole(image, rows, cols, missingPixel, connectivity);

        // Copy the original image and mark the boundaries.
        auto **markedImage = copyImage(image, rows, cols);
        markBoundaries(&markedImage, hole, DEFAULT_MARK_COLOR);
        cv::Mat cvMarked = convertArrayToImage(markedImage, rows, cols);

        // Copy the original image and fill the copy.
        auto **filledImage = copyImage(image, rows, cols);
        fillImageHole(&filledImage, hole, defaultWeightedFunction);
        cv::Mat cvFilled = convertArrayToImage(filledImage, rows, cols);

        // Display results.
        displayResults(cvImage, cvMarked, cvFilled);

        // Clear resources.
        clearResources(&filledImage, rows);
        clearResources(&markedImage, rows);
        clearResources(&image, rows);

        return EXIT_SUCCESS;
    }
    catch (HoleException& exception)
    {
        std::cout << exception.what() << std::endl;
        return EXIT_SUCCESS;
    }
}