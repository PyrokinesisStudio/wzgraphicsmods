/*
	This file is part of Warzone 2100.
	Copyright (C) 1999-2004  Eidos Interactive
	Copyright (C) 2005-2009  Warzone Resurrection Project

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
%{

/* include framework */
#include "lib/framework/frame.h"

#include <physfs.h>

#include "lib/gamelib/parser.h"

/* Get the Yacc definitions */
#include "audp_parser.tab.h"

/* Maximum length for any TEXT value */
#define YYLMAX	255

void audp_error(const char* fmt);

#include "lib/framework/lexer_input.h"

#ifndef yyextra
# define yyextra yyget_extra()
#endif

/* Older GNU Flex versions don't define yyget_extra(), yyset_extra(),
 * yyget_text() and yyget_lineno().
 * (and neither define a subminor version)
 */
#if !defined(YY_FLEX_SUBMINOR_VERSION) || (YY_FLEX_SUBMINOR_VERSION < 9)
# define yyget_extra  audp_get_extra
# define yyset_extra  audp_set_extra
# define yyget_lineno audp_get_lineno
# define yyget_text   audp_get_text
extern void yyset_extra(YY_EXTRA_TYPE user_defined);
extern YY_EXTRA_TYPE yyget_extra(void);
extern int yyget_lineno(void);
int yyget_lineno()
{
	return yylineno;
}

extern char* yyget_text(void);
char* yyget_text()
{
	return yytext;
}
#elif defined(YY_FLEX_SUBMINOR_VERSION) && YY_FLEX_SUBMINOR_VERSION == 33
extern YY_EXTRA_TYPE yyget_extra(void);
extern int audp_get_lineno(void);
extern FILE *audp_get_in(void);
extern FILE *audp_get_out(void);
extern int audp_get_leng(void);
extern char *audp_get_text(void);
extern void audp_set_lineno(int line_number);
extern void audp_set_in(FILE* in_str);
extern void audp_set_out(FILE* out_str);
extern int audp_get_debug(void);
extern void audp_set_debug(int bdebug);
extern int audp_lex_destroy(void);
extern void audp_set_extra(YY_EXTRA_TYPE user_defined);
#endif

%}

%option yylineno noyywrap nounput
%option prefix="audp_"

%x COMMENT
%x QUOTE
%x SLCOMMENT

%%
	/* Match to key words */
oneshot							{	return ONESHOT;			}
loop							{	return LOOP;			}
audio							{	return AUDIO;			}
anim3dfile						{	return ANIM3DFILE;		}
audio_module					{	return AUDIO_MODULE;	}
anim_module						{	return ANIM_MODULE;		}
ANIM3DFRAMES					{	return ANIM3DFRAMES;	}
ANIM3DTRANS						{	return ANIM3DTRANS;		}
ANIMOBJECT						{	return ANIMOBJECT;		}

	/* Match floating point numbers */
	/* This is a problem with the PSX so is disabled
-?[0-9]*"."[0-9]+				{	audp_lval.fval = (float) atof(yytext);
									return FLOAT_T;
								}
	*/

	/* Match integer numbers */
-?[0-9]+						{	audp_lval.ival = atoi(yytext);
									return INTEGER;
								}

	/* Match quoted text */
\"\"				{
					audp_lval.sval = strdup("");
					return QTEXT;
				}

\"				{ BEGIN QUOTE;   }
<QUOTE>\"			{ BEGIN INITIAL; }
<QUOTE>\n			{ audp_error("Unexpected end of line in string"); }
<QUOTE>[^\"\n]*			{
					audp_lval.sval = strdup(yytext);
					return QTEXT;
				}

	/* Skip white space */
[ \t\n\x0d\x0a]						;

	/* Strip comments */
"/*"				{ BEGIN COMMENT; }
<COMMENT>"*/" |
<COMMENT>"*/"\n			{ BEGIN INITIAL; }
<COMMENT>. |
<COMMENT>\n			;

	/* Strip single line comments */
"//"				{ BEGIN SLCOMMENT; }
<SLCOMMENT>\n			{ BEGIN INITIAL;   }
<SLCOMMENT>[^\n]*		;

	/* Match anything that's been missed and pass it as a char */
.							return yytext[0];


%%

static YY_EXTRA_TYPE pBuffer = NULL;

void yyset_extra(YY_EXTRA_TYPE user_defined)
{
	pBuffer = user_defined;
}

YY_EXTRA_TYPE yyget_extra()
{
	return pBuffer;
}

/* Older GNU Flex versions don't define yylex_destroy()
 * (and neither define a subminor version)
 */
#if !defined(YY_FLEX_SUBMINOR_VERSION) || (YY_FLEX_SUBMINOR_VERSION < 9)
int audp_lex_destroy(void)
{
	/* For non-reentrant C scanner only. */
	yy_delete_buffer(YY_CURRENT_BUFFER);
	yy_init = 1;
	return 0;
}
#endif
