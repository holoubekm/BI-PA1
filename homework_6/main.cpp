#ifndef __PROGTEST__
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#endif /* __PROGTEST__ */


//#define DEBUG_NEW_SPEAK


struct banned
{
	const char* replace;
	char replace_c;
	int replace_l;

	const char* pattern;
	char pattern_c;
	int pattern_l;
};

struct occur
{
	int pos;
	banned* new_speak;
};

struct letter
{
	int pos;
	int count;
};

letter letters[256];

void AddLetter(char ltr, int pos)
{
	if(letters[(int)ltr].pos == -1)
	{
		letters[(int)ltr].pos = pos;
	}

	letters[(int)ltr].count++;
}

letter* GetLetter(char ltr)
{
	return &letters[(int)ltr];
}

occur* occurs = NULL;
int num_occurs = 0;

void AddOccurence(int pos, banned* new_speak)
{
	occurs = (occur*)realloc(occurs, sizeof(occur)* ++num_occurs);
	int index = num_occurs - 1;
	occurs[index].pos = pos;
	occurs[index].new_speak = new_speak;
}

bool CheckBannedUniq(banned* new_speak1, banned* new_speak2)
{
	int shorter = new_speak1->pattern_l > new_speak2->pattern_l ? new_speak2->pattern_l : new_speak1->pattern_l;

	for (int x = 0; x < shorter; x++)
	{
		if (new_speak1->pattern[x] != new_speak2->pattern[x])
		{
			return false;
		}
	}
	return true;
}

bool CheckBanned(banned** bann, int count)
{
	for (int x = 0; x < count; x++)
	{
		for (int y = 0; y < count; y++)
		{
			if (x != y && CheckBannedUniq(&(*bann)[x], &(*bann)[y]))
			{
				return false;
			}
		}
	}

	return true;
}

bool StartsWithPattern(const char* text, int length, banned* new_speak)
{
	if (length < new_speak->pattern_l)
	{
		return false;
	}

	int count = new_speak->pattern_l;
	const char* pattern = new_speak->pattern;
	
	/*
	while (*text != '\0' && *pattern != '\0' && *(text++) == *(pattern++))
	{
		count--;
	}
	return count == 0;
	*/
	
	int half = count / 2;

	for (int x = 0; x < half; x++)
	{
		if (text[x] != pattern[x] || text[count - x - 1] != pattern[count - x - 1])
		{
			return false;
		}
	}

	if (count % 2 == 1 && count > 2)
	{
		if (text[half + 1] != pattern[half + 1])
		{
			return false;
		}
	}

	return true;
}

void BuildOccurences(const char* text, banned* pts, int count)
{
	num_occurs = 0;
	const char* cur = text;
	int length = strlen(text);
	int pos = 0;

	int index, max;

	while (*cur != '\0')
	{
		letter* ltr = GetLetter(*cur);
		if (ltr->pos != -1)
		{
			index = ltr->pos;
			max = (ltr->pos + ltr->count);
			for (int x = index; x < max; x++)
			{
				if (StartsWithPattern(cur, length, &pts[x]))
				{
					AddOccurence(pos, &pts[x]);
					cur += pts[x].pattern_l - 1;
					pos += pts[x].pattern_l - 1;
					break;
				}
			}
		}

		++cur;
		++pos;
		--length;
	}
}

char* MakeNewString(const char* text)
{
	int num_chars = strlen(text);
	int to_alloc = num_chars;
	for (int x = 0; x < num_occurs; x++)
	{
		to_alloc += occurs[x].new_speak->replace_l;
		to_alloc -= occurs[x].new_speak->pattern_l;
	}

	char* new_speak = (char*)malloc(sizeof(char)* to_alloc + 1);
	new_speak[to_alloc] = '\0';

	int last_index = 0;
	int text_index = 0;
	for (int x = 0; x < num_occurs; x++)
	{
		strncpy(new_speak + last_index, text + text_index, occurs[x].pos - text_index);

		last_index += (occurs[x].pos - text_index);
		text_index = (occurs[x].pos + occurs[x].new_speak->pattern_l);

		strncpy(new_speak + last_index, occurs[x].new_speak->replace, occurs[x].new_speak->replace_l);

		last_index += occurs[x].new_speak->replace_l;
	}
	strncpy(new_speak + last_index, text + text_index, num_chars - text_index);

	return new_speak;
}

int CompareBanned(const void * a, const void * b)
{
	banned* new_speak1 = (banned*)a;
	banned* new_speak2 = (banned*)b;

	return strcmp(new_speak1->pattern, new_speak2->pattern);
}

char * newSpeak(const char * text, const char * (*replace)[2])
{
	int index = 0;
	while (replace[index++][0] != NULL); index--;

#ifdef DEBUG_NEW_SPEAK
	printf("Pocet prvku je: %d\n", index);
#endif

	banned* pts = (banned*)malloc(sizeof(banned)* index);
	for (int x = 0; x < index; x++)
	{
		pts[x].pattern = replace[x][0];
		pts[x].pattern_c = replace[x][0][0];
		pts[x].pattern_l = strlen(replace[x][0]);

		pts[x].replace = replace[x][1];
		pts[x].replace_c = replace[x][1][0];
		pts[x].replace_l = strlen(replace[x][1]);

#ifdef DEBUG_NEW_SPEAK
		printf("%d: %s, %d: %s\n", pts[x].pattern_l, pts[x].pattern, pts[x].replace_l, pts[x].replace);
#endif

	}

	qsort(pts, index, sizeof(banned), CompareBanned);

	for(int x = 0; x < 256; x++)
	{
		letters[x].pos = -1;
		letters[x].count = 0;
	}

	for (int x = 0; x < index; x++)
	{
		if (pts[x].pattern_l > 0)
		{
			AddLetter(pts[x].pattern_c, x);
		}
	}

	if (!CheckBanned(&pts, index))
	{
		free(pts);
		pts = NULL;
		return NULL;
	}
	BuildOccurences(text, pts, index);
	char* new_speak = MakeNewString(text);

	free(pts);
	pts = NULL;
	free(occurs);
	occurs = NULL;

	return new_speak;
}

#ifndef __PROGTEST__
int main ( int argc, char * argv [] )
 {
   char * res;

   const char * d1 [][2] = {
     { "murderer", "termination specialist" },
     { "failure", "non-traditional success" },
     { "specialist", "person with certified level of knowledge" },
     { "dumb", "cerebrally challenged" },
     { "teacher", "voluntary knowledge conveyor" },
     { "evil", "nicenest deprived" },
     { "incorrect answer", "alternative answer" },
     { "student", "client" },
     { NULL, NULL }
    };


   const char * d2 [][2] = {
     { "fail", "suboptimal result" },
     { "failure", "non-traditional success" },
     { NULL, NULL }
    };


   res = newSpeak ( "Everybody is happy.", d1 );
   /* res = "Everybody is happy."*/
   free ( res );

   res = newSpeak ( "The student answered an incorrect answer.", d1 );
   /* res = "The client answered an alternative answer."*/
   free ( res );

   res = newSpeak ( "He was dumb, his failure was expected.", d1 );
   /* res = "He was cerebrally challenged, his non-traditional success was expected."*/
   free ( res );

   res = newSpeak ( "The evil teacher became a murderer.", d1 );
   /* res = "The nicenest deprived voluntary knowledge conveyor became a termination specialist."*/
   free ( res );

   res = newSpeak ( "Devil's advocate.", d1 );
   /* res = "Dnicenest deprived's advocate."*/
   free ( res );

   res = newSpeak ( "Hello.", d2 );
   /* res = NULL */

   return 0;
 }
#endif /* __PROGTEST__ */
