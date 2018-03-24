# Calculator

#### An attempt started from a specific console application to create a console environment that works like a cmd or bash shell but can provide much greater extensibility. 

## General Information

##### This is a toy project under development that currently only support windows because of a few Windows API calls to launch another program. Will add a few macros to define behaviors under other platforms. (And perhaps change the prompt format for UNIX-like ones.) *

### Building from source

* Use Visual Studio 2017 with VC++, C++17 support and you will fine. Other compilers may or may not work with the code. File a issue if you want your compiler supported, but may not be attended until the program reaches a stable stage.

### Contributing guidelines

* Please be noted that the main goal right now is to make an extensible host from which other functions can be called. From this perspective, please try to use Unicode core for Windows.

*  Becase of the possibility of porting to other platforms, please try to put platform specific code together so that they can easily be changed using a macros.

## License

* This project's code by it self if free to use in any regard without warranty, but code that was referenced or included may not belong to this project. These code inclue but are not limited to the original calculator-3.0 by my teacher and various MSDN topic and reference pages where I looked for the usage of certain Windows API calls.

* Please note that when developing this program, I was using student version of certaion tools (Including but not limited to ReSharper's plugin and Vsiual Studio community). Thus, if you intend to use it for commercial ends, please contact file a issue before proceeding.

### End remark

###### Windows and other trademarks are propriety of corporations that they are licensed to and are not related to this code or project except that they may be the platform that this project is developed with.
