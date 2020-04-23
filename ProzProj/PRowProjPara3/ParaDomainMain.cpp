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

int generatePrimesSeqMultiPara(int table[], int size, int max, int result[],  int numOfThreads)
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
		for (int i = 2; i < max / 2; i++)
		{
			for (int p = 0; p < primesCount; p++)
			{
				int prime = temp[p];
				if (prime * i - 2 <= max)
				{
					if (table[prime * i - 2] != -1)
					{
						table[prime * i - 2] = -1;
					}
				}
				else
				{
					break;
				}
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
#pragma omp parallel
	{
#pragma omp for
		for (int i = 2; i < max / 2; i++)
		{
			for (int p = 0; p < primesCount; p++)
			{
				int prime = temp[p];
				if (prime * i - 2 <= max)
				{
					if (table[prime * i - 2] != -1)
					{
						table[prime * i - 2] = -1;
					}
				}
				else
				{
					break;
				}
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


int generatePrimesSeqActualDomain(int table[], int size, int max, int min, int result[], int numOfThreads)
{
	int *temp = new int[max - min + 1];
	int primesCount = 0;
	primesCount = generatePrimesSeq(table, size, (int)sqrt(max), temp);
	omp_set_num_threads(numOfThreads);
#pragma omp parallel
	{
		int myThreadNum = omp_get_thread_num();
		int myStartingPoint = 0;
		int myEndPoint = 0;
		if(size % numOfThreads == 0){
			myStartingPoint = (size/numOfThreads) * myThreadNum;
			myEndPoint = myStartingPoint + (size/numOfThreads) - 1;
		}else{
			myStartingPoint = (size/numOfThreads + 1) * myThreadNum;
			myEndPoint = myStartingPoint + (size/numOfThreads + 1) - 1;
		}
		
		for (int p = 0; p < primesCount; p++)
		{
			int prime = temp[p];
			int i = 0;
			if(myStartingPoint % prime == 0){
				i = myStartingPoint/prime;
			}else{
				i = myStartingPoint/prime + 1;
			}
			if(i == 0){
				i+=2;
			}
			while (prime * i <= myEndPoint + 2)
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


int generatePrimesSeqWithoutUsingPrimes(int table[], int size, int max, int min, int result[], int numOfThreads)
{
	omp_set_num_threads(numOfThreads);
	int sqrtMax = (int)sqrt(max);
	//wykreslanie
#pragma omp parallel
	{
#pragma omp for
		for (int i = 2; i < max / 2; i++)
		{
			for (int p = 2; p <= sqrtMax; p++)
			{
				if (p * i - 2 <= max)
				{
					if (table[p * i - 2] != -1)
					{
						table[p * i - 2] = -1;
					}
				}
				else
				{
					break;
				}
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

	delete[] numbers;
	delete[] resultOneParallel;
}

void testPrimesSeqWithoutUsingPrimes(int maxNumber, int minNumber, int numOfThreads)
{
	clock_t start, stop;
	int *resultNoPrimes = new int[maxNumber - minNumber + 1]();
	int *numbers = generateSequence(2, maxNumber);
	start = clock();
	int primesCount = generatePrimesSeqActualDomain(numbers, maxNumber - 2 + 1, maxNumber, minNumber, resultNoPrimes, numOfThreads);
	stop = clock();
	printf("%d\n", primesCount);
	printf("Czas przetwarzania dla wersji bez generowania liczb pierwszych wynosi %f sekund\n", ((double)(stop - start) / 1000.0));

	//delete[] numbers;
	//delete[] resultNoPrimes;
}


int main()
{
	int minNumber = 2;
	int maxNumber = 99000000;
	int numOfThreads = 4;

	testMultiParallelGeneration(maxNumber, minNumber, numOfThreads);
	testAfterRecursionParallellGeneration(maxNumber, minNumber, numOfThreads);
	testPrimesSeqWithoutUsingPrimes(maxNumber, minNumber, numOfThreads);
	return 0;
}