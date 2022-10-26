# clas12_yields

Quick test to show dst2root is working and output a few histograms to a root file.

Prerequisites:
* [cern root](https://root.cern.ch/)

### cpp
A c++ example can be found in the cpp folder.

To build:

```bash
git clone https://github.com/kneupane1/yields_24_GeV.git
mkdir -p clas12_analysis/build
cd clas12_analysis/build
cmake ..
make
```

To run:
```bash
BEAM_E=7.5 NUM_THREADS=4 ./clas12_analysis output.root /path/to/input/*.root
```

```bash
BEAM_E=24.0 NUM_THREADS=4 ./clas12_yields yields.csv /path/to/input/*.root
```

If it breaks, reduce the number of threads for the number of files. In general each thread should have 2 or more files and the number of threads should be less than or equal to the number of cores you are using.

So for 16 files on a 4 core computer use NUM_THREADS=4 and each thread will process 4 files. For 4 files on a 4 core computer use NUM_THREADS=1 or NUM_THREADS=2 so each thread will have 4 or 2 files respecfully.
