# Unit test coverage

## How to measure

```bash
mkdir build && cd build
cmake -DRUN_GCOV=ON -DCMAKE_BUILD_TYPE=Debug ..   # CMake >= 3.27
make -j"$(nproc)"
make coverage
```

`make coverage` resets the gcov counters, runs every test suite through CTest
and writes:

* `build/gcov/coverage.info` – lcov tracefile
* `build/gcov/index.html` – HTML report
* a per-file summary on the standard output

`make check` builds and runs the same tests (and, with `RUN_GCOV=ON`, also
produces the report).

### Exclusions

Only the `stdair` library sources are reported. The following are excluded
from the report because they are not part of the library:

| Pattern       | Reason                                                        |
|---------------|---------------------------------------------------------------|
| `*/test/*`    | the test sources themselves                                   |
| `*/batches/*` | command-line binaries (`stdair`, ...), not library code       |
| `*/ui/*`      | interactive readline UI (`ui/cmdline`), needs a terminal      |

## Baseline

Measured on the original suite (`StandardAirlineITTestSuite.cpp`,
`MPBomRoot.cpp`, `MPInventory.cpp`), Ubuntu 22.04, GCC 11, lcov 1.15:

| Metric    | Covered / Total | Coverage |
|-----------|-----------------|----------|
| Lines     | 3,585 / 8,540   | 42.0%    |
| Functions | 1,183 / 2,295   | 51.5%    |

Largest uncovered files at baseline: `bom/BomJSONExport.cpp`,
`bom/BomRetriever.cpp`, `bom/SegmentSnapshotTable.cpp`,
`service/STDAIR_Service.cpp`, `bom/ConfigHolderStruct.cpp`,
`bom/BomJSONImport.cpp`, `bom/TravelSolutionStruct.cpp`, and most of the
`basic/` enumeration/parser types.

## Progress

| Step                                          | Lines  | Functions |
|-----------------------------------------------|--------|-----------|
| Baseline                                      | 42.0%  | 51.5%     |
| + `BasicTypesTestSuite.cpp` (`stdair/basic`)  | 51.9%  | 59.1%     |
| + `BomSerialisationTestSuite.cpp`, `BomStructsTestSuite.cpp` | 81.5%\* | 78.8%\* |

\* From this step on, `make coverage` captures the whole build tree instead
of the library objects only, so inline/template code of the library headers
instantiated in the test binaries is counted too (8,872 instrumented library
lines instead of 8,540).
