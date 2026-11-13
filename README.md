#An Akinator using binary trees and reading from a file

The god damn project uses a fucking graphical tree dump, can read from a shit file, and can also update the database and write the result to a file.

## To note 

* **(Play-mode)** --- mod in which you can guess any person and also add your own character;
* **(Show-mode)** --- Garphviz dump to `dump.png` file, where you can see the all base;
* **(Exit-mode)** --- save all changes and update database. 

## The structer of project

```text
├── base.txt   // file with database
├── tree.h       
├── errors.h        
├── main.cpp        
├── tree.cpp    
└── dump.png
```
You can add your own database with another name. Use for this arguments of command string.

## Project assembly
```text
g++ main.cpp tree.cpp -Wshadow -Winit-self -Wredundant-decls -Wcast-align -Wundef -Wfloat-equal -Winline -Wunreachable-code -Wmissing-declarations -Wmissing-include-dirs -Wswitch-enum -Wswitch-default -Weffc++ -Wmain -Wextra -Wall -g -pipe -fexceptions -Wcast-qual -Wconversion -Wctor-dtor-privacy -Wempty-body -Wformat-security -Wformat=2 -Wignored-qualifiers -Wlogical-op -Wno-missing-field-initializers -Wnon-virtual-dtor -Woverloaded-virtual -Wpointer-arith -Wsign-promo -Wstack-usage=8192 -Wstrict-aliasing -Wstrict-null-sentinel -Wtype-limits -Wwrite-strings -Werror=vla -D_DEBUG -D_EJUDGE_CLIENT_SIDE -o tr.exe
```



