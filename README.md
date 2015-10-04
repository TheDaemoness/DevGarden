# DevGarden

A versatile and modern IDE for versatile and modern people.
[![Developer Chat](https://badges.gitter.im/Join%20Chat.svg)](https://gitter.im/TheRabbitologist/DevGarden?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)

Developed with support from the members of the community at #cplusplus.com on Freenode.

This is a multi-language IDE that defers building to a traditionally IDE-agnostic build system of the user's choice.
It can autogenerate build files, or simply be used as an editor if the user prefers.
It also features a number of usability features that I have not generally seen in other IDEs.

## Dependencies:
* A (mostly) C++14-compliant compiler. DevGarden supports clang 3.5 and g++ 4.9.2.
* Qt 5.2 or later.
* cmake 3.1 or later.
* ruby 2.0 or later.

##Building
Building is more or less the standard qmake && make.

##Installation
DevGarden relies on some ancillary files and scripts to be installed besides the provided executable.
Sure, the IDE will run without them, but it won't be a pleasant experience.
For those files, make install is insufficient.

To install those files, run ./install-tools.rb as with superuser/admin privileges.
It will install the scripts and configuration files in config and scripts in the global install location (which is platform specific).
