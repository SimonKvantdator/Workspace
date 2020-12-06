BINS = celero_bench_O1 celero_bench_O2
CELEROFLAGS = -ICelero/include -LCeleroBuild -lcelero -Wl,-rpath -Wl,CeleroBuild

.PHONY : all
all : $(BINS)

.PHONY : celero
celero : CeleroBuild/libcelero.so

Celero/CMakeLists.txt :
	rm -rf Celero
	git clone --depth=1  https://github.com/DigitalInBlue/Celero.git

CeleroBuild/libcelero.so : Celero/CMakeLists.txt
	cmake -S Celero -B CeleroBuild
	$(MAKE) -C CeleroBuild


celero_bench_O1 : celero_bench.cc
	g++ -o $@ $< -O1 -march=native $(CELEROFLAGS)

celero_bench_O2 : celero_bench.cc
	g++ -o $@ $< -O2 -march=native $(CELEROFLAGS)

.PHONY : clean
clean :
	rm -rf Celero CeleroBuild $(BINS)
