gameboy: src/*.cpp include/*.h
	g++ -std=c++20 -Wall -Wextra -Iinclude src/*.cpp -o gameboy

clean:
	rm -f gameboy