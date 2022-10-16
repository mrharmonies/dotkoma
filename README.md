# dotkoma
dotkoma is a simple interpreter for dotkoma programming language.

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
print list of file in current folder
```
#!/usr/local/bin/dk
.,
<html>
<title>Files in folder</title>
<body>
<h1>File list</h1>
<h2>
.,
.sh ls
.,
</h2>
</body>
</html>
.,
```
