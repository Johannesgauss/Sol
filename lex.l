%{
int count = 0;
%}

%%
[A-Z] {printf("%s capital letter\n", yytext);
       count++;}
.     {printf("%s not a capital letter\n", yytext);}
\n    {return 0;}
%%

int yywrap(){}
int main(int argc, char **argv)
{

// Explanation:
// yywrap() - wraps the above rule section
/* yyin - takes the file pointer 
          which contains the input*/
/* yylex() - this is the main flex function
          which runs the Rule Section*/
// yytext is the text in the buffer

// Uncomment the lines below 
// to take input from file
    yyin = fopen(argv[1],"r");
    yylex();

    return 0;
}
