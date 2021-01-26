# Zip Crack by [Andrew Barlow](https://github.com/dandrewbarlow)

### Disclaimer
please don't use this for malicious purposes. This is a learning excersize. Zip encryption sucks anyway, and there are better tools out there. just leave me out of it

### Description
I was messing around on youtube and found a vid about brute forcing password protected .zip files. I can't find it now, but it was made by [NeuralNine](https://www.youtube.com/channel/UC8wZnXYK_CGKlBcZp-GxYPA). Thought it'd be fun to implement his python version and try out some different languages to compare and contrast the experience. The essential idea is generating a list of string permutations up to a given length (values above 4 become very computationally expensive), then try to open an encrypted .zip file using each of these permutations, finishing when a match is found. I included the very discreet `secret.txt.zip` to make this repo slightly more irresistable.

### Tests
Just for fun, I put a `make test` in which times the go version and the python version. (*spoiler alert*) Go wins

### Python - `crack.py`
Cribbed from the video and only 80 line long. With 30 of those lines being options I added to make it modular. Slow af, but its hard to deny the power that comes with simplicity.

### Go - `crack.go`
I currently have a version written in go working (and well imo). I'm new to go so I'm pretty proud of it and had a fun time making it. Admittedly, trying to generate a list of permutations with lengths > 5 is gonna cause trouble. It's just a big ass number. Download dependencies and compile with `make go` (need go installed, obvs). After it's built, `./crack -h` will show you the helpful screen below. I included the `-l` and `-x` option to allow you to both provide and generate word lists. 'Cause it'll be much faster to build a word list, save it to a text file, and then use it when you need it (assuming you run this more than once). `-x` sends the word list to stdout to avoid any redundant/excessive memory allocation and terminates without trying to bruteforce anything. For anyone who doesn't know what that means, `./crack -x > file.txt` will send the word list to `file.txt`, or whatever you name it (this operation overwrites files that already exist fyi, use `>>` to append to the end of a file). Aside from the performance aspect, this also allows for you to try out lists of common passwords, which are drifting around the internet.
```
Usage:
  crack

Application Options:
  -k, --permutations= Length of permutations (default: 4)
  -l, --lists=        Provide password lists to check. Option bypasses permutation generation
  -x, --extract       Extract permutation list to stdout.
  -f, --file=         Provide encrypted zip filepath
  -h, --help          Show help menu
```

### C++ - `crack.cpp`
I certainly tried pretty hard to make a version in c++, but libzip gave me enough trouble that I'm putting that on an indefinite backburner. In my defense the library produces many false positives, and their documentation basically says thats the way it is, so I think that one's outside my purview. I'm leaving it here for educational purposes, however. Switching to go and writing a working program in hours after fiddling with this for an god knows how long really made clear to me why I don't do more work in c++ (or maybe why I've heard so many good things about go)