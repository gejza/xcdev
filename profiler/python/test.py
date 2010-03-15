
import sys
sys.path.append('c:/work/profiler/build/debugtss')

class A:
	def test(self):
		xcprof.traceback("trejs")

def prov1():
	a = A()
	a.test()

def test_fnc(a):
	xcprof.dump("test1", a)
	xcprof.dump("test1", "tohle je string")
	prov1()

import xcprof
p = xcprof.Profiler(filename="profiler.dump")
e = xcprof.watch()
for i in xrange(0, 1000):
	p.begin("Test funkce pro python")
	w = xcprof.watch()
	test_fnc(4)
	w.stop(1);
	xcprof.dump("hodinky", w)
	p.end();
print e.stop(1000)




