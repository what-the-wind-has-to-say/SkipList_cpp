test:test.cpp mySkipList.h
	g++ -o $@ $^
clean:
	rm test
