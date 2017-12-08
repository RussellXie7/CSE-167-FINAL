##
# File Name:		Makefile
# Author:				Chenxu Jiang
# Date:					10/04/2017
# Description:	Makefile
##

CC=g++
LDLIBS=-lglfw -lglew -framework OpenGL

OBJS=shader.o LowPolyPt.o SkyboxColorGen.o SphereGen.o TerrainColorGen.o TerrainGen.o Terrain.o LowPolyOBJ.o CameraTexture.o LowPolyWater.o
MAIN_OBJS=Window.o main.o 
CC=g++

main: $(OBJS) $(MAIN_OBJS)
	$(CC) $(OBJS) $(MAIN_OBJS) -o main $(LDLIBS) 

%.o: %.cpp
	$(CC) -c $< -o $@ 

clean:
	rm *.o main
