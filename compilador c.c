#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#define MAXNAME 30
#define MAXNUM 5

char look;

void init();

void nexChar();
void error(char *fmt, ...);
void fatal (char *fmt, ...);
void expected (char *fmt, ...);
void skipwhite();
void match (char c);
void getName (char *name);
void getNum (char *num);
void emit (char *fmt, ...);

int isAddOp(char c);

void ident();
void factor();
void multiply();
void divide();
void term();
void add();
void subtract();
void expression();
void assignment();

int main()
{
	init();
	    assignment();
	    if(look != '\n')
	       expected("NewLine");
	       
	return 0;
}

void init()
{
	nextChar();
  skipwhite();
}

void nextChar()
 {
 	look = getchar();
 }    
 
 void error(char *fmt, ...)
 {
 	va_list args;
 	
 	fputs("Error: ", stderr);
 	
 	va_start(args, fmt);
 	vfprintf(stderr, fmt, args);
 	va_end(args);
	 
	 fputc('\n', stderr); 	
 }

void fatal(char *fmt, ...)
{
	va_list args;
	
	fputs("Error:  ", stderr);
	
	va_start(args, fmt);
 	vfprintf(stderr, fmt, args);
 	va_end(args);
 	
 	fputc('\n', stderr); 	
 	
 	exit(1);
}

void expected(char *fmt, ...)
 {
 
     va_list args;
     
    fputs("Error:  ", stderr);
	
	va_start(args, fmt);
 	vfprintf(stderr, fmt, args);
 	va_end(args);
 	
 	fputs(" expected!\n", stderr);
 	
 	exit (1);
}

void skipwhite()
{
	
	while (look == ' ' || look == '\t' )
	     nextChar();
}
void match(char c)
{
	if(look != c)
	   expected("'%c'", c);
	nextChar();
	skipwhite();
	
}

void getName(char *name)
{
	int i;
	
    if(!isalpha(look))
    expected("Name");
    
    for (i=0; isalnum(look); i++) {
    	if (i >= MAXNAME)
    	   fatal("Identifier too long!");
    	name[i] = toupper(look);
    	nextChar();
	}
	name[i] = '\0';
	skipwhite();
	
}

void getNum(char *num)
{
	  int i;
	  
	if(!isdigit(look))
	   expected("integer");
	   
	for (i = 0; isdigit(look); i++) {
		if (i >= MAXNUM)
		   fatal("integer too long!");
		num[i] = look;
		nextChar();
	}
	num[i] = '\0';
	skipwhite();
}

void emit(char *fmt, ...)
{
	va_list args;
	
	putchar('\t');
	
	va_start(args, fmt);
	vprintf(fmt, args);
	va_end(args);
	
	putchar('\n');
}

int isAddOp (char c)
{
return (c == '+' || c == '-');	
}
void ident()
{
	char name[MAXNAME+1];
	
	getName(name);
	if (look == '(') {
		     match ('(');
		     match(')');
		     emit("CALL %s", name);
	} else
	emit("MOV AX, [%s]", name);
}

void factor ()
{
	char num [MAXNUM+1];
	
	if (look == '(') {
		match('(');
		expression ();
		match(')');
		
	} else {
		      getNum(num);
		    emit("MOV AX, %s", num);
	}
}

void multiply ()
{
	match('*');
	factor();
	emit("POP BX");
	emit("IMUL BX");
	
}

void divide ()
{
	match('/');
	factor();
	emit("POP BX");
	emit("XCHG AX, BX");
	emit("CWD");
	emit("IDIV BX");
}

void term()
{
	factor();
	while (look == '*' || look == '/') {
		emit ("PUSH AX");
		switch(look) {
			case '*':
			  multiply();
			  break;
			case '/':
			divide();
		    	break;
	    	}
    	}
	}
	
	void add()
	{
		match('+');
		term();
		emit("POP BX");
		emit("ADD AX, BX");
	}
	void subtract ()
	{
		match('-');
		term();
		emit("POP BX");
		emit("SUB AX, BX");
		emit("NEG AX");
		
	}
	
	void expression()
	{
		if (isAddOp(look)) 
		       emit("XOR AX, AX");
		else 
		     term();
		while (isAddOp(look)) {
			emit ("PUSH AX");
			switch (look){
				case '+':
					add();
					break;
			    case '-':
			    	subtract();
			    	break;
			}
		}
	}
	
	void assignment()
	{
		char name[MAXNAME+1];
		
		getName(name);
		match('=');
		expression();
		emit("MOV [%s], AX", name);
	}
	
