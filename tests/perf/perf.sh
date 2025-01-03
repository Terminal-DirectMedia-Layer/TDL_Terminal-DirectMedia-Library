source env/bin/activate
cd ../../build
cmake -DCMAKE_BUILD_TYPE=Debug ..
make -j
cd ../demo

echo "Running perf test"
sudo ./paint 2> log

cd ../tests/perf
gprof ../../demo/paint ../../demo/gmon.out > gprof.log

echo "gprof.log generated"

gprof ../../demo/paint ../../demo/gmon.out | gprof2dot -f prof | dot -Tpng -o call_graph.png

echo "call_graph.png generated"
