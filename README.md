# excellang - A language made to be like Excel

# 1. Introduction to excellang
We all are familiar with Excel, a spreadsheet app, even if it is in Windows, Linux or in Macbook, we all have heard about it. If you live under a rock, then search up for yourself before coming back...
Excellang (or .elg) is a programming language which has the syntax almost similar to Excel. And as a wise man said: "What is an “excel like language”? E1:E3 = 87", yeah, it's that...

# 2. Variables
A variable name requires a column and a row. A column is a bunch of alphabetical letters (at least 1 letter), and a row is simply a positive integer.

Examples of variables are: `A1`, `Discord5`, `Scratch3`,... But `1A` is not a variable, the column has to go in front of the row.

To set a value to a variable, you do:
```
<variable_name> = <value>
```

Please do note that variable names are **not** case-sensitive, which means that `A1` and `a1` are the same variable and so does `discord2` and `DIScord2`. You can think of each variable as a cell of an Excel sheet (this will be useful later)

# 3. Ranges
A range has a syntax as follows: `variable1:variable2`. What this means is that a range basically represents a large data between 2 corners to form a rectangle.

To set a value for a range, you do:
```
<range_name> = <value>
```

# 4. Operations and data types
This language supports common operations you see on Excel: `+`, `-`, `*` and `/`. And this language also only supports 2 data type: `number` and `string`. But from these 2 data types, I am sure that you can try to replicate other data types like booleans or arrays using these data types.

# 5. Flags
In each cell, there are also 3 flags: bold, italiac, and underline. These flags can also be turned on or off, which can also help you a lot in coding. Anyone who is familiar with making compilers should be able to understand what I am saying...

To set a flag for a variable, you do:
```
flag <flag_name (bold/italiac/underline)> <variable_name> = <true/false>
```

To set a flag for a range, you do:
```
flag <flag_name (bold/italiac/underline)> <range_name> = <true/false>
```

Please note that flags **are** case-sensitive, the keyword `flag`, `true` and `false` are also case-sensitive.
