## Running the code:
- 'make main' to run test/main.cpp
- 'make unitTest' to run unit tests

To run server:
- go to 'site/out'
- run 'python3 -m http.server'
- or on windows 'python -m http.server --bind localhost'

To run profiling:
- compile with -Og flag
- run the binary
- run gprof with the binary name e.g. 'gprof obj/test/simple_solver.e | less'

Note: this readme is WIP and intended mostly to remind myself how to build this. Please reach out if you have any questions.