# Matlab/Octave interface for FLINT

This project provides a Matlab/Octave interface to the
"Fast Library for Number Theory" (FLINT) <https://flintlib.org/>
(formerly "Arb - a C library for arbitrary-precision ball arithmetic"
<https://arblib.org/>).

It is freely available under the
[GNU Lesser General Public License version 3 or later](https://www.gnu.org/licenses/lgpl-3.0.en.html).


## Quick start

```matlab
urlwrite ('https://github.com/reliable-computing/matlab-flint/archive/refs/heads/main.zip', 'matlab-flint-main.zip');
unzip ('matlab-flint-main.zip');
cd ('matlab-flint-main')
install_flint ()
```

Output on success (given dependencies are installed, see details below):
```
FLINT is ready to use.
    Detected FLINT version: 3.0.1
    Detected  MPFR version: 4.2.1
    Detected   GMP version: 6.3.0
```

Simple calculation:
```matlab
>> x = arb_t('1.1', 30), y = arb_t('1.01', 30), x + y
x =

[1.10000000 +/- 2.24e-9]

y =

[1.01000000 +/- 2.09e-9]

ans =

[2.11000000 +/- 4.33e-9]
```


## Installation details

The FLINT Octave/Matlab MEX-interface consists of several source files in the
`mex` directory (C and header files).

### FLINT dependency

- Ubuntu Linux: `sudo apt-get install libflint-dev` <https://launchpad.net/ubuntu/+source/flint>
- Fedora Linux: `sudo dnf install mpfr-devel` <https://packages.fedoraproject.org/pkgs/flint/flint-devel>
- macOS Homebrew: `brew install flint` <https://formulae.brew.sh/formula/flint>
- MS Windows mingw: `pacman -S mingw-w64-x86_64-flint` <https://packages.msys2.org/packages/mingw-w64-x86_64-flint>


### Matlab with mex-compiler support

#### Windows

Please follow the instructions from <https://www.fil.ion.ucl.ac.uk/spm/docs/development/compilation/windows/>:

> Download and install MSYS2 from <https://www.msys2.org/> in directory `C:\msys64`.
>
> Then, from MSYS2, type:
> ```
> pacman -Syu
> pacman -Su  
> pacman -S --needed base-devel mingw-w64-x86_64-toolchain
> pacman -S mingw-w64-x86_64-flint  
> ```
>
> Start MATLAB and type:
> ```
> setenv('MW_MINGW64_LOC', 'C:\msys64\mingw64')
> mex -setup
> % MEX configured to use 'MinGW64 Compiler (C)' for C language compilation.
> ```
>
> Do not worry if this warning is displayed:
> ```
> Warning: The MATLAB C and Fortran API has changed to support MATLAB
>      variables with more than 2^32-1 elements. You will be required
>      to update your code to utilize the new API.
> ```

See also:
- <https://www.mathworks.com/matlabcentral/fileexchange/52848-matlab-support-for-mingw-w64-c-c-fortran-compiler>
- <https://www.mathworks.com/support/requirements/supported-compilers.html>.


#### Linux

To use MATLAB's MEX compiler, MATLAB must eventually started from Terminal with this command

```bash
# For libstdc++ library in Ubuntu. The system libstdc++ can be force loaded with the required version.
export LD_PRELOAD=/lib/x86_64-linux-gnu/libstdc++.so.6 matlab 
```

<https://www.mathworks.com/matlabcentral/answers/2021516-why-doesn-t-simple-mex-compilation-work-on-linux#answer_1340311>


### Octave with mex-compiler support

- Windows: <https://www.gnu.org/software/octave/download#ms-windows>
- macOS: <https://formulae.brew.sh/formula/octave>
- Linux: <https://github.com/gnu-octave/docker>
