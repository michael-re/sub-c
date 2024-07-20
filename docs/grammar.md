# Sub-C Grammar

```txt
program     => function+

function    => "int"  name  "(" parameters ")" body
             | "int" "main" "("            ")" body

parameters  => ( parameter ( "," parameter )* ) ?
parameter   => "int" name

body        => "{" variable* statement+ "}"
variable    => "int" name ";"
statement   => assign
             | block
             | if
             | return
             | while

assign      => name "="  ( expression | call ) ";"
if          => "if"     "(" expression ")" block
return      => "return"     expression     ";"
while       => "while"  "(" expression ")" block

block       => "{" statement+ "}"
primary     => name    | number
expression  => primary | binary
binary      => primary operator primary
operator    => "+"
             | "-"
             | "*"
             | "<"
             | ">"
             | "=="
             | "!="
             | "<="
             | ">="

call        => name "(" args ")"
args        => ( arg ( "," arg )* ) ?
arg         => name | number | string

number      => [0-9]+
string      => "\"" non_quotes "\""
name        => alpha alnum*
alpha       => [a-za-z] | "_"
alnum       => alpha    | num
```
