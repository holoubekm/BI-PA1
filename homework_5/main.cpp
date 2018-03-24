#include <cstdio>
#include <iostream>
#include <cmath>
#include <stdlib.h>


int total_area = 0;
float half_area = 0;
int best_solution = 0;

struct solution
{
	int x_pos, y_pos, cols, rows, area;
};

solution* sols = NULL;
int num_sols = 0;
int alloc_sols = 0;
int last_area = 0;

void AddBest(int rows, int cols, int x_pos, int y_pos, int area, int diff_area)
{
	if(alloc_sols == 0)
	{
		alloc_sols = 3;
		sols = (solution*)malloc(sizeof(solution) * alloc_sols);
	}
	else if(num_sols >= alloc_sols)
	{
		alloc_sols *= 1.5f;
		sols = (solution*)realloc(sols, sizeof(solution) * alloc_sols);
	}

	if(num_sols > 0)
	{
		//int last_area = abs(sols[0].area - half_area);
		//int new_area = abs(area - half_area);
		if(last_area > diff_area)
		{
			num_sols = 0;
		}

		if(last_area < diff_area)
		{
			return;
		}
	}

	bool found = false;

	for(int x = 0; x < num_sols; x++)
	{
		if(sols[x].cols == cols
			&& sols[x].rows == rows
			&& sols[x].x_pos == x_pos
			&& sols[x].y_pos == y_pos)
		{
			found = true;
			break;
		}
	}

	if(!found)
	{
		sols[num_sols].area = area;
		sols[num_sols].x_pos = x_pos;
		sols[num_sols].y_pos = y_pos;
		sols[num_sols].cols = cols;
		sols[num_sols].rows = rows;
		best_solution = area;

		last_area = abs(area - half_area);

		num_sols++;
	}
}

void PrintResults()
{
	//bool found = false;

	/*
	int count = 0;
	for(int x = 0; x < num_sols; x++)
	{
		for(int y = 0; y < x; y++)
		{
			if(sols[x].cols == sols[y].cols && 
				sols[x].rows == sols[y].rows && 
				sols[x].x_pos == sols[y].x_pos && 
				sols[x].y_pos == sols[y].y_pos)
			{
				found = true;
			}
		}

		if(!found)
		{
			count++;
		}
		found = false;
	}
	*/

	printf("Vysledku: %d\n", num_sols);
	for(int x = 0; x < num_sols; x++)
	{/*
		for(int y = 0; y < x; y++)
		{
			if(sols[x].cols == sols[y].cols && 
				sols[x].rows == sols[y].rows && 
				sols[x].x_pos == sols[y].x_pos && 
				sols[x].y_pos == sols[y].y_pos)
			{
				found = true;
			}
		}
		*/
		//W x H @ (X, Y): IN : OUT
		//if(!found)
		//{
			printf("%d x %d @ (%d, %d): %d : %d\n", sols[x].cols, sols[x].rows, sols[x].x_pos, sols[x].y_pos, sols[x].area, total_area - sols[x].area);
		//}
		//found = false;
	}
}

void FreeMatrix(int** matrix, int rows)
{
	//printf("Freein' %d rows\n", rows);
	for(int x = 0; x < rows; x++)
	{
		free(matrix[x]);
	}
	free(matrix);
	matrix = NULL;
}

int LoadFromSTDIN(int*** mtx, int* rows, int* cols)
{
	int ret = 0;
	int cur_num = 0;
	char futile = 0;
	bool new_line = true;

	int** matrix = NULL;

	int num_cols = 0, num_rows = 1;
	int cur_col = 0, cur_row = 0;

	printf("Zadejte matici:\n");
	while((ret = scanf("%d%c", &cur_num, &futile)) != EOF)
	{
		//printf("%d - %d <%c>\n", ret, cur_num, futile);
		if(ret != 2 || (futile != ',' && futile != '\n') || cur_num < 1)
		{
			if(ret != 1 || cur_num < 1)
			{
				if(new_line)
				{
					FreeMatrix(matrix, num_rows - 1);
				}
				else
				{
					FreeMatrix(matrix, num_rows);
				}

				printf("Nespravny vstup.\n");
				return 1;
			}
		}

		if(new_line)
		{
			//Alokace pro dalsi radek
			matrix = (int**)realloc(matrix, sizeof(int*) * num_rows);
			matrix[num_rows - 1] = (int*)malloc(sizeof(int) * num_cols);
			//printf("Allocated %d rows\n", num_rows);
			new_line = false;
		}


		if(cur_row == 0)
		{
			num_cols++;
			//Alokace pro dalsi sloupce
			//printf("Allocated %d cols\n", num_cols);
			for(int x = 0; x < num_rows; x++)
			{
				matrix[x] = (int*)realloc(matrix[x], sizeof(int) * num_cols);
			}
		}

		if(cur_col < num_cols)
		{
			//printf("w: %d, %d: %d\n", cur_row, cur_col, cur_num);
			matrix[cur_row][cur_col] = cur_num;
		}

		cur_col++;

		//printf("%d <%c>\n", cur_num, futile);
		if(futile == '\n')
		{
			//printf("new line\n");

			if(cur_col != num_cols)
			{
				FreeMatrix(matrix, num_rows);
				printf("Nespravny vstup.\n");
				return 1;
			}

			cur_col = 0;
			cur_row++;
			num_rows = cur_row + 1;
			new_line = true;
		}
	}

	if(cur_col != 0 && cur_col != num_cols)
	{
		FreeMatrix(matrix, num_rows);
		printf("Nespravny vstup.\n");
		return 1;
	}

	

	if(num_rows == 1 && num_cols == 0)
	{
		printf("Nespravny vstup.\n");
		return 1;
	}

	//printf("%d %d", num_rows, num_cols);

	if(num_rows != 1) 
	{
		num_rows--;
	}

	if(num_rows == 1 && num_cols == 1)
	{
		FreeMatrix(matrix, num_rows);
		printf("Nespravny vstup.\n");
		return 1;
	}

	//printf("c: %d, r: %d\n", num_cols, num_rows);
	*rows = num_rows;
	*cols = num_cols;
	*mtx = matrix;

	return 0;
}

void Print(int** matrix, int rows, int cols, int x_pos, int y_pos)
{
	printf("\n%d %d\n", rows, cols);
	for(int x = y_pos; x < y_pos + rows; x++)
	{
		for(int y = x_pos; y < x_pos + cols; y++)
		{
			printf("%d ", matrix[x][y]);
		}
		printf("\n");
	}
}


void Print(int** matrix, int rows, int cols)
{
	Print(matrix, rows, cols, 0, 0);
}

int GetArea(int** sat, int rows, int cols, int x_pos, int y_pos)
{
	int total;
	/*
	for(int x = x_pos; x < x_pos + cols; x++)
	{
		for(int y = y_pos; y < y_pos + rows; y++)
		{
			total += matrix[y][x];
		}
	}*/

	total = sat[y_pos + rows - 1][x_pos + cols - 1];
	if(x_pos > 0)
	{
		total -= sat[y_pos + rows - 1][x_pos - 1];
		if(y_pos > 0)
		{
			total += sat[y_pos - 1][x_pos - 1];
		}
	}
	
	if(y_pos > 0)
	{
		total -= sat[y_pos - 1][x_pos + cols - 1];
	}

	return total;
}

int** GenerateSAT(int** matrix, int rows, int cols)
{
	int** buf = (int**) malloc(sizeof(int*) * rows);
	for(int x = 0; x < rows; x++)
	{
		buf[x] = (int*) malloc(sizeof(int) * cols);
		for(int y = 0; y < cols; y++)
		{
			total_area += buf[x][y] = matrix[x][y];
			if(x > 0)
			{
				buf[x][y] += buf[x - 1][y];

				if(y > 0)
				{
					buf[x][y] -= buf[x - 1][y - 1];
				}
			}
			if(y > 0)
			{
				buf[x][y] += buf[x][y - 1];
			}				
		}
	}

	last_area = total_area;
	half_area = total_area / 2.0f;
	return buf;
}

int SplitInSub(int** sat, int rows, int cols)
{
	int area = 0;
	int diff_area = 0;

	for(int x = rows; x >= 0; x--)
	{
		for(int y = cols; y >= 0; y--)
		{
			if(x != 0 && y != 0)
			{
				for(int a = 0; a < rows - x + 1; a++)
				{
					for(int b = 0; b < cols - y + 1; b++)
					{
						area = GetArea(sat, x, y, b, a);

						//int last_area = abs(sols[0].area - half_area);
						
						
						diff_area = abs(area - half_area);
						if(last_area >= diff_area)
						{
							AddBest(x, y, b, a, area, diff_area);
						}
						
/*
						diff_area = area - half_area;

						if(diff_area > 0 && last_area >= diff_area)
						{
							AddBest(x, y, b, a, area, diff_area);
						}
						else if(diff_area <= 0 && last_area <= diff_area)
						{
							AddBest(x, y, b, a, area, -diff_area);
						}*/
					}
				}
			}
		}
	}
	return 0;
}

int main(int argc, char** argv)
{
	int** matrix = NULL;
	int** sat = NULL;

	int rows, cols;

	bool mat_alloc = false;
	mat_alloc = !LoadFromSTDIN(&matrix, &rows, &cols);

	if(mat_alloc)
	{
		sat = GenerateSAT(matrix, rows, cols);
		/*
		AddBest(2, 3, 0, 0, 10);
		AddBest(1, 5, 2, 3, 10);
		AddBest(1, 6, 8, 7, 9);
		AddBest(7, 8, 6, 5, 11);
		*/

		//Print(matrix, rows, cols);
		//Print(sat, rows, cols);

		//printf("Total area: %d\n\n", GetArea(matrix, rows, cols, 0, 0));
		SplitInSub(sat, rows, cols);

		//printf("%d\n", GetArea(sat, 2, 2, 1, 1));
		//printf("%d\n", GetArea(sat, 2, 2, 0, 0));
		PrintResults();
	}

	if(mat_alloc)
	{
		FreeMatrix(matrix, rows);
		FreeMatrix(sat, rows);
	}
	free(sols);

	//system("pause");

	return 0;
}