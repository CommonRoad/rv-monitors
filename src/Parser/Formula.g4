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
| <assoc=right> left=unary op=RELEASE iv=interval right=unary             #ReleaseInterval
| <assoc=right> left=unary op=UNTIL iv=interval right=unary               #UntilInterval
| unary                                                                #UnaryExpr
;

unary
: op=GLOBALLY right=atom                         #Globally
  | op=FUTURE right=atom                         #Future
  | op=ONCE right=atom                           #Once
  | op=PREVIOUSLY right=atom                     #Previously
  | op=NEXT_WEAK right=atom                      #NextWeak
  | op=NEXT_STRONG right=atom                    #NextStrong
  | op=GLOBALLY iv=interval right=atom           #GloballyInterval
  | op=FUTURE iv=interval right=atom             #FutureInterval
  | op=ONCE iv=interval right=atom               #OnceInterval
  | op=PREVIOUSLY iv=interval right=atom         #PreviouslyInterval
  | op=NEXT_WEAK iv=interval right=atom          #NextWeakInterval
  | op=NEXT_STRONG iv=interval right=atom        #NextStrongInterval
  | op=NOT right=atom                            #Not
  | op=NOT right=expr                            #NotExpr
  | atom                                         #AtomExpr
 ;

interval
: '[' l=number ',' u=number ']'      #FiniteInterval
| '[' l=number ',' u='inf' ']'       #InfiniteInterval
;

atom
 : BOOL                         #Boolean
 | ATOMIC                       #Atomic
 | '(' content=expr+ ')'        #Brackets
 ;

number
 : DIGIT                 #Digit
 ;

 BOOL		                : TRUE|FALSE;
 NOT                        : ('!'|'not');
 GLOBALLY	                : 'G';
 FUTURE		                : 'F';
 NEXT_WEAK	                : 'W';
 NEXT_STRONG	            : 'X';
 UNTIL		                : 'U';
 RELEASE		            : 'R';
 ONCE                       : 'O';
 PREVIOUSLY                 : 'P';
 AND        	            : ('&'|'and');
 OR         	            : ('|'|'or');
 XOR                        : ('^'|'xor');
 IMPL       	            : '->';
 EQ                         : '<->';
 TRUE       	            : ('True'|'true');
 FALSE      	            : ('False'|'false');
 ATOMIC		                : [a-z]+([_]?[a-z]+)*;
 WS                         : [ \t\n]+ -> skip;       //Skip whitespaces
 DIGIT                      : [0-9] | [1-9]+;

