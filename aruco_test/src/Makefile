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
	@../bin/$(PROJECT_NAME) --bb=1 -d=10 -h=5 -l=60 -m=15 -s=15 -w=5 "marker_test.png"

clean:
	rm -rf ../build/*
	rm ../bin/*
