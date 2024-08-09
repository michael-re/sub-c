# Sub-C Grammar

- **Built-in Output Functions**:
  - `__says`: Print null-terminated string to `stdout`.
  - `__sayn`: Print integer to `stdout`.
  - `__sayl`: Print newline to `stdout`.

```txt
program     => function+

function    => "int"  name  "(" parameters ")" body
             | "int" "main" "("            ")" body

parameters  => ( parameter ( "," parameter )* ) ?
parameter   => "int" name

body        => "{" variable* statement+ "}"
variable    => "int" name ";"
statement   => block
             | expr_stmt
             | if
             | return
             | while

expr_stmt   => call | assign
assign      => name "="  ( expression | call ) ";"
if          => "if"     "(" expression ")" statement
return      => "return"     expression     ";"
while       => "while"  "(" expression ")" statement

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
