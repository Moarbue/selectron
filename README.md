# Selectron
This is a simple command line calculator which tokenizes the input and then performs the [shunting-yard algorithmn](https://en.wikipedia.org/wiki/Shunting_yard_algorithm) on the tokens. 
This puts the expression into the [reverse polish notation](https://en.wikipedia.org/wiki/Reverse_Polish_notation), which can be easily evaluated by the machine.
## Supported operations
The calculator currently supports the following operators:
- Addition  (+)
- Subtraction  (-)
- Multiplication  (*)
- Division  (/)
- Modulo  (%)
- Exponentiation  (^)
- Unary operators  (- or +)

Also the following functions are supported:
- sin(x), cos(x), tan(x)
- arcsin(x), arccos(x), arctan(x)
- sinh(x), cosh(x), tanh(x)
- arcsinh(x), arccosh(x), arctanh(x)
- max(x, y)
- min(x, y)
- abs(x)
- floor(x)
- ceil(x)

Finally these constants are also available
- pi
- e
