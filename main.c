#include <stdio.h>

int main(void)
{
    int i;
    float f;
    int maxVal = 10000;
    // Loop through integer values from 1 to 10,000
    for (i = 1; i <= maxVal; i++)
    {
        // Loop through float values from 1.0 to 5000.0
        for (f = 1.0; f <= maxVal; f += 1.0)
        {
            float result = i * f;
            float diff = result - ((float)i * f); // Take the difference between the int*float and float*float.
            
            if (diff > 0.0 || diff < 0.0)
            {
                printf("Multiplication of %d and %.2f is %.2f\n", i, f, result);
            }
        }
    }

    return 0;
}
