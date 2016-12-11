%{
#include <readline/readline.h>
#include <readline/history.h>
#include "cpu_sim.h"
using namespace std;

class commandFlexLexer : public yyFlexLexer {
public:
	char *reading_point;

	commandFlexLexer();
	int yylex();
	size_t LexerInput(char *buf, size_t max_size);
};

commandFlexLexer::commandFlexLexer() {
	reading_point = NULL;
}

size_t commandFlexLexer::LexerInput(char *buf, size_t max_size) {
	if (reading_point == NULL) return 0;
	int i;
	for (i=0; i<max_size && *reading_point; i++) {
		buf[i] = *reading_point;
		reading_point++;
	}
	return i;
}

void show_help() {
	fprintf(stderr,
		"commands:\n"
		"\tb             show all breakpoints\n"
		"\tb <numbers>   set breakpoints at <numbers>\n"
		"\tc             continue execution\n"
		"\tp             print registers\n"
		"\ts             step execution\n"
		"if you simply type ENTER, the previous command will be repeated.\n"
	);
}
%}



%option noyywrap
%option c++
%option yyclass="commandFlexLexer"

%x BREAKPOINT
%x BREAKPOINT_SET
%x CONTINUE
%x PRINT
%x STEP
%x INVALID



%%
<INITIAL>{
	"b" BEGIN(BREAKPOINT);
	"c" BEGIN(CONTINUE);
	"p" BEGIN(PRINT);
	"s" BEGIN(STEP);
}

<BREAKPOINT,BREAKPOINT_SET>{
	0|[1-9][0-9]* {
		int breakpoint = atoi(yytext);
		if (breakpoint <= lastpc) {
			breakpoints.insert(breakpoint);
		} else {
			fprintf(stderr, "%d is out of range (max PC is %d)\n", breakpoint, lastpc);
		}
		BEGIN(BREAKPOINT_SET);
	}
}

<INITIAL,INVALID><<EOF>> {
	show_help();
	return 1;
}
<BREAKPOINT><<EOF>> {
	for(set<int>::iterator i=breakpoints.begin(); i!=breakpoints.end(); i++) {
		fprintf(stderr, "breakpoint %d\n", *i);
	}
	return 1;
}
<BREAKPOINT_SET><<EOF>> {
	return 1;
}
<CONTINUE><<EOF>> {
	mode = MODE_CONTINUE;
	return 0;
}
<PRINT><<EOF>> {
	print_reg();
	return 1;
}
<STEP><<EOF>> {
	mode = MODE_STEP;
	return 0;
}

<*>[[:blank:]]
<*>. BEGIN(INVALID);
%%



void command() {
	int ret;
	do {
		commandFlexLexer lexer;
		char *line = readline(">>> ");
		if (line == NULL) {
			fprintf(stderr, "\nstdin EOF detected\n");
			exit(1);
		}
		if (*line && (current_history() == NULL || strcmp(line, current_history()->line))) {
			add_history(line);
		}
		free(line);
		if (current_history() != NULL) {
			lexer.reading_point = (char*) current_history()->line;
		}
		ret = lexer.yylex();
	} while (ret);
}
