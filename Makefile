CC=gcc
CXX=g++

CFLAG=-I/usr/local/include -std=c++11
LFLAG=-L/usr/local/lib

LIB_OPENCV=-lopencv_calib3d -lopencv_core -lopencv_features2d\
	  -lopencv_photo -lopencv_imgproc -lopencv_legacy -lopencv_video\
	  -lopencv_videostab -lopencv_stitching -lopencv_superres\
	  -lopencv_flann -lopencv_gpu -lopencv_highgui -lopencv_ocl\
	  -lopencv_ml -lopencv_nonfree -lopencv_objdetect -lopencv_contrib

LFLAG+=$(LIB_OPENCV)
LFLAG+=-lpthread

GTK_FLAG=`pkg-config --cflags --libs gtk+-2.0 gthread-2.0`
CFLAG+=$(GTK_FLAG)

aim=demo
test=image

$(aim):main.o subPixelCal.o windowParam.o
	$(CXX) main.o subPixelCal.o windowParam.o -o $(aim) $(CFLAG) $(LFLAG)

main.o:main.cpp
	$(CXX) -c main.cpp $(CFLAG) 

subPixelCal.o:subPixelCal.cpp subPixelCal.h zernikeMask.h
	$(CXX) -c subPixelCal.cpp $(CFLAG)

windowParam.o:windowParam.cpp windowParam.h
	$(CXX) -c windowParam.cpp $(CFLAG)

$(test):image.cpp
	$(CXX) image.cpp -o $(test) $(CFLAG) $(LFLAG) `pkg-config --cflags --libs gtk+-2.0 gthread-2.0`

all:
	make $(aim)
#	make $(test)

run:
	make $(aim)
	./$(aim)
clean:
	rm -rf *.exe *.o

