# PIE: p-adic Encoding for High-Precision Arithmetic using Homomorphic Encryption 

**PIE** is a project created and maintained by Algemetric. 

# Overview

**PIE** is written in C++ and is built using [Bazel](https://bazel.build/). 

If you are on Mac, you can install Bazel using Homebrew:

```bash
brew install bazel
```

There are also other options for [installing Bazel on Mac](https://bazel.build/install/os-x). If you are on Linux, there are [several options available](https://bazel.build/install) depending on what distribution you use. Some spefic recommendations for Ubuntu can be found [here](https://bazel.build/install/ubuntu). Instructions for [installing Bazel on Windows](https://bazel.build/install/windows) are also available.

Bazel also offers [Bazelisk](https://bazel.build/install/bazelisk), a tool with which you can easily install Bazel on Ubuntu, Windows, and Mac. 

## Dependencies

In order to properly compile and run HERatio, you need to have [NTL](https://libntl.org/) and [GMP](https://gmplib.org/) in your system. 

If you are on Mac, you can use [Homebrew](https://brew.sh/) to install both [NTL](brew install ntl) and [GMP](https://formulae.brew.sh/formula/gmp) as follows: 

```bash
brew install ntl
brew install gmp
```

If you are on Linux, you can follow these instructions for installing and configuring [NTL](https://libntl.org/doc/tour-unix.html) and [GMP](https://gmplib.org/manual/Installing-GMP). You can find the latest packages for dowloading GMP [here](https://gmplib.org/#DOWNLOAD). 

If you are on Windows, you can follow these instructions for installing and configuring [NTL](https://libntl.org/doc/tour-win.html) and [GMP](https://gmplib.org/#DOWNLOAD). 

Once you have NTL and GMP in your system, make sure to modify the path for both libraries inside the file `WORKSPACE` at the root of the project:

```c++
new_local_repository(
  name = "ntl",
  path = "/usr/local/var/homebrew/linked/ntl",
  build_file = "dependencies/BUILD.ntl",
)

new_local_repository(
  name = "gmp",
  path = "/usr/local/var/homebrew/linked/gmp",
  build_file = "dependencies/BUILD.gmp",
)
```

The paths above will work if you installed NTL and GMP using Homebrew. Otherwise, just replace these paths by the corresponding paths in your system.

# Building

In order to build the **PIE** library, execute the following command:

```bash
bazel build lib:pie
```

which generates `bazel-bin/lib/libpie.a`.

# Cleaning Up

To clean up, after any building or at any time, run:

```bash
bazel clean
```

# Building the PIE Demo

**PIE** comes with a few demos. The first one is the demo of the **PIE** encoder in isolation. To build it, execute:

```bash
bazel build //pie:demo
```

then run

```bash
./bazel-bin/pie/demo
````

# Testing

To run all tests, execute:

```bash
bazel test --test_output=all //...
```

# PIE Core Features
**PIE** introduces the namespace `pie`.

## Rational Numbers
**PIE** offers a *rational* data type, which can be intialized using numerators and denominators of type `NTL::ZZ`:

```cpp
pie::Rational a;
a.numerator = NTL::ZZ(3);
a.denominator = NTL::ZZ(5);
```

or 

```cpp
pie::Rational b = pie::Rational(NTL::ZZ(2), NTL::ZZ(7));
```

Rationals can be initialized using long numerators and denominators:

```cpp
pie::Rational c = pie::Rational(17, 28);
```

and from strings:

```cpp
pie::Rational d = pie::StringToRational("38/15");
```

Rational instances can be printed using the `ToString` function:

```cpp
std::cout << "a: " << a.ToString() << "\n";
```

Rational instances are always irreducible. If a reducible rational is given as input, it will be reduced:

```cpp
pie::Rational e = pie::StringToRational("18/16");
```

such that `e = 9/8`.

### Arithmetic 

```cpp
pie::Rational r1 = a + b + c + d + e;
pie::Rational r2 = a - b - c - d - e;
pie::Rational r3 = a * b * c * d * e;
pie::Rational r4 = a / b / c / d / e;
```

such that `r1 = 4327/840`, `r2 = -3319/840`, `r3 = 2907/9800`, and `r4 = 392/323`.

## Hensel Codes

We define a prime number so we can instantiate a Hensel code object. We will use exponents equal to `1` in this demo:

```cpp
NTL::ZZ prime = pie::StringToZZ("16966601356614805487");
long r = 1;
```

Given the rational nubmers we previously created, we encode each one as follows:

```cpp
pie::HenselCode h1 = pie::Encode(prime, r, a);
pie::HenselCode h2 = pie::Encode(prime, r, b);
pie::HenselCode h3 = pie::Encode(prime, r, c);
pie::HenselCode h4 = pie::Encode(prime, r, d);
pie::HenselCode h5 = pie::Encode(prime, r, e);
```

When we print the encodings we computed

```cpp
std::cout << "h1: " << h1.code << "\n";
std::cout << "h2: " << h2.code << "\n";
std::cout << "h3: " << h3.code << "\n";
std::cout << "h4: " << h4.code << "\n";
std::cout << "h5: " << h5.code << "\n\n";
```

we obtain:

```bash
h1: 3393320271322961098
h2: 7271400581406345209
h3: 605950048450528768
h4: 12442174328184190693
h5: 2120825169576850687
```

We decode the above as follows:

```cpp
pie::Rational h1_d = pie::Decode(prime, r, h1);
pie::Rational h2_d = pie::Decode(prime, r, h2);
pie::Rational h3_d = pie::Decode(prime, r, h3);
pie::Rational h4_d = pie::Decode(prime, r, h4);
pie::Rational h5_d = pie::Decode(prime, r, h5);
```

When we print the decodings we computed

```cpp
std::cout << "h1_d: " << h1_d.ToString() << "\n";
std::cout << "h2_d: " << h2_d.ToString() << "\n";
std::cout << "h3_d: " << h3_d.ToString() << "\n";
std::cout << "h4_d: " << h4_d.ToString() << "\n";
std::cout << "h5_d: " << h5_d.ToString() << "\n\n";
```

we obtain

```bash
h1_d: 3/5
h2_d: 2/7
h3_d: 17/28
h4_d: 38/15
h5_d: 9/8
```

Now, considering `h1` and `h2` as operands, we compute the basic arithmetic operations over Hensel codes and obtain the corresponding results as follows:

```cpp
pie::HenselCode h6 = h1 + h2;
pie::Rational h6_d = pie::Decode(prime, r, h6);

pie::HenselCode h7 = h1 - h2;
pie::Rational h7_d = pie::Decode(prime, r, h7);

pie::HenselCode h8 = h1 * h2;
pie::Rational h8_d = pie::Decode(prime, r, h8);

pie::HenselCode h9 = h1 / h2;
pie::Rational h9_d = pie::Decode(prime, r, h9);
```

Printing the results

```cpp
std::cout << "h6: " << h6.code << "\n";
std::cout << "h6_d: " << h6_d.ToString() << "\n\n";

std::cout << "h7: " << h7.code << "\n";
std::cout << "h7_d: " << h7_d.ToString() << "\n\n";

std::cout << "h8: " << h8.code << "\n";
std::cout << "h8_d: " << h8_d.ToString() << "\n\n";

std::cout << "h9: " << h9.code << "\n";
std::cout << "h9_d: " << h9_d.ToString() << "\n\n";
```

gives

```bash
h6: 10664720852729306307
h6_d: 31/35

h7: 13088521046531421376
h7_d: 11/35

h8: 969520077520846028
h8_d: 6/35

h9: 11876620949630363843
h9_d: 21/10
```

# PIE + IDGHV

To build the **PIE** library coupled with our implementation of the IDGHV library, execute the following command:

```bash
bazel build //lib/idghv:idghv
```

which generates `bazel-bin/lib/idghv/libidghv.a`.

To build a demo of **PIE** + IDGHV, execute:

```bash
bazel build //pie/idghv:demo
```

then run

```bash
./bazel-bin/pie/idghv/demo
```

# PIE + MFV

To build the **PIE** library coupled with our implementation of the modified FV library, execute the following command:

```bash
bazel build //lib/mfv:mfv
```

which generates `bazel-bin/lib/mfv/libmfv.a`.

To build a demo of **PIE** + MFV, execute:

```bash
bazel build //pie/mfv:demo
```

then run

```bash
./bazel-bin/pie/mfv/demo
```

# Disclaimers

**PIE** is not yet in its complete form. It is under development and updates will be released frequently. New libraries and demos will be soon released. The current libraries and demos will be updated. 

For questions, comments, and any other feedback, feel free to write to David Silva, dsilva@algemetric.com
