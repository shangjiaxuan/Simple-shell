# Road map

- [x] A feature that has been completed
- [ ] A feature that has NOT yet been completed

# Change log

These are the changes to each version that has been tagged and released on Github.
Prior versions before the cleanup of `v0.0.1.2` are described within the scope of `v0.0.1.2` 

## v0.0.2.0
  -- Planned
- [x] Simple command parsing
  - [ ] Parsing complex commands
  - [ ] Command parsing with custom algorithm to avoid multiple "if"s and string comparisons
- [ ] Encoding support
  - [ ] chcp command
  - [ ] Unicode support
    - [x] UTF-16 coding in non-io functions and conversion in IO functions
    - [ ] Default UTF-8 code page
- [x] Dynamic loading library
  - [x] Example function calling in dll improved
  - [x] Launching arbitrary functions with default parameters 
  - [x] SEH with loading arbitrary dll functions (normally wrong types for parameters)
- [x] Launching other applications
  - [x] Launch with command line arguments
  - [ ] (To be added) Launch with process parameters
  - [ ] (To be added) Open files with default programs
- [x] Follow shell links
  - [x] Open programs with shell links
    - [ ] Pass command line parameters to targets
- [ ] IO manipulations
  - [ ] Filesystem related (using c++17 standard library)
    - [x] cd command
    - [ ] dir and ls commands
    - [ ] mklink command
- [ ] Manual and documentation
  - [x] Proxy manual function
  - [ ] Manual viewing system
    
## v0.0.1.2

- [x] Initial release
- [x] Support 
- [x] exit command
- [x] cd command
  - [x] cd supports switching drive letters in Windows
- [x] Dynamic loading library
  - [x] Example function calling in dll
  - [ ] Launching arbitrary functions with default parameters 
- [x] Launching other applications
  - [ ] Launch with commandline arguments
  - [ ] (To be added) Launch with process parameters
  - [ ] (To be added) Open files with default programs
- [ ] Unicode support
  - [x] UTF-16 coding in non-io functions and conversion in IO functions
- [ ] (To be added) Support for OS's other than Windows
  - [x] Macros for keeping Windows specific code seperable
- [ ] (To be added) Manual and documentation
  - [x] Proxy manual function
