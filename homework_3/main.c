#ifndef __PROGTEST__
#include <stdio.h>
#endif /* __PROGTEST__ */

/* Pocty dni v mesici */

const int mesice[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
const int prestupne[] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

const int dny[] = {5580, 5040, 5580, 5400, 5580, 5400, 5580, 5580, 5400, 5580, 5400, 5580};
const int dny_p[] = {5580, 5220, 5580, 5400, 5580, 5400, 5580, 5580, 5400, 5580, 5400, 5580};

/* Pocet kukani za jeden cely den */
const int za_den = 180;
const int za_pul = 90;

int JePrestupny(int y)
{
	if ((y & 3) == 0 && ((y % 25) != 0 || (y & 15) == 0))
	{
		return 1;
	}
	return 0;
}

int TestZadani(int y1, int m1, int d1, int h1, int i1, int y2, int m2, int d2, int h2, int i2)
{
	if(y1 < 1600 || m1 < 1 || m1 > 12 || h1 < 0 || h1 > 23 || i1 < 0 || i1 > 59)
	{
		return 0;
	}

	if(y2 < 1600 || m2 < 1 || m2 > 12 || h2 < 0 || h2 > 23 || i2 < 0 || i2 > 59)
	{
		return 0;
	}

	if(y2 < y1)
	{
		return 0;
	}

	if(y1 == y2 && m2 < m1)
	{
		return 0;
	}

	if(y1 == y2 && m1 == m2 && d2 < d2)
	{
		return 0;
	}

	if(y1 == y2 && m1 == m2 && d1 == d2 && h2 < h1)
	{
		return 0;
	}

	if(y1 == y2 && m1 == m2 && d1 == d2 && h1 == h2 && i2 < i1)
	{
		return 0;
	}

	if(JePrestupny(y1))
	{
		if(d1 > prestupne[m1 - 1])
		{
			return 0;
		}
	}
	else
	{
		if(d1 > mesice[m1 - 1])
		{
			return 0;
		}
	}

	if(JePrestupny(y2))
	{
		if(d2 > prestupne[m2 - 1])
		{
			return 0;
		}
	}
	else
	{
		if(d2 > mesice[m2 - 1])
		{
			return 0;
		}
	}

	return 1;
}

void ZbytekPoKukani(int h1, int i1, int h2, int i2, long long int * cuckoo )
{
	long long int kukani_start = 0;
	long long int kukani_konec = 0;

	/* Prvni den */
	if(h1 < 12)
	{
		kukani_start += za_pul;
	}
	else
	{
		h1 -= 12;
	}

	if(i1 == 0)
	{
		kukani_start += h1;
		if(h1 == 0)
		{
			kukani_start += 12;
		}
	}
	
	if(i1 <= 30)
	{
		kukani_start++;
	}

	kukani_start += (12 - h1)*(h1 + 1 + 12)/2;
	kukani_start += (12 - h1 - 1);

	/* Dalsi den */
	if(h2 > 12)
	{
		kukani_konec += za_pul;
		h2 = h2 - 12;
	}
	
	if(i2 >= 30)
	{
		kukani_konec++;
	}
	/*
	if(i2 == 0)
	{	
		kukani_konec += h2;
	}
	*/
	kukani_konec += (1 + h2)*(h2)/2;
	kukani_konec += (h2);

	(*cuckoo) =  kukani_start + kukani_konec;
}


long long int PocetPrestupnych(long int y1, long int y2)
{
	long long int konec = ((y2 - 1) / 4) - ((y2 - 1) / 100) + ((y2 - 1) / 400);
	long long int zacatek = (y1 / 4) - (y1 / 100) + (y1 / 400);

	return konec - zacatek;
}

long long int DniOd1600(long long int y, long int m, int d)
{
	long long int x;
	long long int dni = 0;
	long int let = (y - 1600);
	dni = let * 365;

	if(y > 1600)
	{
		dni += PocetPrestupnych(1600, y);
	}

	if(JePrestupny(y))
	{
		for(x = 0; x < m - 1; x++)
		{
			dni += prestupne[x];
		}
	}
	else
	{
		for(x = 0; x < m - 1; x++)
		{
			dni += mesice[x];
		}
	}

	dni += d;

	return dni;
}

int cuckooClock ( int y1, int m1, int d1, int h1, int i1,
                  int y2, int m2, int d2, int h2, int i2, 
                  long long int * cuckoo )
 {
	 long long int kukani = 0;
	 long long int zbytek = 0;

	 long long int dniOdY1 = DniOd1600(y1, m1, d1);
	 long long int dniOdY2 = DniOd1600(y2, m2, d2);
	 long long int rozdil = dniOdY2 - dniOdY1;

		ZbytekPoKukani(h1, i1, h2, i2, &zbytek);

		kukani = zbytek;
		kukani += rozdil * za_den;
		kukani -= za_den;

	 if(rozdil < 0)
	 {
		 return 0;
	 }

	 if(TestZadani(y1, m1, d1, h1, i1, y2, m2, d2, h2, i2) != 1)
	 {
		 return 0;
	 }

	 if(rozdil == 0)
	 {
		 int min1 = h1 * 60 + i1;
		 int min2 = h2 * 60 + i2;
		 if(min2 < min1)
		 {
			 return 0;
		 }
	 }


	 (*cuckoo) = kukani;
	 return 1;
	 

 }

#ifndef __PROGTEST__

void Test(long long int a, int res, long long int b)
{
	if(res == 1)
	{
		if(a == b)
		{
		}
		else
		{
			printf("Spatne vyslo %lld, ale melo byt %lld\n\n", a, b);
		}
	}
	else
	{
		printf("Navracena nula \n\n");
	}
}

int main ( int argc, char * argv [] ) 
 {
   long long int cuckoo;
   int res = 0;


   /*****************************************/
  

   res = cuckooClock ( 2156, 2, 11, 2, 39, 2088970909, 6, 27, 9, 6, &cuckoo);
   Test(cuckoo, 1, 137336336579208);

   res = cuckooClock ( 2013, 10, 1, 11, 0, 2013, 10, 1, 12, 0, &cuckoo );
   Test(cuckoo, 1, 24);

   res = cuckooClock ( 2013, 10, 1, 11, 0, 2013, 10, 1, 12, 0, &cuckoo );
   Test(cuckoo, 1, 24);

   res = cuckooClock ( 2100, 12, 26, 0, 0, 2100, 12, 31, 0, 0, &cuckoo );
   Test(cuckoo, 1, 912);

   res = cuckooClock ( 2084, 11, 19, 0, 0, 2084, 11, 19, 0, 45, &cuckoo);
   Test(cuckoo, 1, 13);


   res = cuckooClock ( 2000, 12, 24, 0, 0, 2000, 12, 29, 0, 0, &cuckoo );
   Test(cuckoo, 1, 912);

   res = cuckooClock ( 2046, 10, 12, 0, 0, 2046, 10, 12, 0, 0, &cuckoo );
   Test(cuckoo, 1, 12);


   
   res = cuckooClock ( 2013, 10,  1, 13, 15,
                       2013, 10,  1, 18, 45, &cuckoo );
   Test(cuckoo, 1, 26);

   res = cuckooClock ( 2013, 10,  1, 13, 15,
                       2013, 10,  2, 11, 20, &cuckoo );
   Test(cuckoo, 1, 165);

   res = cuckooClock ( 2013,  1,  1, 13, 15,
                       2013, 10,  5, 11, 20, &cuckoo );
   Test(cuckoo, 1, 49845);

   res = cuckooClock ( 2013, 10,  1,  0,  0,
                       2013, 10,  1, 12,  0, &cuckoo );
   Test(cuckoo, 1, 102);

   res = cuckooClock ( 2013, 10,  1,  0, 15,
                       2013, 10,  1,  0, 25, &cuckoo );
   Test(cuckoo, 1, 0);

   res = cuckooClock ( 2013, 10,  1, 12,  0,
                       2013, 10,  1, 12,  0, &cuckoo );
   Test(cuckoo, 1, 12);

   
   res = cuckooClock ( 2012,  1,  1, 13, 15,
                       2012, 10,  5, 11, 20, &cuckoo );
   Test(cuckoo, 1, 50025);

   res = cuckooClock ( 1900,  1,  1, 13, 15,
                       1900, 10,  5, 11, 20, &cuckoo );
   Test(cuckoo, 1, 49845);

   res = cuckooClock ( 2013, 10,  1,  0,  0,
                       2013, 10,  1, 12,  0, &cuckoo );
   Test(cuckoo, 1, 102);

   res = cuckooClock ( 2013, 10,  1,  0, 15,
                       2013, 10,  1,  0, 25, &cuckoo );
   Test(cuckoo, 1, 0);

   res = cuckooClock ( 2013, 10,  1, 12,  0,
                       2013, 10,  1, 12,  0, &cuckoo );
   Test(cuckoo, 1, 12);

   res = cuckooClock ( 2013, 11,  1, 12,  0,
                       2013, 10,  1, 12,  0, &cuckoo );
   Test(cuckoo, 0, -1);

   res = cuckooClock ( 2013, 10, 32, 12,  0,
                       2013, 11, 10, 12,  0, &cuckoo );
   Test(cuckoo, 0, -1);

   res = cuckooClock ( 2100,  2, 29, 12,  0,
                       2100,  2, 29, 12,  0, &cuckoo );
   Test(cuckoo, 0, -1);

   res = cuckooClock ( 2400,  2, 29, 12,  0,
                       2400,  2, 29, 12,  0, &cuckoo );
   Test(cuckoo, 1, 12);
   
   system("pause");
   return 0;
 }
#endif /* __PROGTEST__ */