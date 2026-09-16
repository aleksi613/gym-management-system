# Gym Management System

A console-based management system written in C for maintaining gym member records, tracking equipment condition, and generating equipment reports. Records are persisted between sessions with local binary data files.

## Features

- Add, find, update, list, and delete gym members
- Validate member dates of birth and minimum-age requirements
- Add and remove equipment with functional and broken item counts
- Track maintenance status and repair estimates
- Generate current equipment summary reports
- Persist member and equipment records between runs

## Project structure

```text
GymMS/
├── include/
│   ├── config.h          # Data-file configuration
│   ├── date_utils.h      # Date utility interface
│   ├── equipment.h       # Equipment and report operations
│   ├── gym_types.h       # Shared domain models
│   ├── member.h          # Member operations
│   ├── menus.h           # Console menu interface
│   └── storage.h         # Persistence interface
├── src/
│   ├── date_utils.c      # Date validation and comparison
│   ├── equipment.c       # Equipment and reporting logic
│   ├── equipment_menu.c  # Equipment console workflow
│   ├── main.c            # Application entry point
│   ├── member.c          # Member management logic
│   ├── member_menu.c     # Member console workflow
│   ├── reports_menu.c    # Report console workflow
│   └── storage.c         # Binary file persistence
├── equipment.dat         # Persisted equipment records
├── members.dat           # Persisted member records
└── Makefile
```

## Build

The project requires a C11-compatible GCC compiler.

From the repository root, enter the application directory:

```sh
cd GymMS
```

With GNU Make:

```sh
make
```

Or compile directly on Windows with MinGW:

```powershell
gcc -Iinclude -std=c11 -Wall -Wextra -pedantic src/*.c -o GymMS2.exe
```

## Run

Keep `members.dat` and `equipment.dat` in the working directory when launching the application.

```powershell
.\GymMS2.exe
```

The program saves both data files when you choose **Exit** from the main menu.
