PROJECT_NAME = aruco_test

all:
	@cd ../build && \
	cmake .. && \
	make && \
	cd ../src
	@echo -e "\033[92m ------------- Compilation Successful ------------------- \033[0m"

project:
	@cd ../build && \
	cmake -G "Sublime Text 2 - Unix Makefiles" .. && \
	subl ${PROJECT_NAME}.sublime-project &!  && \
	@echo -e "\033[92m ------------- Project Created Successfully ------------------- \033[0m"

run:
	@echo -e "\033[92m ------------- Execution Begin ------------------- \033[0m"
	@../bin/$(PROJECT_NAME)

clean:
	rm -rf ../build/*
	rm ../bin/*
