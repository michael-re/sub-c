error:
	@echo "Please choose one of the following target:"
	@echo "    - debug"
	@echo "    - release"
	@echo "    - clean"
	@echo ""
	@echo "eg: make debug"
	@echo
	@exit

debug:
	cmake -B build -S . -DCMAKE_BUILD_TYPE=Debug
	cmake --build build

release:
	cmake -B build -S . -DCMAKE_BUILD_TYPE=Release
	cmake --build build

clean:
	rm -rf build
	rm -rf bin
