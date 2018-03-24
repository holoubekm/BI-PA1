#include <cstdlib>
#include <climits>
#include <cstdio>


class Conts
{
public:
	int* levels;
	int* cache;

	int count;
	int levels_count;
	int base_alt;
	int cache_count;
	int cache_step;

	long total_volume;
public:

	inline Conts()
	{
		count = 0;
		levels_count = 2001;
		base_alt = 1000;
		cache_count = 40;
		cache_step = 0;

		total_volume = 0;

		levels = new int[levels_count];
		cache = new int[cache_count];

		for(int x = 0; x < levels_count; x++)
		{
			levels[x] = 0;
		}
	}

	inline ~Conts()
	{
		delete [] levels;
		delete [] cache;

		levels = NULL;
		cache = NULL;
	}

	inline bool LoadFromSTDIN()
	{
		printf("Zadejte pocet nadrzi:\n");
		if(scanf("%d", &count) != 1 || count < 1 || count > 200000)
		{
			printf("Nespravny vstup.\n");
			return false;
		}

		int alt, w, h, d;

		printf("Zadejte parametry nadrzi:\n");
		for(int x = 0; x < count; x++)
		{
			if(scanf("%d %d %d %d", &alt, &h, &w, &d) != 4 || w < 1 || h < 1 || d < 1)
			{
				printf("Nespravny vstup.\n");
				return false;
			}

			for(int y = 0; y < h; y++)
			{
				levels[y + alt + base_alt] += w * d;
			}

			total_volume += h * w * d;
		}

		cache_step = levels_count/cache_count;
		int semi_total = 0;
		for(int x = 0; x < levels_count; x++)
		{
			semi_total += levels[x];

			if(x % cache_step == 0)
			{
				cache[x / cache_step] = semi_total;
			}
		}

		return true;
	}

	inline bool ComputeWaterLevels()
	{
		int target = 0;
		int result = 0;
		printf("Zadejte objem vody:\n");
		while((result = scanf("%d", &target)) != EOF)
		{
			if(result != 1 || target < 0)
			{
				printf("Nespravny vstup.\n");
				return false;
			}
			else if(target == 0)
			{
				printf("Prazdne.\n");
			}
			else
			{
				ComputeWaterLevel(target);
			}
		}

		return true;
	}


	inline void ComputeWaterLevel(int target)
	{
		if(target > total_volume)
		{
			printf("Pretece.\n");
		
		}
		else
		{
			double result = 0;
			int cache_result = -1;

			for(int x = 0; x < cache_count; x++)
			{
				//printf("%d - %d\n", x, cache[x]);
				if(cache[x] > target)
				{
					cache_result = x - 1;
					break;
				}
			}

			if(cache_result == -1)
			{
				cache_result = 0;
			}
			else
			{
				target -= cache[cache_result];
				cache_result = cache_result * cache_step + 1;
			}

			for(int x = cache_result; x < levels_count; x++)
			{
				//printf("%d - %d\n", x, levels[x]);
				if(target > levels[x])
				{
					target -= levels[x];
				}
				else
				{
					result = x - base_alt + target / (double)levels[x];
					printf("h = %f\n", result);
					return;
				}
			}			
		}
	}
};


int main(int argc, char* argv[])
{
	Conts* wrapper = new Conts();

	if(wrapper->LoadFromSTDIN() == true)
	{
		wrapper->ComputeWaterLevels();
	}
	
	delete wrapper;
	return 0;
}