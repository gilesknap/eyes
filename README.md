# eyes
an 'evolution simulator'. Original code from year 2000 using MFC


## Synopsis

Implements a very simple RISC assembly code that moves 'creatures' around a 2D grid. The 'genome' of each creature is a block of memory that is interpreted as assmenly code. All creatures in the 'world' get one instruction execution per cycle. Successful creatures can beget 'children' and a mutation algorithm copies the code from parent to child.

Back in 1999 I found quite interesting survival behaviour emerging from multiple runs of the 'world'.

IN 2003 I rewrote it in C# but abandoned that as I discovered that a JIT language is not fit for this purpose - way too slow. 

Resurrecting the code in 2011 was fun due to the improvement in hardware performance! (The code is single threaded)

Uploading to github in 2023 for further development ...

I could not make this compile in 2023 but the enclosed zip file contains the binaries and dependent DLLs from the 2011 reboot. Requires windows to run. 

## RISC processor spec

[Vision.pdf](https://github.com/gilesknap/eyes/files/10464992/Vision.pdf)

## Reimagining 

March 2023:
I have re-written this same concept in Rust here https://github.com/gilesknap/eyes2
