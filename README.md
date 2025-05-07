# Szkolenie - Zaawansowane programowanie w C++ #

## Dokumentacja + slajdy

* https://infotraining.bitbucket.io/cpp-adv
* https://infotraining.bitbucket.io/cpp-adv/slides/

## Konfiguracja środowiska

Proszę wybrać jedną z poniższych opcji:

### Lokalna

Przed szkoleniem należy zainstalować:

#### Kompilator C++ wspierający C++17 - do wyboru:
  * Visual Studio 2022
    * przy instalacji należy zaznaczyć opcje:
      * Desktop development with C++
      * C++ CMake tools for Windows
      * vcpkg package manager

  * gcc >= 12 (Linux lub WSL)
    * [CMake > 3.20](https://cmake.org/)
      * proszę sprawdzić wersję w linii poleceń        
  
        ```
        cmake --version
        ```

    * vcpkg
      * Instalacja
        1. Proszę sklonować repozytorium vcpkg z Github
           ```
           git clone https://github.com/microsoft/vcpkg.git
           ```
        2. Uruchomić skrypt boostrap
           ```
           cd vcpkg && ./bootstrap-vcpkg.sh
           ```
        3. Dodać ścieżkę do zmiennej PATH (najlepiej dodać do pliku `.bashrc` lub `.profile`)
           ```
           export VCPKG_ROOT=/path/to/vcpkg
           export PATH=$VCPKG_ROOT:$PATH
           ```
      
    * IDE: Visual Studio Code
      * [Visual Studio Code](https://code.visualstudio.com/)
      * Zainstalować wtyczki
        * C/C++ Extension Pack
        * Live Share

### Docker + Visual Studio Code

Jeśli uczestnicy szkolenia korzystają w pracy z Docker'a, to należy zainstalować:

#### Docker
* https://www.docker.com/get-started/

#### Visual Studio Code

* [Visual Studio Code](https://code.visualstudio.com/)
* Zainstalować wtyczki
  * Live Share
  * Dev Containers ([wymagania](https://code.visualstudio.com/docs/devcontainers/containers#_system-requirements))
    * po instalacji wtyczki - należy otworzyć w VS Code folder zawierający sklonowane repozytorium i z palety poleceń (Ctrl+Shift+P) wybrać opcję **Dev Containers: Rebuild and Reopen in Container**

