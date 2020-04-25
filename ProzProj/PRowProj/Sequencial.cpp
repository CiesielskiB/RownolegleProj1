#include <stdio.h>
#include <time.h>
#include <omp.h>
#include <math.h>

void createDivider() {
	printf("-------------------------------------------------------------------------\n");
	printf("-------------------------------------------------------------------------\n");
}


void displayNumbers(int numbers[], int lineSize, int numbersCount) {
	int numberInLine = 0;
	for (int i = 0; i < numbersCount; i++) {
		printf("%d ", numbers[i]);
		if (numberInLine++ >= lineSize) {
			printf("\n");
			numberInLine = 0;
		}
	}
}

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

	delete[] temp;
    return count;
}



void testSieveGeneration(int maxNumber, int minNumber)
{
	clock_t start, stop;
	int *resultMultiPara = new int[maxNumber - minNumber + 1]();
	int *numbers = generateSequence(2, maxNumber);
	start = clock();
	int primesCount = generatePrimesSeq(numbers, maxNumber - 2 + 1, maxNumber, minNumber, resultMultiPara);
	stop = clock();
	printf("%d\n", primesCount);
	printf("Czas przetwarzania sita w rekurencji wynosi %f sekund\n", ((double)(stop - start) / 1000.0));
	printf("predkosc sita wynosi %f liczb na sekunde\n", (maxNumber - minNumber + 1) / ((double)(stop - start) / 1000.0));

	createDivider();


	delete[] numbers;
	delete[] resultMultiPara;
}

void testDivitingGeneration(int maxNumber, int minNumber)
{
	clock_t start, stop;
	int *resultOneParallel = new int[maxNumber - minNumber + 1]();
	start = clock();
	int primesCount = generatePrimesSeqDiv(maxNumber, minNumber, resultOneParallel);
	stop = clock();
	printf("%d\n", primesCount);
	printf("Czas przetwarzania dla dzielenia wynosi %f sekund\n", ((double)(stop - start) / 1000.0));
	printf("predkosc dzielenia wynosi %f liczb na sekunde\n", (maxNumber - minNumber + 1) / ((double)(stop - start) / 1000.0));

	createDivider();


	delete[] resultOneParallel;
}



int main()
{
	int minNumber = 2;
	int maxNumber = 95000000;

	testSieveGeneration(maxNumber, minNumber);
	testDivitingGeneration(maxNumber, minNumber);

    return 0;
}