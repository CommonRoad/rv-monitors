grammar Log;

log : first=entry (ENTRY_SEPARATOR it=entry)* ENTRY_SEPARATOR? EOF;

entry : time identifier* ;

identifier: ATOMIC;

time : TIMESPEC DIGIT;


 // *********
 // IMPORTANT
 // *********
 //
 // Synchronize changes of LOWER_ALPHA_UNDERSCORE with the Formula grammer (in Formula.g4)!
 //
 fragment LOWER_ALPHA_UNDERSCORE : [a-z]+([_]?[a-zA-Z0-9]+)*;

 ATOMIC		                : LOWER_ALPHA_UNDERSCORE;
 PREDICATE                  : LOWER_ALPHA_UNDERSCORE'(';
 DIGIT                      : [0-9] | [1-9][0-9]+;


 ENTRY_SEPARATOR            : [\n]+;

 WS                         : [ \t]+ -> skip;

 TIMESPEC : '\u0040';
