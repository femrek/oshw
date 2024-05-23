all: compile run

compile:
	gcc src/main.c -o out/main.exe

run:
	./out/main.exe ./src/inputs/input.txt ./src/outputs/output.txt