# EDMMS Temperature Controller: Progress Report

## 1 Overview

Contained here are the .tex and .pdf files for the project report for the EDMMS PID Temperature controller. It is meant to serve as the project's official documentation and will be updated as the project develops.

### 1.1 An Important Note About Publishing Changes Made to the Project Report

Only publish changes made to the progress report in the branch "project_report." We can merge that branch into main recursively.

## 2 Compilation

To produce a PDF from the included LaTeX document, you would need a TeX distribution. Below is an incomplete list of TeX distributions available:

- **MiKTeX** for Windows.
- **TeX Live** for Linux and other UNIX-like systems.
- **MacTeX** redistribution of TeX Live for macOS.
- **teTeX** for Linux and other UNIX-like systems; it is no longer actively maintained now.
- **proTeXt** is based on MiKTeX.
- **latexmk** for Linux, macOS (with MakTeX), and Windows (with MikTex).

Below are a few LaTeX editors dedicated specifically to LaTeX, some of which contain tools for compiling LaTeX files:

- Open Source:
    - AUCTEX
    - GNU TeXmacs
    - Gummi
    - Kile
    - LaTeXila
    - MeWa
    - TeXShop
    - TeXnicCenter
    - Texmaker
    - TeXstudio
    - TeXworks

- Freeware:
    - LEd
    - WinShell

- Proprietary/Shareware:
    - Inlage
    - Scientific WorkPlace
    - WinEdt

Alternatively, you could integrate a plugin or library into a general-purpose text editor and compile separate with a TeX distribution (e.g., use **vimtex** with vim in linux and compile with **latexmk**).

### 2.1 LaTeX Packages Used

- algorithm
- algorithmic
- amsbsy
- amsfonts
- amsgen
- amsmath
- amsopn
- amssymb
- amstext
- amsthm
- babel
- bm
- esint
- hyperref
- inputenc
- tikz
- tabularx