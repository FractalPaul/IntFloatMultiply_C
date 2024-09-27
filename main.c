#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STRING 150

float calcIntMulFloat(int i1, float f1)
{
    return i1 * f1;
}

int calcIntMulInt(int i1, int i2)
{
    return i1 * i2;
}

double calcIntMulDouble(int i1, double d1)
{
    return i1 * d1;
}

float calcFloatMulFloat(float f1, float f2)
{
    return f1 * f2;
}

// Given a string array calculate the length.
int calcLength(char **lines)
{
    int count = 0;
    while (lines[count] != NULL)
    {
        count++;
    }

    return count;
}

/* Free up memory resources for the lines of strings...*/
void freeUp(char **lines)
{
    int cnt = calcLength(lines);

    for (int i = 0; i < cnt; i++)
    {
        if (lines[i] != NULL)
        {
            free(lines[i]);
        }
    }

    free(lines);
}

// Given the provided numbers and calculated values format them into a string for display or saving to file.
char *formatValues(int iVal, float fVal, float ifRes, int iiRes, double idRes, float ffRes, float diff, bool isErr)
{
    // Allocate memory for the result string.  Must make sure length is long enough.
    char *result = (char *)malloc(MAX_STRING * sizeof(char));

    if (result == NULL)
    {
        fprintf(stderr, "Memory allocation in formatValues failed\n");
        exit(1);
    }

    // Format the values into the string.
    sprintf(result, "%d,%.1f,%.1f,%d,%.1f,%.1f,%.1f,%s", iVal, fVal, ifRes, iiRes, idRes, ffRes, diff, isErr ? "True" : "False");

    return result;
}

// Do the calculations of the different types of int, float, and double.
char *calcMultiplications(int iVal, float fVal)
{
    char *line;

    float ifResult = calcIntMulFloat(iVal, fVal);
    int iiResult = calcIntMulInt(iVal, (int)fVal);
    double idResult = calcIntMulDouble(iVal, (double)fVal);
    float ffResult = calcFloatMulFloat(iVal, fVal);
    float diff = ifResult - idResult;
    bool isError = diff != 0.0;

    line = formatValues(iVal, fVal, ifResult, iiResult, idResult, ffResult, diff, isError);

    return line;
}

// Create the header string to describe the comma seperated numbers.
char *produceHeader()
{
    return "A~I,B~F,A~I * B~F,A~I * B~I,A~I * B~dbl,A~F * B~F,Diff, Is Error";
}

// Given the two numbers calculate the possible combinations of the numbers as int, float, double, and negative.
char **calcCombinations(int intVal, float floatVal)
{
    int numCombinations = 5;

    char **array = malloc(sizeof(char *) * (numCombinations + 1));

    // // Allocate memory for each combination result
    // for (int i = 0; i < numCombinations; i++)
    // {
    //     array[i] = malloc(MAX_STRING * sizeof(char));
    //     if (array[i] == NULL)
    //     {
    //         fprintf(stderr, "Memory allocation for line %d failed\n", i);
    //         exit(1);
    //     }
    // }

    array[0] = calcMultiplications(intVal, floatVal);
    array[1] = calcMultiplications(floatVal, intVal);
    array[2] = calcMultiplications(-intVal, -floatVal);
    array[3] = calcMultiplications(intVal, -floatVal);
    array[4] = calcMultiplications(-intVal, floatVal);

    // Ensure the last element is NULL.
    array[numCombinations] = NULL;

    return array;
}

// Given the calculated lines display them to the console.
void displayCombinations(char **lines)
{
    int cnt = calcLength(lines);
    char *header = produceHeader();
    printf("%s\n", header);
    for (int i = 0; i < cnt; i++)
    {
        printf("%s\n", lines[i]);
    }

    freeUp(lines);
}

// Given the two numbers do the combinations of multiplication and display the results.
void doCombinations(int intVal, float floatVal)
{
    char **lines = calcCombinations(intVal, floatVal);

    displayCombinations(lines);
}

// Calculate the Odd Ball prefetched numbers causing error calculations.
void calcOddBallSamples()
{
    printf("Calculating Odd Ball numbers...\n");

    doCombinations(1683, 9971);
    doCombinations(2399, 6995);
    doCombinations(2401, 6997);
    doCombinations(2797, 5999);
    doCombinations(3055, 6111);
    doCombinations(3055, 6995);
}

// Given the calculated lines of strings check to see if there is any errors in the calculations...
bool IsAnyError(char **lines)
{
    int cnt = calcLength(lines);
    bool isErr = false;
    for (int i = 0; i < cnt; i++)
    {
        if (lines[i] != NULL && strstr(lines[i], "True"))
        {
            isErr = true;
        }
    }

    return isErr;
}

// Given a max Value calculate the multiplications for the range from 1 to max Value.
void calcRange(int maxVal)
{
    FILE *file = fopen("IntFloatDiff_C.csv", "w");

    int i;
    float f;
    char *header = produceHeader();

    fprintf(file, "%s\n", header);

    printf("Calculating values up to %d... Please wait...\n", maxVal);

    // Loop through integer values from 1 to 10,000
    for (i = 1; i <= maxVal; i++)
    {
        // Loop through float values from 1.0 to 5000.0
        for (f = 1.0; f <= maxVal; f += 1.0)
        {
            char **lines = calcCombinations(i, f);

            bool isErr = IsAnyError(lines);
            if (isErr)
            {
                int cnt = calcLength(lines);
                for (int i = 0; i < cnt; i++)
                {
                    fprintf(file, "%s\n", lines[i]);
                }
            }

            freeUp(lines);
        }
    }

    fclose(file);

    printf("Done calculating...\n");
}

// Main entry... display instructions and accept command line arguments to envoke calculations.
int main(int argc, char *argv[])
{
    printf("Numberic multiplication error check...\n");
    if (argc == 1)
    {
        printf("input two whole numbers to multiply and see if the produce correct result.\n");
        printf("Usage: %s <A> <B>\n", argv[0]);
        printf("A * B = result.\n");
        printf("Usage: %s <maxVal>\n", argv[0]);
        printf("If maxVal (int) is provided then both A and B will iterate from 1 to the maxVal (10,000)\n");
        printf("If A and B are not provided then a predetermined Odd ball list will be calculated.\n");
    }

    // Convert command line arguments to appropriate data types
    int A = 0;
    float B = 0;
    int maxVal = 0;

    if (argc == 3)
    {
        A = atoi(argv[1]); // Convert A to integer
        B = atof(argv[2]); // Convert B to float
    }
    else if (argc == 2)
    {
        maxVal = atoi(argv[1]); // Convert maxVal to integer
    }

    if (A == 0 && B == 0 && maxVal == 0)
    {
        calcOddBallSamples();
    }
    else if (A != 0 && B != 0)
    {
        doCombinations(A, B);
    }

    if (maxVal > 0)
    {
        calcRange(maxVal);
    }

    return 0;
}