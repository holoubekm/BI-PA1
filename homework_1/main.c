#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <float.h>
#include <limits.h>


#define VERYSMALL  1.0E-150
#define EPSILON    1.0E-8

int Compare(double num1, double num2);
double Area(double radius);
double Distance(double x, double y, double a, double b);

int main(int argc, char* argv[])
{
	double x, y, z;
	double a, b, c;

	int switched = 0;
	double buf = 0.f;

/*
	x = 0; y = 0; z = 4.5;
	a = 10; b = 10; c = 15;
*/
	
	printf("Zadejte parametry kruznice #1:\n");
	if(scanf("%lf %lf %lf", &x, &y, &z) != 3 || z <= 0.0f)
	{
		printf("Nespravny vstup.\n");
		return 1;
	}

	printf("Zadejte parametry kruznice #2:\n");
	if(scanf("%lf %lf %lf", &a, &b, &c) != 3 || c <= 0.f)
	{
		printf("Nespravny vstup.\n");
		return 1;
	}	
	
	if(z < c)
	{
		buf = x;
		x = a;
		a = buf;

		buf = y;
		y = b;
		b = buf;

		buf = z;
		z = c; 
		c = buf;

		switched = 1;
	}

	if(Compare(a, x) == 1 && Compare(b, y) == 1 && Compare(c, z) == 1)
	{
		printf("Kruznice splyvaji, prekryv: %.6f\n", Area(z));
		#ifndef __PROGTEST__
			system ("pause");
		#endif
		return 0;	
	}

	if(Compare(Distance(x, y, a, b), (z + c)) == 1)
	{
		printf("Vnejsi dotyk, zadny prekryv.\n");
		#ifndef __PROGTEST__
			system ("pause");
		#endif
		return 0;	
	}

	if(Distance(x, y, a, b) > (z + c))
	{
		printf("Kruznice lezi vne sebe, zadny prekryv.\n");
		#ifndef __PROGTEST__
			system ("pause");
		#endif
		return 0;	
	}


	if((Distance(x, y, a, b) + c) < z)
	{
		printf("Kruznice #%d lezi uvnitr kruznice #%d, prekryv: %.6f\n", switched == 1 ? 1 : 2, switched == 1 ? 2 : 1, Area(c));
		#ifndef __PROGTEST__
			system ("pause");
		#endif
			return 0;	
	}

	if(Compare(Distance(x, y, a, b) + c, z) == 1)
	{
		printf("Vnitrni dotyk, kruznice #%d lezi uvnitr kruznice #%d, prekryv: %.6f\n", switched == 1 ? 1 : 2, switched == 1 ? 2 : 1, fabs(Area(c)));
		#ifndef __PROGTEST__
			system ("pause");
		#endif
		return 0;	
	}



	if(Distance(x, y, a, b) < (z + c))
	{
		double dist = Distance(a, b, x, y);
		double cbd = 2 * acos((z * z + dist * dist - c * c) / (2 * z * dist));
		double cad = 2 * acos((c * c + dist * dist - z * z) / (2 * c * dist));

		double total = (cbd * z * z) / 2 - (z * z * sin(cbd)) / 2 + (cad * c * c) / 2 - (c * c * sin(cad)) / 2;
		printf("Kruznice se protinaji, prekryv: %.6f\n", total);
		#ifndef __PROGTEST__
			system ("pause");
		#endif
		return 0;
	}

#ifndef __PROGTEST__
  system ("pause");
#endif
	return 2;
}

int Compare(double num1, double num2)
{
	double maxAbs;
	double absDiff;

	absDiff = fabs(num1 - num2);
    if (absDiff < VERYSMALL)
    {
        return 1;
    }

    maxAbs  = fmax(fabs(num1), fabs(num2));
    return (absDiff/maxAbs) < EPSILON;
}

double Area(double radius)
{
	return M_PI * radius * radius;
}

double Distance(double x, double y, double a, double b)
{
	return sqrt((x - a) * (x - a) + (y - b) * (y - b));
}