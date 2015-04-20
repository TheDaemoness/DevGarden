## Indentation and Braces
* Tabs must be used for primary indentation, and spaces for alignment.
* If a statement spans multiple lines, each line must have exactly the same number of tabs as the opening line.
* Control flow statements with only one line should not use braces.
* Inline member functions with only one statement should be declared and defined on the same line.
* Braces may be either on their own line or at the end of a line.
* Please don't put spaces before parentheses unless aligning code.
* Empty lines may be used to better group statements or declarations in code.
* All files should have a single empty line at the end, where possible.

## Naming
* Macro names should be `UPPER_CASE`.
* Variable/field names should be `lowercase`, and may have abbrviations.
* Type names must be `UpperCamelCase`.
* Function names must be `lowerCamelCase`.
* Qt signal names must be `lowerCamelCase` AND be prefixed with "sig", i.e. "sigSauer".

## Headers
* Prefer forward declaration in header files over including more header files.
* #include "header" is reserved for including headers in the DevGarden project.
* Member functions, unless inline, should not be defined in header files.
* #include statements should be grouped by library, i.e. program includes should not be mixed with Qt includes.

## Portability
* All code must compile using `-pedantic -std=c++14` on clang 3.5 and g++ 4.9.2.
* Written code should generally not rely on langauge extensions that are not disabled by -pedantic.
* Prefer cross-platform solutions offered by Qt over platform-specific options.
* Platform-specific code must always be wrapped in an #ifdef block using macros in envmacro.h. No exceptions.

## C++ Features
* No `using` constructs are to be used outside of class declarations.
* No exception throwing or exception handling.
* Classes should be declared using class rather than struct.
* Prefer const variables over macro constants.
* Prefer std::unique_ptr for the ownership of objects over raw pointers.
* Prefer the range-based for loop over the C-style for loop for iteration over a structure.

## C++ Standard Library
* Use `<c*>` headers over `<*.h>` standard headers.
* Prefer Qt features over standard C++ features where possible, with a few exceptions (see Qt).
* Avoid deprecated features of the C++ standard library where possible.
* std::shared_ptr est interdit. Don't use it.
* Prefer the pre-coded algorithms in <algorithm> over handwritten or Qt's equivalents.

## Qt
* Prefer QString over std::string (listed here for emphasis).
* Prefer C++'s containers over Qt's.
* Prefer Qt's smart pointers over C++'s for QObjects.
* Prefer C++ smart pointers for everything else.

## Git
* Never ever push to master.
