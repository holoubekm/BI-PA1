#include <stdlib.h>
#include <stdio.h>

long ComputePhi(long pNumber);
int IsPrime(long pNumber);

int main(int argc, char* argv[])
{
	long number = 0;
	printf("Zadejte n:\n");
	if(scanf("%ld", &number) != 1 || number <= 0)
	{
		printf("Nespravny vstup.\n");
		return -1;
	}

	printf("phi(%ld) = %ld\n", number, ComputePhi(number));
	return 0;
}

long ComputePhi(long pNumber)
{
	long Result = 1;
	long x = 0;

	if(pNumber == 1 || pNumber == 2)
	{
		return 1;
	}

	if(IsPrime(pNumber) == 1)
	{
		return pNumber - 1;
	}

/*
	Specialni pripad, kdyz je cislo sude a my vime, ze dvojka je prvocislo
*/

	if(pNumber % 2 == 0)
	{
		pNumber = pNumber / 2;
		while(pNumber % 2 == 0)
		{
			pNumber = pNumber / 2;
			Result = Result * 2;
		}
	}

/*
	Postupne hledame prvocisla, ktera jsou v cisle obsazena
	Cyklem osetrime, ze v cisle jiz zadne prvocislo neni a proto se nemusime obavat mocnin
	V cyklu staci jit od tri do odmocniny ze soucasneo cisla
*/

	for(x = 3; x * x <= pNumber; x = x + 2)
	{
		if(pNumber % x == 0)
		{
			pNumber = pNumber / x;
			Result = Result * (x - 1);
			while(pNumber % x == 0)
			{
				pNumber = pNumber / x;
				Result = Result * x;
			}
		}
	}

/*
	Zbylo nam prvocislo
*/

	if(pNumber > 1) 
	{
		Result = Result * (pNumber - 1); 
	}

	return Result;

}

int IsPrime(long pNumber)
{
	long x;

	if(pNumber == 1)
	{
		return -1;
	}

	if(pNumber % 2 == 0)
	{
		return -1;
	}

	for(x = 3; x * x <= pNumber; x = x + 2)
	{
		if(pNumber % x == 0)
		{
			return -1;
		}
	}

	return 1;
}