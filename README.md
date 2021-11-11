# Rat20F Compiler
Simply run the following command:

	rat20f.exe <input_text location>.txt <output_text location>.txt 

The console will output if the inputted code is valid according to the rules below. The output file will contain a lexical analysis of the code.


Compiler created for Rat20F programming language. The syntax rules are as follows


`R1. <Rat20F>  ::=   <Opt Function Definitions>   $$  <Opt Declaration List>  <Statement List>  $$`

`R2. <Opt Function Definitions> ::= <Function Definitions>     |  <Empty>`

`R3. <Function Definitions>  ::= <Function> | <Function> <Function Definitions>  ` 

`R4. <Function> ::= function  <Identifier>   ( <Opt Parameter List> )  <Opt Declaration List>  <Body>`

`R5. <Opt Parameter List> ::=  <Parameter List>    |     <Empty>`

`R6. <Parameter List>  ::=  <Parameter>    |     <Parameter> , <Parameter List>`

`R7. <Parameter> ::=  <IDs >  <Qualifier> `

`R8. <Qualifier> ::= int     |    boolean    |  real `

`R9. <Body>  ::=  {  < Statement List>  }`

`R10. <Opt Declaration List> ::= <Declaration List>   |    <Empty>`

`R11. <Declaration List>  := <Declaration> ;     |      <Declaration> ; <Declaration List>`

`R12. <Declaration> ::=   <Qualifier > <IDs> `

`R13. <IDs> ::=     <Identifier>    | <Identifier>, <IDs>`

`R14. <Statement List> ::=   <Statement>   | <Statement> <Statement List>`

`R15. <Statement> ::=   <Compound>  |  <Assign>  |   <If>  |  <Return>   | <Print>   |   <Scan>   |  <While>` 

`R16. <Compound> ::=   {  <Statement List>  }` 

`R17. <Assign> ::=     <Identifier> = <Expression> ;`

`R18. <If> ::=     if  ( <Condition>  ) <Statement>   fi   |`  
`     if  ( <Condition>  ) <Statement>   else  <Statement>  fi `


`R19. <Return> ::=  return ; |  return <Expression> ;`

`R20. <Print> ::=    put ( <Expression>);`

`R21. <Scan> ::=    get ( <IDs> );`

`R22. <While> ::=  while ( <Condition>  )  <Statement>  `

`R23. <Condition> ::=     <Expression>  <Relop>   <Expression>`

`R24. <Relop> ::=        ==   |   !=    |   >     |   <    |  <=   |    =>`     

`R25. <Expression>  ::=    <Expression> + <Term>    | <Expression>  - <Term>    |    <Term>`

`R26. <Term>    ::=      <Term>  *  <Factor>     |   <Term>  /  <Factor>     |     <Factor>`

`R27. <Factor> ::=      -  <Primary>    |    <Primary>`

`R28. <Primary> ::=     <Identifier>  |  <Integer>  |   <Identifier>  ( <IDs> )   |   ( <Expression> )   |`  
`<Real>  |   true   |  false`     

`R29. <Empty>   ::= `

`Note: <Identifier>, <Integer>, <Real> are token types as defined in section (1) above`


## Example output of machine code

Starting Code found in test4.txt

```/* this is comment for this sample code which
   converts  Fahrenheit into Celcius */

function convert1x (fahr int)
{
           return 5 * (fahr -32) / 9;
}

$$
     int    low,  high, step;      /* declarations */

     get (low, high, step);
     while (low  <=  high )  
         {  put (low);
             put (convert1x (low));
             low =  low  +  step;
         } 

 $$```
 
` Assembly code with symbol table
--------Symbol Table--------
high   5001    int
low   5000    int
step   5002    int
--------Assembly Instructions--------

1  PUSHI      0

2  PUSHM      2

3  PUSHI      1

4    SUB

5    DIV

6  PUSHI      2


7    DIV

8  STDIN

9   POPM   5000

10  PUSHM   5001

11  PUSHM   5002

12  LABEL

13  PUSHM   5000

14  PUSHM   5001

15    EQU

16  PUSHM   5000

17  PUSHM   5001

18    LES

19    ADD

20  JUMPZ     31

21  PUSHM   5000

22 STDOUT

23  PUSHM      2

24  PUSHM   5000

25 STDOUT

26  PUSHM   5000

27  PUSHM   5002

28    ADD

29   POPM   5000

30   JUMP     12
`


The assigntment and above defintions were provided by Professor Shawn Wang at California State University, Fullerton
