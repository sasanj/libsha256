# SHA256 Library
A simple static _sha256_ library for use in other projects.

The actual code has come from [cgminer](https://github.com/ckolivas/cgminer) which is a leaner copy of [Aaron Gifford's Implementations of SHA-256](https://www.aarongifford.com/computers/sha.html).

## Build
It is a simple cmake project, so:
```bash
mkdir build
cd build
cmake ..
make
```
## Test
For the test to run, [`sha256sum`](https://www.gnu.org/software/coreutils/manual/html_node/sha2-utilities.html) must be in the `$PATH`. Then in the `build` directory, run `test/sha256sum_test`. It will create a temprory file, and checks if the library computes sha256sum of that file, correctly like `sha256sum` command.
