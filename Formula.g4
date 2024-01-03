grammar Formula;

formula: expr;
expr
 : <assoc=right> left=expr op=IMPL right=expr   #Implication
 | <assoc=right> left=expr op=EQ right=expr     #Equivalence
 |  xor_expr                                    #XorExpr
 ;

xor_expr
 : or_expr (op=XOR or_expr)*                    #Xor
 ;

or_expr
 : and_expr (op=OR and_expr)*                   #Or
 ;

and_expr
 : release_until_expr (op=AND release_until_expr)*  #And
 ;


release_until_expr
: <assoc=right> left=unary op=RELEASE right=unary                      #Release
| <assoc=right> left=unary op=UNTIL right=unary                        #Until
| <assoc=right> left=unary op=RELEASE iv=interval right=unary          #ReleaseInterval
| <assoc=right> left=unary op=UNTIL iv=interval right=unary            #UntilInterval
| <assoc=right> left=unary op=SINCE right=unary                        #Since
| <assoc=right> left=unary op=SINCE iv=interval right=unary            #SinceInterval
| unary                                                                #UnaryExpr
;

unary
: op=GLOBALLY right=atom                                                       #Globally
  | op=FUTURE right=atom                                                       #Future
  | op=ONCE right=atom                                                         #Once
  | op=PREVIOUSLY right=atom                                                   #Previously
  | op=NEXT_WEAK right=atom                                                    #NextWeak
  | op=NEXT_STRONG right=atom                                                  #NextStrong
  | op=GLOBALLY iv=interval right=atom                                         #GloballyInterval
  | op=FUTURE iv=interval right=atom                                           #FutureInterval
  | op=ONCE iv=interval right=atom                                             #OnceInterval
  | op=PREVIOUSLY iv=interval right=atom                                       #PreviouslyInterval
  | op=NEXT_WEAK iv=interval right=atom                                        #NextWeakInterval
  | op=NEXT_STRONG iv=interval right=atom                                      #NextStrongInterval
  | op=HISTORICALLY right=atom                                                 #Historically
  | op=HISTORICALLY iv=interval right=atom                                     #HistoricallyInterval
  | op=NOT right=atom                                                          #Not
  | op=NOT right=expr                                                          #NotExpr
  | atom                                                                       #AtomExpr
  | op=FUNCTION_NAME right=(ATOMIC|CONST_VALUE) (',' (ATOMIC|CONST_VALUE))*')' #Predicate
  | op=UNIVERSAL dv=domain_vars '.' right=expr                                 #Universal
  | op=EXISTENTIAL dv=domain_vars '.' right=expr                               #Existential
  | op=QUOTE piv=quote_interval dv=domain_vars '.' right=expr                  #Quote
  | op=COUNTING cop=('<='|'='|'>=') num=DIGIT dv=domain_vars '.' right=expr    #Counting
 ;

interval
: '[' l=number ',' u=number   ']'       #FiniteInterval
| '[' l=number ',' u=INFINITY ']'       #InfiniteInterval
;

quote_interval
 : '[' l=quote_number ',' u=quote_number ']'    #FiniteQuoteInterval
 ;

domain_vars
 : ATOMIC (',' ATOMIC)* IN domain_value (',' ATOMIC (',' ATOMIC)* IN domain_value)*   #DomainVariables
 ;

domain_value
 : DOMAIN #StringDomain
 | FUNCTION_NAME right=(ATOMIC|CONST_VALUE) (',' (ATOMIC|CONST_VALUE))*')' #FunctionDomain
 ;

atom
 : bool_                                           #Boolean
 | ATOMIC                                          #Atomic
 | '(' content=expr+ ')'                           #Brackets
 ;

number
 : DIGIT                 #Digit
 ;

quote_number
 : QUOTE_DIGIT            #QuoteDigit
 ;

bool_
  : TRUE                 # TrueBoolean
  | FALSE                # FalseBoolean
  ;

 TRUE                       : ('True'|'true');
 FALSE                      : ('False'|'false');

 INFINITY                   : ('inf'|'INFINITY');
 NOT                        : ('!'|'not'|'NOT');

 GLOBALLY                   : 'G';
 FUTURE                     : 'F';
 NEXT_WEAK                  : 'W';
 NEXT_STRONG                : ('X'|'NEXT');
 UNTIL                      : ('U'|'UNTIL');
 RELEASE                    : 'R';
 ONCE                       : 'O';
 PREVIOUSLY                 : ('P'|'PREV');
 SINCE                      : ('S'|'SINCE');
 HISTORICALLY               : 'H';

 UNIVERSAL                  : ('A'|'ALL');
 EXISTENTIAL                : ('E'|'EXISTS');
 QUOTE                      : ('Q'|'QUOTE');
 COUNTING                   : ('C'|'COUNT');

 AND        	            : ('&'|'and'|'AND');
 OR         	            : ('|'|'or'|'OR');
 XOR                        : ('^'|'xor');
 IMPL       	            : '->';
 EQ                         : '<->';

 IN                         : 'in';


 // *********
 // IMPORTANT
 // *********
 //
 // Synchronize changes of LOWER_ALPHA_UNDERSCORE with the Log grammer (in Log.g4)!
 //
 fragment LOWER_ALPHA_UNDERSCORE : [a-z]+([_]?[a-zA-Z0-9]+)*;

 ATOMIC		                : LOWER_ALPHA_UNDERSCORE;
 FUNCTION_NAME                   : LOWER_ALPHA_UNDERSCORE'(';
 DOMAIN                     : [A-Z]+[A-Z]*[0-9]*;
 CONST_VALUE                : '\'' [a-zA-Z0-9]([_]?[a-zA-Z0-9]+)* '\'';

 WS                         : [ \t\n]+ -> skip;       //Skip whitespaces

 DIGIT                      : [0-9] | [1-9][0-9]+;
 QUOTE_DIGIT                : [0] '.' [0-9]* | [1] '.' [0]*;