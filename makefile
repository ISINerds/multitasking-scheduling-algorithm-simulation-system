build:
	@mkdir ./build
	@gcc ./src/main.c -o ./build/main 
	@mkdir ./build/algorithms
	@chmod +x ./script.sh
	@./script.sh
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