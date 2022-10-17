# dotkoma
dotkoma is a simple interpreter for dotkoma programming language.

## dotkoma syntax structure (EBNF)
```
dotkoma  = ["#!" {ANY} newline]                   // shebang compatibility
           { (instruct) | (heredoc newlline) } .  // series of instructions or heredoc
           
heredoc  = ".,"  {ANY} ".," .                     // heredoc caged using dot and comma

instruct = ".sh" ident {ANY} newline.             // shell instruction starts with ".sh", end with newline
```
## compile from source
cd to the src directory and run gcc
```
cd src
gcc -o dk *.c
```
this will generate an executable file called `dk`, short for DotKoma.

## running dotkoma
to use dotkoma, just run the executable followed by the filename
```
./dk <filename>
```
you can add `dk` executable into PATH folder (usually /usr/bin or /usr/local/bin) and you can execute `dk` from anywhere in your system
```
dk <filename>
```
you can also run `dk` as shell script by adding the first line of dotkoma file with `#!<path to dk>`. For example, `dk` is installed in `/usr/local/bin`, so you can create a new file, `test.dk` with this content
```
#!/usr/local/bin/dk
.,
<html>
<title>This is a Test</title>
<body><h1>Hello World</h1></body>
</html>
.,
```
modify the permission of this file to allow execution. Now to execute `test.dk` just run this command
```
./test.dk
```
## hello world in dotkoma
```
.,Hello World.,
```
## more example
print list of file in current folder. save as `list.dk`
```
#!/usr/local/bin/dk
.,
<html>
<title>Files in folder</title>
<body>
<h1>File list</h1>
<h2><pre>
.,
.sh ls
.,
</pre></h2>
</body>
</html>
.,
```
run with this command
```
./list.dk > list.html
```
open `list.html` with your favorite web browser

## syntax highlighting in Notepad++
included in this repository is the `dotkoma.xml` file in `npp-UDL` folder. This is a syntax highligther file (Notepad++ XML User Defined Language) for DotKoma language. Sample screenshot:

![image](https://scontent-kut2-2.xx.fbcdn.net/v/t39.30808-6/307712624_10225536774900226_5970646952773867353_n.jpg?_nc_cat=107&ccb=1-7&_nc_sid=730e14&_nc_ohc=oJIH4SyPugwAX_cKBbA&_nc_ht=scontent-kut2-2.xx&oh=00_AT8ISeQkTcxsUa48RJkTOHYJrk9zrRCEtxIouO9Cy_NH1w&oe=6351EA1C)

# license
  Copyright (C) 2022 Mohd Kholid Yaacob (<http://mrharmonies.blogspot.com>)
  
  This source is free software; you can redistribute it and/or modify it under
  the terms of the GNU General Public License as published by the Free
  Software Foundation; either version 2 of the License, or (at your option)
  any later version.
  
  
  This code is distributed in the hope that it will be useful, but WITHOUT ANY
  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
  FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
  details.
  
  
  A copy of the GNU General Public License is available on the World Wide Web
  at <http://www.gnu.org/copyleft/gpl.html>. You can also obtain it by writing
  to the Free Software Foundation, Inc., 51 Franklin Street - Fifth Floor,
  Boston, MA 02110-1335, USA. 
