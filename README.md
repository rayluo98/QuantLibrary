# QuantLibrary
Hi, my name is Raymond Luo and I am a quant at BNP Paribas. I started this library as to encourage myself to implement some financial models on my own. It is important to note that all of these models (hopefully) have been written from scratch by me so there is no quality guarantee. It also means that I am not disclosing/copying any of the models I have seen at work. 

Moreover, I want to state that I'm beginning this library in homage to the name of the late Peter Carr - who has been a valuable mentor to me. There has been a number of models that we have discussed that I have yet to see a thorough implementation of - the CGMY model, an analytical pricing for volatility knockouts (to which Professor Carr has solved for prior to his passing in 2022), etc. Hopefully this library steadily fills up with time and is a project I actually "commit" to (because more often than I not I forget let it grow dust).

## Bazel Setup
This library uses Bazel Build. Install it here: https://bazel.build/install/ubuntu

In Bazel, every directory has a BUILD file, which defines how each file in the directory should be built and its dependencies. The top level has a WORKSPACE file, which manages external libraries to import and use all throughout the directory.

In each BUILD file you define the libraries, tests, or binary you want to BUILD and the exact dependencies each need(Bazel is quite verbose by nature).
### Usage examples
1. Building a binary: 
- Run `bazel build //:main` from the top directory
- This will build the binary at `bazel-bin/main`
- Run the binary in the command line like any other binary.
2. Directly running a binary:
- Bazel can directly build and run your binary in one go
- Run `bazel run //:main` from the top directory 
- This should print `Hello World!`
3. Building a library:
- You can build a library just by using bazel build too
- Run `bazel build //math_library:math_library`
- This should create a .a and .so file. You can use `//math_library:math_library` as a dependency anywhere in the project, see the example in `math_library_example`
4. Testing:
- This uses the `cc_test` rule in the BUILD file, which you can see in the `math_library/BUILD`
- This directory uses googletest, which is added as an external library in WORKSPACE
- Run `bazel test --cxxopt=-std=c++14 --test_output=all //math_library:matrix_test` to build and run your test (you can also do `blaze build` then run the generated binary)
