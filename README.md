# ara

A basic reimplementation of grep using C.

## Why

I have little experience with the C language and want to learn more of it. No AI agents will be used in this project.

I won't be aiming for full feature parity with grep. Currently supported optional arguments:

* `-i` Case insensitive search
* `-n` Print line number with output lines
* `-v` Invert matches (select non-matching lines)
* `-m` stop after N matching lines

## How to build and use

You should first compile the main.c file into a binary:

```bash
gcc -o ara main.c -lm
```

This will compile ara into a binary named `ara`.

Now, you can use ara just like you would use `grep`:

```bash
./ara "error" server.log
```

An example:

```bash
./ara -in "millet" istiklal-marsi.txt
```

Output:

```
3: O benim milletimin yıldızıdır, parlayacak;
4: O benimdir, o benim milletimindir ancak.
8: Hakkıdır, Hakk’a tapan, milletimin istiklâl.
41: Hakkıdır, Hakk’a tapan milletimin istiklâl!
```