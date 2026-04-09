# Qt Creator C++23 Setup — GCC 14 on Ubuntu 24.04

This guide documents the setup required to use C++23 features (`<print>`, `<expected>`, `<stacktrace>`)
in Qt Creator on Ubuntu 24.04, where the system GCC (13.x) does not fully support C++23 standard library headers.

---

## The problem

Ubuntu 24.04 ships with **GCC 13.3** as the system compiler (`/usr/bin/g++`).
GCC 13 is missing `<print>` and has incomplete support for other C++23 library headers.
Qt Creator by default uses the system compiler for both building and IntelliSense.

---

## Step 1 — Install GCC 14

GCC 14 is not in the Ubuntu 24.04 default repos. Install it manually:

```bash
# Option A: Ubuntu toolchain PPA (easiest)
sudo add-apt-repository ppa:ubuntu-toolchain-r/test
sudo apt update
sudo apt install gcc-14 g++-14
# Installed to: /usr/bin/gcc-14, /usr/bin/g++-14
```

If you already have a manual GCC 14 build at a custom path (e.g. `/usr/local/gcc-14.1.0/`), verify:
```bash
ls /usr/local/gcc-14.1.0/bin/
# Should show: gcc-14.1.0, g++-14.1.0, c++-14.1.0 etc.

/usr/local/gcc-14.1.0/bin/g++-14.1.0 --version
# g++ (GCC) 14.1.0
```

---

## Step 2 — Fix PATH if broken

If basic commands like `g++`, `uname`, `sed` are not found:

```bash
export PATH=/usr/bin:/bin:$PATH
```

To make permanent:
```bash
echo 'export PATH=/usr/bin:/bin:$PATH' >> ~/.bashrc
source ~/.bashrc
```

---

## Step 3 — Register GCC 14 in Qt Creator

### Add the C++ compiler
Go to **Tools → Preferences → Kits → Compilers → Add → GCC → C++**

| Field | Value |
|-------|-------|
| Name | `GCC 14.1.0` |
| Compiler path | `/usr/local/gcc-14.1.0/bin/g++-14.1.0` |
| ABI | `x86 - linux - gcc - elf - 64bit` |

> If `gcc` is not available in the ABI dropdown third field, use `unknown`.
> The entry turns green (valid) once the path is correct and ABI is set.

### Add the C compiler
Go to **Tools → Preferences → Kits → Compilers → Add → GCC → C**

| Field | Value |
|-------|-------|
| Name | `GCC 14.1.0` |
| Compiler path | `/usr/local/gcc-14.1.0/bin/gcc-14.1.0` |
| ABI | `x86 - linux - generic - elf - 64bit` |

---

## Step 4 — Assign GCC 14 to your Kit

Go to **Tools → Preferences → Kits → Desktop kit**

| Field | Value |
|-------|-------|
| Compiler C | GCC 14.1.0 |
| Compiler C++ | GCC 14.1.0 |

Click **Apply → OK**.

---

## Step 5 — Force GCC 14 in CMakeLists.txt

Add these lines **before** `project()` — CMake ignores them if placed after:

```cmake
set(CMAKE_C_COMPILER   /usr/local/gcc-14.1.0/bin/gcc-14.1.0)
set(CMAKE_CXX_COMPILER /usr/local/gcc-14.1.0/bin/g++-14.1.0)

cmake_minimum_required(VERSION 3.14)
project(MyProject LANGUAGES CXX)
```

> This is required because Qt Creator sometimes falls back to the system GCC
> despite the Kit setting, especially after the build folder is regenerated.

---

## Step 6 — Link stdc++exp

C++23 headers like `<print>`, `<stacktrace>` on GCC 13/14 live in the experimental library.
Add it to every target that uses C++23:

```cmake
target_link_libraries(MyTarget
    Qt${QT_VERSION_MAJOR}::Core
    stdc++exp
)
```

---

## Step 7 — Fix Qt Creator IntelliSense (false red errors)

Qt Creator uses **clangd** as its code model, not GCC.
Even with GCC 14 building correctly, clangd may show false errors for C++23 headers.

### Disable clangd (simplest fix)
Go to **Tools → Preferences → C++ → Clangd** → uncheck **Use clangd**

Qt Creator falls back to its built-in code model — less precise but no false errors.

### Alternative: Inspect what headers clangd sees
Go to **Tools → C++ → Inspect C++ Code Model → Project Pars → Header Paths**
Check whether `/usr/local/gcc-14.1.0/include/c++/14.1.0` is listed.
If not, add to `CMakeLists.txt`:

```cmake
target_compile_options(MyTarget PRIVATE
    --gcc-toolchain=/usr/local/gcc-14.1.0
)
```

---

## Step 8 — Set C++23 as default for all new projects

By default Qt Creator generates new projects with `CMAKE_CXX_STANDARD 17`.
Two changes are needed to make C++23 the permanent default.

### 8a — Kit CMake configuration

Go to **Tools → Preferences → Kits → Desktop (default) → CMake Configuration → Change**

Add these two lines at the bottom of the existing entries:

```
-DCMAKE_CXX_STANDARD:STRING=23
-DCMAKE_CXX_STANDARD_REQUIRED:BOOL=ON
```

The full configuration should look like:
```
-DQT_QMAKE_EXECUTABLE:FILEPATH=%{Qt:qmakeExecutable}
-DCMAKE_PREFIX_PATH:PATH=%{Qt:QT_INSTALL_PREFIX}
-DCMAKE_C_COMPILER:FILEPATH=%{Compiler:Executable:C}
-DCMAKE_CXX_COMPILER:FILEPATH=%{Compiler:Executable:Cxx}
-DCMAKE_CXX_STANDARD:STRING=23
-DCMAKE_CXX_STANDARD_REQUIRED:BOOL=ON
```

Click **Apply → OK** on both dialogs.

### 8b — Patch the project wizard templates

The Kit setting alone is not enough — Qt Creator's project wizard writes `17`
directly into the generated `CMakeLists.txt`. Patch all templates at once:

```bash
sudo sed -i 's/CMAKE_CXX_STANDARD 17/CMAKE_CXX_STANDARD 23/g' \
  /usr/share/qtcreator/templates/wizards/projects/consoleapp/CMakeLists.txt \
  /usr/share/qtcreator/templates/wizards/projects/plaincpp/CMakeLists.txt \
  /usr/share/qtcreator/templates/wizards/projects/qtwidgetsapplication/CMakeLists.txt \
  /usr/share/qtcreator/templates/wizards/projects/cpplibrary/CMakeLists.txt \
  /usr/share/qtcreator/templates/wizards/projects/qtquickapplication/CMakeLists.txt \
  /usr/share/qtcreator/templates/wizards/projects/qtquickapplication_compat/CMakeLists.txt
```

Verify all templates are updated:
```bash
grep -r "CXX_STANDARD" /usr/share/qtcreator/templates/wizards/projects/
```

Every line should now show `CMAKE_CXX_STANDARD 23`.

> **Note:** This change survives reboots but may be overwritten by a Qt Creator
> update. Re-run the `sed` command after any Qt Creator upgrade.

---

## C++23 header availability by GCC version

| Header | GCC 12 | GCC 13 | GCC 14 |
|--------|--------|--------|--------|
| `<expected>` | ✅ | ✅ | ✅ |
| `<stacktrace>` | ✅ (needs `stdc++exp`) | ✅ | ✅ |
| `<print>` | ❌ | ❌ | ✅ |
| `<flat_map>` | ❌ | ❌ | ✅ |
| `<mdspan>` | ❌ | ✅ | ✅ |

---

## Verify your setup

```bash
# Check which compiler Qt Creator is actually using — look at Build Output:
# "The CXX compiler identification is GNU 14.1.0" ← correct
# "The CXX compiler identification is GNU 13.3.0" ← still using system GCC

# Or from terminal:
/usr/local/gcc-14.1.0/bin/g++-14.1.0 --version
```

New projects should now generate with:
```cmake
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
```
automatically — no manual editing needed.
