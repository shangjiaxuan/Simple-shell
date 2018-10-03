# Simple shell

[![Build status](https://ci.appveyor.com/api/projects/status/4ioxd82h1l91yoqd?svg=true)](https://ci.appveyor.com/project/shangjiaxuan/simple-shell)

#### An attempt started from a specific console application to create a console environment that works like a cmd or shell but can provide much greater extensibility
e.g. support some UNIX commands, 'cd'ing directly to other drives(multiple root currently NOT supported), default loading functions in dlls (`as void function()`or as `int function(int, char**)`), default `UTF-8` codepage (in the near future), etc...

## General Information

##### This is a toy project under development that currently only support windows. Will add a few macros to define behaviors under other platforms. (And perhaps change the prompt format for UNIX-like ones.)

### Building from source

* Use Visual Studio 2017 with VC++, C++17 support and you will fine (You may need to retarget the solution corresponding to your SDK version, though...). 

* Note that you may need to compile and run`CmdTreeGen.exe` project before building and running the main program in future code(to create a fixed commandline parsing struct in code).

* Other compilers may or may not work with the code. File a issue if you want your compiler supported, but may not be attended until the program reaches a stable stage.

* Please note that there's a `__try{}__except(){}` block in LoadLibrary functions, you may need to redefine behavior(possibly using a macro to not compile the code that involve default calling of library functions and use c++ try block insead of a SEH error handling block) if you use a compiler that does not support it. 

* Also, `_WINDLL`, `_WIN32`, `_UNICODE`, `_MBCS` macros are used in the program, please adapt those macros and codes if your development environment does not use them

### Contributing guidelines

* Please be noted that the main goal right now is to make an extensible host from which other functions can be called. From this perspective, please try to use Unicode core for Windows.

*  Becase of the possibility of porting to other platforms, please try to put platform specific code together so that they can easily be changed using a macros.

## License

* This project's code by it self if free to use in any regard without warranty, but code that was referenced or included may not belong to this project. These code inclue but are not limited to the original calculator-3.0 by my teacher and various stackoverflow(licensed under `CC BY-SA 3.0` if not specified), MSDN (licensed under `Ms-LPL` if not specified) and other forum topic and reference pages where I looked for the usage of certain Windows API calls.(Note that my code is licensed under `MPL-2.0`, but I guess I won't do anything if someone violates it with MY code without causing too much harm...)

* Please note that when developing this program, I was using student version of certaion tools (Including but not limited to ReSharper's plugin and Vsiual Studio community). Thus, if you intend to use it for commercial ends, please file an issue before proceeding. 
(According to license terms up to Oct 4th, 2018, there is no issue here......)

### End remark

###### Windows and other trademarks are propriety of corporations that they are licensed to and are not related to this code or project except that they may be the platform(s), forum(s) or tool(s) that this project is developed with.
