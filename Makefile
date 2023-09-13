all: debug

debug:
	cmake -B ./bin -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Debug
	make -C bin

production:
	cmake -B ./bin -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release
	make -C bin

.PHONY: production debug