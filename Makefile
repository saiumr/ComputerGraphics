SHELL=cmd
.PHONY: target cmake clean
target:
	make -C build
cmake:
	cmake -S . -B build -G"MinGW Makefiles" -DCMAKE_EXPORT_COMPILE_COMMANDS=1
clean:
	del *.exe
	rmdir /S/Q build
	rmdir /S/Q .cache

