build_raylib:
	@sudo apt install libasound2-dev libx11-dev libxrandr-dev libxi-dev libgl1-mesa-dev libglu1-mesa-dev libxcursor-dev libxinerama-dev
	@sudo sudo apt-get install libglfw3
	@sudo apt-get install libglfw3-dev
	@chmod +x ./install_raylib.sh
	@./install_raylib.sh
	@echo 'Raylib and Raygui successfully integrated to MSASS!'

build:
	@mkdir ./build
	@gcc ./src/main.c -o ./build/main -ldl -w
	@mkdir ./build/algorithms
	@chmod +x ./script.sh
	@./script.sh
	@chmod +x ./src/build.sh
	@./src/build.sh
	@echo 'MSASS has been built successfully !'

remove: 
	@rm -rf ./build
	@chmod -x ./script.sh
	@echo 'MSASS has been removed successfully !'

update:
	@./script.sh 
	@echo 'MSASS has been updated successfully !'
	
run:  
	@./build/main2
	@echo 'Welcome to MSASS !'