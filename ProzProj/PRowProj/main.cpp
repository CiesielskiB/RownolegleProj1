#include <stdio.h>
#include <time.h>
#include <omp.h>
#include <math.h>

int *generateSequence(int min, int max)
{
    int *tab = new int[max - min];
    int j = min;
    for (int i = 0; i <= max - min; i++)
    {
        tab[i] = j++;
    }
    return tab;
}

int generatePrimesSeqDiv(int max, int min, int result[])
{

    int count = 0;
    if (min <= 2)
    {
        count = 1;
        result[0] = 2;
        min++;
    }
    for (int i = min; i <= max; i++)
    {
        bool isPrime = true;
        for (int j = 2; j <= (int)sqrt(i); j++)
        {
            if (i % j == 0)
            {
                isPrime = false;
                break;
            }
        }
        if (isPrime)
        {
            result[count++] = i;
        }
    }

    return count;
}

int generatePrimesSeq(int table[], int size, int max, int result[])
{
    if (max <= 2)
    {
        result[0] = 2;
        return 1;
    }

    int *temp = new int[max];
    int primesCount = generatePrimesSeq(table, size, (int)sqrt(max), temp);

    //wykreslanie
    for (int p = 0; p < primesCount; p++)
    {
        int i = 2;
        int prime = temp[p];
        while (prime * i <= max)
        {
            table[prime * i - 2] = -1;
            i++;
        }
    }

    //wpisywanie liczb pierwszych do tablicy przekazywanej w gore
    int count = 0;
    for (int i = 0; i <= max - 2; i++)
    {
        int currentNumber = table[i];
        if (currentNumber != -1)
        {
            result[count++] = currentNumber;
        }
    }

    delete[] temp;
    return count;
}

int generatePrimesSeq(int table[], int size, int max, int min, int result[])
{
    int *temp = new int[max - min + 1];
    int primesCount = generatePrimesSeq(table, size, max, temp);

    int count = 0;
    for (int i = 0; i < max - min + 1; i++)
    {
        if (temp[i] >= min)
        {
            result[count++] = temp[i];
        }
    }

    return count;
}

int main()
{
    clock_t start, stop;
    int minNumber = 2;
    int maxNumber = 20;
    int *test = generateSequence(2, maxNumber);
    int *result = new int[maxNumber - minNumber + 1];

    start = clock();
    int primesCount = generatePrimesSeq(test, maxNumber - 2 + 1, maxNumber, minNumber, result);
    stop = clock();

    printf("%d\n", primesCount);
    printf("Czas przetwarzania wynosi %f sekund\n", ((double)(stop - start) / 1000.0));

    int *result2 = new int[maxNumber - minNumber + 1];

    start = clock();
    primesCount = generatePrimesSeqDiv(maxNumber, minNumber, result2);
    stop = clock();

    printf("%d\n", primesCount);
    printf("Czas przetwarzania wynosi %f sekund\n", ((double)(stop - start) / 1000.0));

    delete[] result;
    return 0;
}