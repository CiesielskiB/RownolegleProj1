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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////Wiele parallel///////////////////////////////////////////////////////////////////////
int generatePrimesSeqMultiPara(int table[], int size, int max, int result[], int numOfThreads)
{
	int primesCount = 0;
	int *temp = new int[max];

	if (max <= 2)
	{
		result[0] = 2;
		return 1;
	}
	primesCount = generatePrimesSeqMultiPara(table, size, (int)sqrt(max), temp, numOfThreads);
	omp_set_num_threads(numOfThreads);
//wykreslanie
#pragma omp parallel
	{
#pragma omp for
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

int generatePrimesSeqMultiPara(int table[], int size, int max, int min, int result[], int numOfThreads)
{
	int *temp = new int[max - min + 1];
	int primesCount = 0;
	primesCount = generatePrimesSeqMultiPara(table, size, max, temp, numOfThreads);

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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////parallel poza rekurencja//////////////////////////////////////////////////////////////////

int generatePrimesSeq(int table[], int size, int max, int result[])
{
	int primesCount = 0;
	int *temp = new int[max];

	if (max <= 2)
	{
		result[0] = 2;
		return 1;
	}
	primesCount = generatePrimesSeq(table, size, (int)sqrt(max), temp);

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

int generatePrimesSeq(int table[], int size, int max, int min, int result[], int numOfThreads)
{
	int *temp = new int[max - min + 1];
	int primesCount = 0;
	primesCount = generatePrimesSeq(table, size, (int)sqrt(max), temp);
	omp_set_num_threads(numOfThreads);
//wykreslanie
#pragma omp parallel
	{
#pragma omp for
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
	}

	int count = 0;

	for (int i = 0; i <= max - 2; i++)
	{
		int currentNumber = table[i];
		if (currentNumber != -1 && currentNumber >= min)
		{
			result[count++] = currentNumber;
		}
	}
	delete[] temp;
	return count;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////Parallel poza rekurencja bez liczb pierwszych/////////////////////////////////////////////////////////////////

int generatePrimesSeqWithoutUsingPrimes(int table[], int size, int max, int min, int result[], int numOfThreads)
{
	//wykreslanie
	int sqrtMax = (int)sqrt(max);
	omp_set_num_threads(numOfThreads);
#pragma omp parallel
	{
#pragma omp for
		for (int p = 2; p <= sqrtMax; p++)
		{
			if (table[p - 2] == -1)
				continue;
			int i = 2;
			while (p * i <= max)
			{
				if (table[p * i - 2] != -1)
				{ //kolejne przyspieszenie
					table[p * i - 2] = -1;
				}
				i++;
			}
		}
	}

	int count = 0;

	for (int i = 0; i <= max - 2; i++)
	{
		int currentNumber = table[i];
		if (currentNumber != -1 && currentNumber >= min)
		{
			result[count++] = currentNumber;
		}
	}

	return count;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////testowanie///////////////////////////////////////////////////////

void testMultiParallelGeneration(int maxNumber, int minNumber, int numOfThreads)
{
	clock_t start, stop;
	int *resultMultiPara = new int[maxNumber - minNumber + 1]();
	int *numbers = generateSequence(2, maxNumber);
	start = clock();
	int primesCount = generatePrimesSeqMultiPara(numbers, maxNumber - 2 + 1, maxNumber, minNumber, resultMultiPara, numOfThreads);
	stop = clock();
	printf("%d\n", primesCount);
	printf("Czas przetwarzania dla parallel w rekurencji (multi para) wynosi %f sekund\n", ((double)(stop - start) / 1000.0));
	printf("predkosc dla parallel w rekurencji (multi para) wynosi %f liczb na sekunde\n", (maxNumber - minNumber + 1) / ((double)(stop - start) / 1000.0));
	createDivider();

	delete[] numbers;
	delete[] resultMultiPara;
}

void testAfterRecursionParallellGeneration(int maxNumber, int minNumber, int numOfThreads)
{
	clock_t start, stop;
	int *resultOneParallel = new int[maxNumber - minNumber + 1]();
	int *numbers = generateSequence(2, maxNumber);
	start = clock();
	int primesCount = generatePrimesSeq(numbers, maxNumber - 2 + 1, maxNumber, minNumber, resultOneParallel, numOfThreads);
	stop = clock();
	printf("%d\n", primesCount);
	printf("Czas przetwarzania dla parallel po rekurencj wynosi %f sekund\n", ((double)(stop - start) / 1000.0));
	printf("predkosc dla parallel po rekurencj wynosi %f liczb na sekunde\n", (maxNumber - minNumber + 1) / ((double)(stop - start) / 1000.0));
	createDivider();

	delete[] numbers;
	delete[] resultOneParallel;
}

void testPrimesSeqWithoutUsingPrimes(int maxNumber, int minNumber, int numOfThreads)
{
	clock_t start, stop;
	int *resultNoPrimes = new int[maxNumber - minNumber + 1]();
	int *numbers = generateSequence(2, maxNumber);
	start = clock();
	int primesCount = generatePrimesSeqWithoutUsingPrimes(numbers, maxNumber - 2 + 1, maxNumber, minNumber, resultNoPrimes, numOfThreads);
	stop = clock();
	printf("%d\n", primesCount);
	printf("Czas przetwarzania dla wersji bez generowania liczb pierwszych wynosi %f sekund\n", ((double)(stop - start) / 1000.0));
	printf("predkosc dla wersji bez generowania liczb pierwszych wynosi %f liczb na sekunde\n", (maxNumber - minNumber + 1) / ((double)(stop - start) / 1000.0));
	createDivider();

	delete[] numbers;
	delete[] resultNoPrimes;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main()
{
	int minNumber = 2;
	int maxNumber = 95000000;
	int numOfThreads = 4;

	testMultiParallelGeneration(maxNumber, minNumber, numOfThreads);
	testAfterRecursionParallellGeneration(maxNumber, minNumber, numOfThreads);
	testPrimesSeqWithoutUsingPrimes(maxNumber, minNumber, numOfThreads);

	return 0;
}