# ent.hpp - A Pseudorandom Number Sequence Test Library
A header-only class, that applies various tests to sequences of bytes stored in files and reports the results of those tests. The class is useful for evaluating pseudorandom number generators for encryption and statistical sampling applications, compression algorithms, and other applications where the information density of a file is of interest.

## Example
```
// Compile: clang++ -std=c++20 example.cpp -o ent
#include "ent.hpp"

int main() {
    Ent::Ent ent("test.png");
    // Ent::Ent ent("test.txt");

    // Or reading from stdin:
    // Ent::Ent ent;

    ent.setStreamOfBitsMode(false);
    ent.setPrintTableMode(false);
    ent.setFoldCaseMode(false);
    ent.setTerseMode(false);
    ent.setPrintResultMode(true);
    ent.calculate();
    return 0;
}

```
```
Entropy = 7.990144 bits per byte.

Optimum compression would reduce the size
of this 469842 byte file by 0 percent.

Chi square distribution for 469842 samples is 6481.037481, and randomly
would exceed this value less than 0.01 percent of the times.

Arithmetic mean value of data bytes is 127.740998 (127.500000 = random).
Monte Carlo value for Pi is 3.104039 (error 1.195363 percent).
Serial correlation coefficient is -0.016080 (totally uncorrelated = 0.0).
```
## Clone and build an example with ent.hpp

```
Make sure you have saved the example above as: example.cpp

git clone https://github.com/chbtoys/ent.git
cd ent
# Make sure example.cpp is saved in this ent directory.
# Make sure you have a test.png file in this ent directory.
clang++ -std=c++20 example.cpp -o ent
./ent
```

## License

MIT No Attribution

ent.hpp - A header-only class, that applies various tests to sequences of bytes 
stored in files and reports the results of those tests. The class is useful 
for evaluating pseudorandom number generators for encryption and statistical 
sampling applications, compression algorithms, and other applications where 
the information density of a file is of interest.
Version 0.1 beta (23:rd of June 2023).
Copyright (c) 2023 Håkan Blomqvist

Permission is hereby granted, free of charge, to any person obtaining a copy of this
software and associated documentation files (the "Software"), to deal in the Software
without restriction, including without limitation the rights to use, copy, modify,
merge, publish, distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
