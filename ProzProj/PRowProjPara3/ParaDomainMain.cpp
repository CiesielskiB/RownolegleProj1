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


///////////////////////////////////////////////////////////////parallel wenątrzn rekurencji, błędny domenowy/////////////////////////////////////////////////////////////////////
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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////parallel poza rekuręcją, błędny domenowy/////////////////////////////////////////////////////////////////////

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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////parallel użyty bez wynajdywania liczb pierwszych/////////////////////////////////////////////////////////////////////

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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////parallel poza rekurencją, poprawny domenowy/////////////////////////////////////////////////////////////////////

int generatePrimesSeqActualDomain(int table[], int size, int max, int result[]){
	int primesCount = 0;
	int *temp = new int[max];

	if (max <= 2)
	{
		result[0] = 2;
		return 1;
	}
	primesCount = generatePrimesSeqActualDomain(table, size, (int)sqrt(max), temp);

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

int generatePrimesSeqActualDomain(int table[], int size, int max, int min, int result[], int numOfThreads)
{
	int *temp = new int[max - min + 1];
	int primesCount = 0;
	primesCount = generatePrimesSeqActualDomain(table, size, (int)sqrt(max), temp);
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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////parallel poza rekurencją, poprawny domenowy bez liczb pierwszych/////////////////////////////////////////////////////////////////////
int generatePrimesSeqActualDomainWithoutUsingPrimes(int table[], int size, int max, int min, int result[], int numOfThreads)
{
	omp_set_num_threads(numOfThreads);
	int sqrtMax = (int)sqrt(max);
	//wykreslanie
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
		
		for (int p = 2; p < sqrtMax; p++)
		{
			int i = 0;
			if(myStartingPoint % p == 0){
				i = myStartingPoint/p;
			}else{
				i = myStartingPoint/p + 1;
			}
			if(i == 0){
				i+=2;
			}
			while (p * i <= myEndPoint + 2)
			{
				table[p * i - 2] = -1;
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

///////////////////////////////////////////////////////////////Testy/////////////////////////////////////////////////////////////////////

void testMultiParallelGeneration(int maxNumber, int minNumber, int numOfThreads)
{
	clock_t start, stop;
	int *resultMultiPara = new int[maxNumber - minNumber + 1]();
	int *numbers = generateSequence(2, maxNumber);
	start = clock();
	int primesCount = generatePrimesSeqMultiPara(numbers, maxNumber - 2 + 1, maxNumber, minNumber, resultMultiPara, numOfThreads);
	stop = clock();
	printf("%d\n", primesCount);
	printf("Czas przetwarzania dla parallel w rekurencji (multi para) z rozdawaniem mnoznikow wynosi %f sekund\n", ((double)(stop - start) / 1000.0));
	printf("predkosc dla parallel w rekurencji (multi para) z rozdawaniem mnoznikow wynosi %ld liczb na sekunde\n", (long)((maxNumber - minNumber + 1) / ((double)(stop - start) / 1000.0)));
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
	printf("Czas przetwarzania dla wersji parallel po rekurencj z rozdawaniem mnoznikow wynosi %f sekund\n", ((double)(stop - start) / 1000.0));
	printf("predkosc dla wersji parallel po rekurencj z rozdawaniem mnoznikow wynosi %ld liczb na sekunde\n", (long)((maxNumber - minNumber + 1) / ((double)(stop - start) / 1000.0)));
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
	printf("Czas przetwarzania dla wersji z rozdawaniem mnoznikow bez generowania liczb pierwszych wynosi %f sekund\n", ((double)(stop - start) / 1000.0));
	printf("predkosc dla wersji z rozdawaniem mnoznikow bez generowania liczb pierwszych wynosi %ld liczb na sekunde\n", (long)((maxNumber - minNumber + 1) / ((double)(stop - start) / 1000.0)));
	createDivider();

	delete[] numbers;
	delete[] resultNoPrimes;
}



void testPrimesSeqWithActualDomain(int maxNumber, int minNumber, int numOfThreads)
{
	clock_t start, stop;
	int *resultDomain = new int[maxNumber - minNumber + 1]();
	int *numbers = generateSequence(2, maxNumber);
	start = clock();
	int primesCount = generatePrimesSeqActualDomain(numbers, maxNumber - 2 + 1, maxNumber, minNumber, resultDomain, numOfThreads);
	stop = clock();
	printf("%d\n", primesCount);
	printf("Czas przetwarzania dla wersji domenowej z liczbami pierwszymi wynosi %f sekund\n", ((double)(stop - start) / 1000.0));
	printf("predkosc dla wersji domenowej z liczbami pierwszymi wynosi %ld liczb na sekunde\n", (long)((maxNumber - minNumber + 1) / ((double)(stop - start) / 1000.0)));
	createDivider();


	delete[] numbers;
	delete[] resultDomain;
}

void testPrimesSeqWithActualDomainNoPrimes(int maxNumber, int minNumber, int numOfThreads)
{
	clock_t start, stop;
	int *resultDomainNoPrimes = new int[maxNumber - minNumber + 1]();
	int *numbers = generateSequence(2, maxNumber);
	start = clock();
	int primesCount = generatePrimesSeqActualDomainWithoutUsingPrimes(numbers, maxNumber - 2 + 1, maxNumber, minNumber, resultDomainNoPrimes, numOfThreads);
	stop = clock();
	printf("%d\n", primesCount);
	printf("Czas przetwarzania dla wersji domenowej liczb pierwszych bez uzywania liczb pierwszych wynosi %f sekund\n", ((double)(stop - start) / 1000.0));
	printf("predkosc dla wersji domenowej liczb pierwszych bez uzywania liczb pierwszych wynosi %ld liczb na sekunde\n", (long)((maxNumber - minNumber + 1) / ((double)(stop - start) / 1000.0)));
	createDivider();


	delete[] numbers;
	delete[] resultDomainNoPrimes;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main()
{
	int minNumber = 2;
	int maxNumber = 9500000;
	int numOfThreads = 4;

	testMultiParallelGeneration(maxNumber, minNumber, numOfThreads);
	testAfterRecursionParallellGeneration(maxNumber, minNumber, numOfThreads);
	testPrimesSeqWithoutUsingPrimes(maxNumber, minNumber, numOfThreads);
	testPrimesSeqWithActualDomain(maxNumber, minNumber, numOfThreads);
	testPrimesSeqWithActualDomainNoPrimes(maxNumber, minNumber, numOfThreads);
	return 0;
}