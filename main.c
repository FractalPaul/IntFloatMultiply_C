#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

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

char *formatValues(int iVal, float fVal, float ifRes, int iiRes, double idRes, float ffRes, float diff, bool isErr)
{
    // Allocate memory for the result string.  Must make sure length is long enough.
    char *result = (char *)malloc(150 * sizeof(char));

    // Format the values into the string.
    sprintf(result, "%d,%.1f,%.1f,%d,%.1f,%.1f,%.1f,%s", iVal, fVal, ifRes, iiRes, idRes, ffRes, diff, isErr ? "True" : "False");

    return result;
}

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
char *produceHeader()
{
    return "A~I,B~F,A~I * B~F,A~I * B~I,A~I * B~dbl,A~F * B~F,Diff, Is Error";
}

void calcOddBallSamples()
{
    char *header = produceHeader();
    printf("%s\n", header);

    printf("%s\n", calcMultiplications(1683, 9971));
    printf("%s\n", calcMultiplications(2399, 6995));
    printf("%s\n", calcMultiplications(2401, 6997));
    printf("%s\n", calcMultiplications(2797, 5999));
    printf("%s\n", calcMultiplications(3055, 6111));
    printf("%s\n", calcMultiplications(3055, 6995));
}

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
            char *line = calcMultiplications(i, f);

            bool isErr = strstr(line, "True");
            if (isErr)
            {
                fprintf(file, "%s\n", line);
            }
            free(line);
        }
    }

    printf("Done calculating...\n");
}

int main(void)
{
    calcOddBallSamples();

    calcRange(10000);

    return 0;
}