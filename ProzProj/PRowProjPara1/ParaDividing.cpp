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
///////////////////////////////////////////////////////////////////paralell static/////////////////////////////////////////////////////////////////////////

int generatePrimesDivStatic(int max, int min, int result[], int numOfThreads)
{
	int count = 0;
	if (min <= 2)
	{
		result[2] = 1;
		min++;
		count = 1;
	}

	omp_set_num_threads(numOfThreads);
#pragma omp parallel
	{
		int localCount = 0;
#pragma omp for
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
				localCount++;
				result[i] = 1;
			}
		}
#pragma omp atomic
		count += localCount;
	}

	return count;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////paralell dynamic/////////////////////////////////////////////////////////////////////////

int generatePrimesDivDynamic(int max, int min, int result[], int numOfThreads)
{
	int count = 0;
	if (min <= 2)
	{
		result[2] = 1;
		min++;
		count = 1;
	}

	omp_set_num_threads(numOfThreads);
#pragma omp parallel
	{
		int localCount = 0;
#pragma omp for schedule(dynamic, numOfThreads)
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
				localCount++;
				result[i] = 1;
			}
		}
#pragma omp atomic
		count += localCount;
	}

	return count;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////testowanie/////////////////////////////////////////////////////////////////////////

void testStaticGeneration(int maxNumber, int minNumber, int numOfThreads)
{
	clock_t start, stop;
	int *resultStatic = new int[maxNumber - minNumber + 1]();

	start = clock();
	int primesCount = generatePrimesDivStatic(maxNumber, minNumber, resultStatic, numOfThreads);
	stop = clock();

	printf("%d\n", primesCount);
	printf("Czas przetwarzania statycznego wynosi %f sekund\n", ((double)(stop - start) / 1000.0));
	printf("prêdkosc statycznego wynosi %f liczb na sekunde\n", (maxNumber - minNumber + 1)/ ((double)(stop - start) / 1000.0));
	createDivider();

	delete[] resultStatic;
}

void testDynamicGeneration(int maxNumber, int minNumber, int numOfThreads)
{
	clock_t start, stop;
	int *resultDynamic = new int[maxNumber - minNumber + 1]();

	start = clock();
	int primesCount = generatePrimesDivDynamic(maxNumber, minNumber, resultDynamic, numOfThreads);
	stop = clock();

	printf("%d\n", primesCount);
	printf("Czas przetwarzania dynamicznego wynosi %f sekund\n", ((double)(stop - start) / 1000.0));
	printf("predkosc dynamicznego wynosi %f liczb na sekunde\n", (maxNumber - minNumber + 1) / ((double)(stop - start) / 1000.0));
	createDivider();

	delete[] resultDynamic;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main()
{
	int minNumber = 2;
	int maxNumber = 95000000;
	int numOfThreads = 4;

	testStaticGeneration(maxNumber, minNumber, numOfThreads);
	testDynamicGeneration(maxNumber, minNumber, numOfThreads);
	
	return 0;
}