%{
#include <stdio.h>
#include "y.tab.h"
%}
%%



\= { return EQ; }

\#[^\n] { return COMMENT; }

\<bg\> { return BG; }

^defprompt { return DEFPROMPT; }

^cd { return CD;  }

^run { return RUN; }

^listjobs { return LISTJOBS;  }

^bye { return BYE;  }

^assignto { return ASSIGNTO;  }

\".+\" {
	yylval.str = strdup(yytext);
	return STRING;
}

\$[A-Z0-9a-z]+ {
	yylval.str = strdup(yytext);
	return VARIABLE;
}

[^ \n\t=\#]+ {
	yylval.str = strdup(yytext);
	return WORD;
}

%%

