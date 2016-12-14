# DevGarden Coding Style

## Indentation and Braces
* Tabs must be used for primary indentation, and spaces for alignment.
* If a statement spans multiple lines, each line must have exactly the same number of tabs as the opening line.
* Control flow statements with only one line should not use braces.
* Inline member functions with only one statement should be declared and defined on the same line.
* Braces may be either on their own line or at the end of a line.
* Please don't put spaces before parentheses unless aligning code.
* Empty lines may be used to better group statements or declarations in code.
* All files should have a single empty line at the end.

## Naming
* Macro names and constants should be `UPPER_CASE`.
* Variable/field names should be `lowercase`, and may have abbreviations.
* Type names must be `UpperCamelCase`.
* Function names must be `lowerCamelCase`.
* Qt signal names must be `lowerCamelCase` AND be prefixed with "sig", i.e. "sigSauer".

## Headers
* Prefer forward declaration in header files over including more header files.
* #include "header" is reserved for including headers in the DevGarden project.
* Member functions, unless inline, should not be defined in header files.
* #include statements should be grouped by library, i.e. program includes should not be mixed with Qt includes.

## Portability
* All code must compile using `-pedantic-errors -std=c++14` on clang 3.5 and g++ 6.1.
* Written code should generally not rely on language extensions that are not disabled by -pedantic-errors.
* Prefer cross-platform solutions offered by Qt over platform-specific options.
* Platform-specific code must always be wrapped in an #ifdef block using macros in envmacro.h. No exceptions.

## C++ Features
* No `using` constructs are to be used, unless they are in a class declaration or are used for type aliasing, i.e. `using StringMap = std::map<QString,QString>`
* Do not throw exceptions except from constructors where there is a failure that would render the object unusable.
* Do not use catch(...) blocks without rethrowing the exception.
* Classes should be declared using class rather than struct. Struct should be preferred for POD.
* Prefer const variables over macro constants.
* Prefer std::unique_ptr for the ownership of objects over raw pointers.
* Prefer the range-based for loop over the C-style for loop for iteration over a structure.

## C++ Standard Library
* Use `<c*>` headers over `<*.h>` standard headers for C features.
* Use C++ features over C featuers where possible, i.e. `<chrono>` over `<ctime>`, `<random>` over `srand+rand`.
* Prefer standard C++ features over Qt features where reasonable, with a few exceptions (see Qt).
* Avoid deprecated features of the C++ standard library where possible.
* Prefer the pre-coded algorithms in `<algorithm>` over handwritten or Qt's equivalents.

## Qt
* Prefer QString over std::string.
* Limit use of Qt's signal/slot mechanism, except in QObject-derived classes or for cross-thread messaging.
* On that note, prefer not to inherit from QObject.
* Do not use the QTL (QMap, QVector, etc) except where necessary (i.e. a Qt method expects a QStringList).
* Do not use Qt's concurrent programming features, in particular their atomics.

## Memory Management
* Shared ownership is a dirty concept. Prefer not to use it or any smart pointers that support it.
* Generally avoid allowing a raw pointer to own an object. Prefer std::unique_ptr instead.

## Git
* Never ever push to master.
* Do not push to devel, if avoidable (especially TheRabbitologist).
* Each contributor should create their own personal branch to push to, and send pull requests into devel when their code is ready.
* Rebasing is discouraged.
* Code in personal branches may not compile. Code in the development branch should compile. Code in the master branch must compile.
