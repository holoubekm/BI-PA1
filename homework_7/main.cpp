#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <cmath>


class Item
{
public:
	inline void InitItem(int p_id, int p_price) { id = p_id; price = p_price; used = 0;}
	inline int GetId() { return id; }
	inline int GetPrice() { return price; }
	inline int GetUsed() { return used; }
	inline void Used() { used++; }
	inline void Reset() { used = 0; }

private:
	int id;
	int price;
	int used;
};

class CargoSolver
{
private: 
	int num_items;
	int desired;
	int total_price;
	int** solutions;
	bool** used;
	Item* items;

public:
	inline void Solve(Item* p_items, int p_num_items, int p_desired)
	{
		num_items = p_num_items;
		desired = p_desired;
		items = p_items;

		SortItems();

		for (int x = 0; x < num_items; x++)
		{
			items[x].Reset();
			total_price += items[x].GetPrice();
		}

		SolveDynamic(p_items);
		Test(0, desired);

		PrintResults();
		
		for (int x = 0; x < num_items; x++)
		{
			delete[] used[x];
		}
		delete[] used;
	}

private:

	inline static int CompareItem(const void* item1, const void* item2)
	{
		Item* a = (Item*)item1;
		Item* b = (Item*)item2;

		return a->GetPrice() < b->GetPrice();
	}

	inline void SortItems()
	{
		qsort(items, num_items, sizeof(Item), CompareItem);
	}

	inline void PrintSolutions()
	{
		for (int x = 0; x < num_items; x++)
		{
			for (int y = 0; y < desired + 1; y++)
			{
				printf("%d ", solutions[x][y]);
			}
			printf("\n");
		}
		printf("\n");
	}

	inline void PrintUsed()
	{
		for (int x = 0; x < num_items; x++)
		{
			for (int y = 0; y < desired + 1; y++)
			{
				printf("%c ", used[x][y] ? 'T' : 'F');
			}
			printf("\n");
		}
		printf("\n");
	}

	inline void Test(int x, int value)
	{
		if(value == 0)
		{
			return;
		}

		if(used[x][value] == true)
		{
			items[x].Used();
			Test(x, value - items[x].GetPrice());
		}
		else
		{
			Test(x + 1, value);
		}
	}

	inline void PrintResults()
	{
		printf(" = ");
		int count = 0;

		for(int x = num_items - 1; x >= 0; x--)
		{
			if(items[x].GetUsed() > 0)
			{
				if(count == 0)
				{
					printf("%d x %d", items[x].GetUsed(), items[x].GetPrice());
				}
				else
				{
					printf(" + %d x %d", items[x].GetUsed(), items[x].GetPrice());
				}

				count++;
			}
		}
		printf("\n");
	}

	inline void SolveDynamic(Item* p_items)
	{
		solutions = new int*[num_items];
		used = new bool*[num_items];
		for (int x = 0; x < num_items; x++)
		{
			solutions[x] = new int[desired + 1];
			used[x] = new bool[desired + 1];
		}

		//Jednicka bude vzdycky pritomna, presne podle zadani
		/*
		for(int x = 0; x < desired + 1; x++)
		{
			solutions[0][x] = x;
			used[0][x] = true;
		}
		*/

		//PrintSolutions();
		//PrintUsed();

		//Postavim si tabulku pro jednotlive hmotnosti, ktera rika, kolik zavazi musim pouzit,
		//zacnu jednickou a postupne se propracuji k dalsim zavazim, jejich pocty zavisi na predchozich
		//Pocty kombinaci pro jednicku a zaroven i dalsi cisla, postupne pribyva moznych zpusobu reseni
		//Proto ubyva nutnych zavazi
		/*
		for (int x = 1; x < num_items; x++)
		{
			for (int value = 0; value < desired + 1; value++)
			{
				int prevItemTarget = solutions[x - 1][value];
				int diff = value - items[x].GetPrice();
				if(diff >= 0 && items[x].GetPrice() <= value)
				{
					int buf = items[x].GetPrice() + solutions[x][diff];
					if(buf <= desired && 1 + solutions[x][diff] < prevItemTarget)
					{
						solutions[x][value] = 1 + solutions[x][diff];
						used[x][value] = true;
					}
					else
					{
						solutions[x][value] = prevItemTarget;
						used[x][value] = false;
					}
				}
				else
				{
					solutions[x][value] = prevItemTarget;
					used[x][value] = false;
				}
			}
		}*/

		for(int x = 0; x < desired + 1; x++)
		{
			solutions[num_items - 1][x] = x;
			used[num_items - 1][x] = true;
		}

		for (int x = num_items - 2; x >= 0; x--)
		{
			for (int value = 0; value < desired + 1; value++)
			{
				if(items[x].GetPrice() > value || solutions[x + 1][value] < 1 + solutions[x][value - items[x].GetPrice()])
				{
					solutions[x][value] = solutions[x + 1][value];
					used[x][value] = false;
				}
				else
				{
					solutions[x][value] = 1 + solutions[x][value - items[x].GetPrice()];
					used[x][value] = true;
				}
			}
		}

		//PrintSolutions();
		//PrintUsed();

		for (int x = 0; x < num_items; x++)
		{
			delete[] solutions[x];
		}
		delete[] solutions;
	}


	inline int max(int first, int second)
	{
		return first > second ? first : second;
	}
};

int main(int argc, char* argv[])
{
	int num_loads = 0;
	printf("Zadejte pocet zavazi:\n");
	if(scanf("%d", &num_loads) != 1 || num_loads < 1)
	{
		printf("Nespravny vstup.\n");
		return 1;
	}

	printf("Zadejte hmotnosti zavazi:\n");
	int cur_value = 0;
	bool found_one = false;
	Item* items = new Item[num_loads];
	for(int x = 0; x < num_loads; x++)
	{
		if(scanf("%d", &cur_value) != 1 || cur_value < 1)
		{
			printf("Nespravny vstup.\n");
			delete[] items;
			return 1;
		}

		for(int y = 0; y < x; y++)
		{
			if(items[y].GetPrice() == cur_value)
			{
				printf("Nespravny vstup.\n");
				delete[] items;
				return 1;
			}
		}

		items[x].InitItem(x, cur_value);
		if(cur_value == 1)
		{
			found_one = true;
		}
	}

	if(!found_one)
	{
		printf("Nespravny vstup.\n");
		delete[] items;
		return 1;
	}

	CargoSolver* solver = new CargoSolver();
	int res = 0;
	int desired_value = 0;

	printf("Zadejte hledanou hmotnost:\n");
	while((res = scanf("%d", &desired_value)) != EOF)
	{
		if(res != 1 || desired_value < 1)
		{
			printf("Nespravny vstup.\n");
			delete[] items;
			return 1;
		}
		
		solver->Solve(items, num_loads, desired_value);
	}

	delete solver;
	//system("pause");
}