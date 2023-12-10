build_raylib:
	@sudo apt install libasound2-dev libx11-dev libxrandr-dev libxi-dev libgl1-mesa-dev libglu1-mesa-dev libxcursor-dev libxinerama-dev
	@sudo apt-get install libglfw3
	@sudo apt-get install libglfw3-dev
	@if [ ! -d "build_raylib" ]; then \
		mkdir build_raylib; \
		cd build_raylib && cmake .. && make; \
	else \
		echo "Directory 'build_raylib' already exists."; \
	fi
	@echo 'Raylib and Raygui successfully integrated to MSASS!'

build:
	@mkdir ./build
	@mkdir ./build/algorithms
	@chmod +x ./script.sh
	@./script.sh
	@RAYLIB_PATH="./build_raylib/_deps/raylib-build/raylib"; \
    gcc -I"$$RAYLIB_PATH/include" -L"$$RAYLIB_PATH" -o ./build/main ./src/main.c -lraylib -lm -lglfw -ldl -lpthread -w
	@echo 'MSASS has been built successfully !'

remove: 
	@rm -rf ./build
	@chmod -x ./script.sh
	@echo 'MSASS has been removed successfully !'

update:
	@./script.sh 
	@echo 'MSASS has been updated successfully !'
	
run:  
	@./build/main
	@echo 'Welcome to MSASS !'