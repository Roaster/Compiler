# Rat20F Compiler
Simply run the command 
	```console 
	Syntax.exe input_text.txt output_text.txt 
	```
The console will output if the inputted code is valid according to the rules below. The output file will contain a lexical analysis of the code.


Compiler created for Rat20F programming language. The syntax rules are as follows

R1. <Rat20F>  ::=   <Opt Function Definitions>   $$  <Opt Declaration List>  <Statement List>  $$
R2. <Opt Function Definitions> ::= <Function Definitions>     |  <Empty>
R3. <Function Definitions>  ::= <Function> | <Function> <Function Definitions>   
R4. <Function> ::= function  <Identifier>   ( <Opt Parameter List> )  <Opt Declaration List>  <Body>
R5. <Opt Parameter List> ::=  <Parameter List>    |     <Empty>
R6. <Parameter List>  ::=  <Parameter>    |     <Parameter> , <Parameter List>
R7. <Parameter> ::=  <IDs >  <Qualifier> 
R8. <Qualifier> ::= int     |    boolean    |  real 
R9. <Body>  ::=  {  < Statement List>  }
R10. <Opt Declaration List> ::= <Declaration List>   |    <Empty>
R11. <Declaration List>  := <Declaration> ;     |      <Declaration> ; <Declaration List>
R12. <Declaration> ::=   <Qualifier > <IDs>                   
R13. <IDs> ::=     <Identifier>    | <Identifier>, <IDs>
R14. <Statement List> ::=   <Statement>   | <Statement> <Statement List>
R15. <Statement> ::=   <Compound>  |  <Assign>  |   <If>  |  <Return>   | <Print>   |   <Scan>   |  <While> 
R16. <Compound> ::=   {  <Statement List>  } 
R17. <Assign> ::=     <Identifier> = <Expression> ;
R18. <If> ::=     if  ( <Condition>  ) <Statement>   fi   |   
                          if  ( <Condition>  ) <Statement>   else  <Statement>  fi 
R19. <Return> ::=  return ; |  return <Expression> ;
R20. <Print> ::=    put ( <Expression>);
R21. <Scan> ::=    get ( <IDs> );
R22. <While> ::=  while ( <Condition>  )  <Statement>  
R23. <Condition> ::=     <Expression>  <Relop>   <Expression>
R24. <Relop> ::=        ==   |   !=    |   >     |   <    |  <=   |    =>        
R25. <Expression>  ::=    <Expression> + <Term>    | <Expression>  - <Term>    |    <Term>
R26. <Term>    ::=      <Term>  *  <Factor>     |   <Term>  /  <Factor>     |     <Factor>
R27. <Factor> ::=      -  <Primary>    |    <Primary>
R28. <Primary> ::=     <Identifier>  |  <Integer>  |   <Identifier>  ( <IDs> )   |   ( <Expression> )   |  
                                     <Real>  |   true   |  false                        
R29. <Empty>   ::= 

Note: <Identifier>, <Integer>, <Real> are token types as defined in section (1) above


The assigntment and above defintions were provided by Professor Shawn Wang at California State University, Fullerton