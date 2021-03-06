/*
	This file is part of Warzone 2100.
	Copyright (C) 2007-2009  Warzone Resurrection Project

	Warzone 2100 is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	Warzone 2100 is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with Warzone 2100; if not, write to the Free Software
	Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
*/

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <errno.h>

#ifndef WIN32
#include <stdbool.h>
#include <limits.h>
#else
typedef int bool;
#define PATH_MAX 255
#define true 1
#define false 0
#endif

// To compile: gcc -o ~/bin/simplipie simplipie.c -Wall -g -O0 -Wshadow

#define iV_IMD_TEX	0x00000200
#define iV_IMD_XNOCUL	0x00002000
#define iV_IMD_TEXANIM	0x00004000
#define MAX_POLYGON_SIZE 6 // the game can't handle more

static char *input = "";
static bool verbose = false;

typedef struct {
	int index[MAX_POLYGON_SIZE];
	int texCoord[MAX_POLYGON_SIZE][2];
	int vertices;
	int frames, rate, width, height; // animation data
	bool cull;
} WZ_FACE;

typedef struct {
	int x, y, z, reindex;
	bool dupe;
} WZ_POSITION;

static void parse_args(int argc, char **argv)
{
	unsigned int i = 1;

	for (i = 1; argc >= 2 + i && argv[i][0] == '-'; i++)
	{
		if (argv[i][1] == 'v')
		{
			verbose = true;
		}
	}
	if (argc < 1 + i)
	{
		fprintf(stderr, "Syntax: simplipie [-v] input_filename\n");
		fprintf(stderr, "  -v  Verbose mode.\n");
		exit(1);
	}
	input = argv[i++];
}

static void dump_to_pie(FILE *ctl, FILE *fp)
{
	int num, x, y, z, levels, level;
	char s[200];

	num = fscanf(fp, "PIE %d\n", &x);
	if (num != 1)
	{
		fprintf(stderr, "Bad PIE file %s\n", input);
		exit(1);
	}
	
 	if (x != 2)
	{
		fprintf(stderr, "Unknown PIE version %d in %s\n", x, input);
		exit(1);
	}	
	fprintf(ctl, "PIE 2\n");

	num = fscanf(fp, "TYPE %d\n", &x); // ignore
	if (num != 1)
	{
		fprintf(stderr, "Bad TYPE directive in %s\n", input);
		exit(1);
	}
	fprintf(ctl, "TYPE 200\n");

	num = fscanf(fp, "TEXTURE %d %s %d %d\n", &z, s, &x, &y);
	if (num != 4)
	{
		fprintf(stderr, "Bad TEXTURE directive in %s\n", input);
		exit(1);
	}
	fprintf(ctl, "TEXTURE %d %s 256 256\n", z, s);

	num = fscanf(fp, "LEVELS %d\n", &levels);
	if (num != 1)
	{
		fprintf(stderr, "Bad LEVELS directive in %s\n", input);
		exit(1);
	}
	fprintf(ctl, "LEVELS %d", levels);

	for (level = 0; level < levels; level++)
	{
		int j, points, faces, facesPIE3, textureArrays = 1;
		WZ_FACE *faceList;
		WZ_POSITION *posList;

		num = fscanf(fp, "\nLEVEL %d\n", &x);
		if (num != 1)
		{
			fprintf(stderr, "Bad LEVEL directive in %s.\n", input);
			exit(1);
		}
		if (level + 1 != x)
		{
			fprintf(stderr, "LEVEL directive in %s was %d should be %d.\n", input, x, level + 1);
			exit(1);
		}
		fprintf(ctl, "\nLEVEL %d\n", x);

		num = fscanf(fp, "POINTS %d\n", &points);
		if (num != 1)
		{
			fprintf(stderr, "Bad POINTS directive in %s, level %d.\n", input, level);
			exit(1);
		}

		posList = malloc(sizeof(WZ_POSITION) * points);
		for (j = 0; j < points; j++)
		{
			num = fscanf(fp, "%d %d %d\n", &posList[j].x, &posList[j].y, &posList[j].z);
			if (num != 3)
			{
				fprintf(stderr, "File %s. Bad POINTS entry level %d, number %d.\n", input, level, j);
				exit(1);
			}
			posList[j].dupe = false;
		}

		num = fscanf(fp, "POLYGONS %d", &faces);
		if (num != 1)
		{
			fprintf(stderr, "Bad POLYGONS directive in %s, level %d.\n", input, level);
			exit(1);			
		}

		facesPIE3 = faces;	// for starters
		faceList = malloc(sizeof(WZ_FACE) * faces);
		for (j = 0; j < faces; ++j)
		{
			int k;
			unsigned int flags;

			num = fscanf(fp, "\n%x", &flags);
			if (num != 1)
			{
				fprintf(stderr, "File %s. Bad POLYGONS texture flag entry level %d, number %d\n", input, level, j);
				exit(1);
			}
			if (!(flags & iV_IMD_TEX))
			{
				fprintf(stderr, "File %s. Bad texture flag entry level %d, number %d - no texture flag!\n", input, level, j);
				exit(1);
			}

			if (flags & iV_IMD_XNOCUL)
			{
				faceList[j].cull = true;
				facesPIE3++;	// must add additional face that is faced in the opposite direction
			}
			else
			{
				faceList[j].cull = false;
			}

			num = fscanf(fp, "%d", &faceList[j].vertices);
			if (num != 1)
			{
				fprintf(stderr, "File %s. Bad POLYGONS vertices entry level %d, number %d\n", input, level, j);
				exit(1);
			}
			assert(faceList[j].vertices <= MAX_POLYGON_SIZE); // larger polygons not supported
			assert(faceList[j].vertices >= 3); // points and lines not supported

			// Read in vertex indices and texture coordinates
			for (k = 0; k < faceList[j].vertices; k++)
			{
				num = fscanf(fp, "%d", &faceList[j].index[k]);
				if (num != 1)
				{
					fprintf(stderr, "File %s. Bad vertex position entry level %d, number %d\n", input, level, j);
					exit(1);
				}
			}
			if (flags & iV_IMD_TEXANIM)
			{
				num = fscanf(fp, "%d %d %d %d", &faceList[j].frames, &faceList[j].rate, &faceList[j].width, &faceList[j].height);
				if (num != 4)
				{
					fprintf(stderr, "File %s. Bad texture animation entry level %d, number %d.\n", input, level, j);
					exit(1);
				}
				if (faceList[j].frames <= 1)
				{
					fprintf(stderr, "File %s. Level %d, polygon %d has a single animation frame. Disabled.\n", input, level, j);
					faceList[j].frames = 0;
				}
				if (textureArrays < faceList[j].frames)
				{
					textureArrays = faceList[j].frames;
				}
			}
			else
			{
				faceList[j].frames = 0;
				faceList[j].rate = 0;
				faceList[j].width = 1; // to avoid division by zero
				faceList[j].height = 1;
			}
			for (k = 0; k < faceList[j].vertices; k++)
			{
				num = fscanf(fp, "%d %d", &faceList[j].texCoord[k][0], &faceList[j].texCoord[k][1]);
				if (num != 2)
				{
					fprintf(stderr, "File %s. Bad texture coordinate entry level %d, number %d\n", input, level, j);
					exit(1);
				}
			}
		}

		/* Remove duplicate points */
		x = 0;
		for (j = 0; j < points; j++)
		{
			int k;

			for (k = j + 1; k < points; k++)
			{
				// if points in k are equal to points in j, replace all k with j in face list
				if (posList[k].x == posList[j].x && posList[k].y == posList[j].y && posList[k].z == posList[j].z && !posList[k].dupe)
				{
					posList[k].dupe	= true;	// oh noes, a dupe! let's skip it when we write them out again.
					posList[k].reindex = x;	// rewrite face list to point here
				}
			}
			if (!posList[j].dupe)
			{
				posList[j].reindex = x;
				x++;
			}
		}

		fprintf(ctl, "POINTS %d", x);
		for (j = 0; j < points; j++)
		{
			if (!posList[j].dupe)
			{
				fprintf(ctl, " \n\t%d %d %d", posList[j].x, posList[j].y, posList[j].z);
			}
		}
		
		if (verbose && (points - x))
		{
			printf("Duplicated vertexes: %d ", points - x);
		}
		
		// Rewrite face table
		for (j = 0; j < faces; j++)
		{
			int m;

			for (m = 0; m < faceList[j].vertices; m++)
			{
				faceList[j].index[m] = posList[faceList[j].index[m]].reindex;
			}
		}

		if (verbose && (facesPIE3 - faces))
		{
			printf("Corrected 2faces: %d ", facesPIE3 - faces);
		}
		
		fprintf(ctl, "\nPOLYGONS %d", facesPIE3);
		for (j = 0; j < faces; j++)
		{
			int k, flags = iV_IMD_TEX;
			char fill[128];

			fill[0] = '\0';
			if (faceList[j].frames > 0)
			{
				flags |= iV_IMD_TEXANIM;
				sprintf(fill, " %d 1 %d %d", faceList[j].frames, faceList[j].width, faceList[j].height);
			}

			if (faceList[j].cull)
			{
				fprintf(ctl, "\n\t%x %d", flags, faceList[j].vertices);
				for (k = faceList[j].vertices - 1; k >= 0; k--)
					fprintf(ctl, " %d", faceList[j].index[k]);
				fprintf(ctl, "%s", fill);
				for (k = faceList[j].vertices - 1; k >= 0; k--)
					fprintf(ctl, " %d %d", faceList[j].texCoord[k][0], faceList[j].texCoord[k][1]);
			}
			fprintf(ctl, "\n\t%x %d", flags, faceList[j].vertices);
			for (k = 0; k < faceList[j].vertices; k++)
				fprintf(ctl, " %d", faceList[j].index[k]);
			fprintf(ctl, "%s", fill);
			for (k = 0; k < faceList[j].vertices; k++)
				fprintf(ctl, " %d %d", faceList[j].texCoord[k][0], faceList[j].texCoord[k][1]);
		}

		num = fscanf(fp, "\nCONNECTORS %d", &x);
		if (num == 1)
		{
			if (x < 0)
			{
				fprintf(stderr, "Bad CONNNECTORS directive in %s, level %d\n", input, level);
				exit(EXIT_FAILURE);
			}
			fprintf(ctl, "\nCONNECTORS %d", x);
			for (j = 0; j < x; ++j)
			{
				int a, b, c;

				num = fscanf(fp, "\n%d %d %d", &a, &b, &c);
				if (num != 3)
				{
					fprintf(stderr, "File %s. Bad CONNECTORS directive entry level %d, number %d\n", input, level, j);
					exit(1);
				}
				fprintf(ctl, "\n\t%d %d %d", a, b, c);
			}
			fprintf(ctl, "\n");
		}

		free(faceList);
		free(posList);
	}
}

int main(int argc, char **argv)
{
	FILE *p, *f;
	char buffer[1024];
	size_t rsize;

	parse_args(argc, argv);
	
	p = fopen(input, "r");
	if (!p)
	{
		fprintf(stderr, "Cannot open \"%s\" for reading: %s\n", input, strerror(errno));
		exit(1);
	}
	f = tmpfile();
	if (!f)
	{
		fprintf(stderr, "Cannot open temporary file for writing: %s\n", strerror(errno));
		exit(1);
	}

#ifdef _WIN32
	if (verbose)
	{
		// Should it be / on Linux?
		//char *fname = strrchr(input, '/');
		char *fname = strrchr(input, '\\');
		if (fname && *(fname + 1))
		{
			fname++;
		}
		else
		{
			fname = input;
		}
		printf("PIE: %s ", fname);
	}
#endif
	
	dump_to_pie(f, p);
	fclose(p);

	if (verbose)
	{
		printf("\n");
	}
		
	// Now copy temporary file to original
	rewind(f);
	p = fopen(input, "w");
	while (!feof(f) && !ferror(p) && !ferror(f))
	{
		rsize = fread(buffer, 1, sizeof(buffer), f);
		fwrite(buffer, rsize, 1, p);
	}
	if (ferror(f)) fprintf(stderr, "Error reading: %s\n", strerror(ferror(f)));
	if (ferror(p)) fprintf(stderr, "Error writing: %s\n", strerror(ferror(p)));

	fclose(p);
	fclose(f);	// also deletes temporary file

	return 0;
}
