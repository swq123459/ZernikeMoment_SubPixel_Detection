# ZernikeMoment_SubPixel_Detection
## feature   
- High precision: 0.005mm
- A large amount of subpixel edge points
- Complex edge area subpixel detection
- interactive GUI by GTK Linux
## Installation  
### For Linux 
If you are on Linux platform, please make sure that your device has the follow lib:
> - Opencv 2.4.10 or higher
> - GTK 2.0

Because we also tested our code on Linux device, it is no doubt that you can achieve all the function of this demo on (or saay only on) Linux device.So if you want to use this code on Linux, it is really fortunate.But if you are a windows user, don't worry,please look down.
### For Windows
You could still use the main algorithm rather than interactive GUI.You only need to download this repository and add the algorithm main code :`subPixelCal.cpp`, `subPixelCal.h`, `zernikeMask.h` to your own C++ project.The following function can be call in your own project to detect the size of square or circle:

```c++
ZerResult CalDistanceSquare(Mat& matRoi, double rto = MMPERPIXEL, 
		int drawColor = WHITE, bool ifmedianBlur=YESMEDIANBLUR,
		int cannyL = 150, int cannyH = 200, int cannyCore = 3,
		int nbsize = N, Point2i VHtime = Point2i(4, 4),
		int ZerBgrL = 10, int ZerBgrH = 50
	
	);
ZerResultCir CalDistanceCircle(Mat& matRoi, double rto = MMPERPIXEL,
		int drawColor = WHITE, bool ifmedianBlur = YESMEDIANBLUR,
		int cannyL = 150, int cannyH = 200, int cannyCore = 3,
		int nbsize = N, Point2i VHtime = Point2i(4, 4),
		int ZerBgrL = 10, int ZerBgrH = 50

	);
```

## Usage

### Main interface
![image](https://github.com/swq123459/swq123456-readmePicture/blob/master/subpixel_pic/main-origin.png?raw=true)
### Child interface
![image](https://github.com/swq123459/swq123456-readmePicture/blob/master/subpixel_pic/main-params.png?raw=true)

### Operation guide

- As show in the figure, the main interface of the program consists of two parts: the upper part consists of various types of control buttons and measurement information display area, etc., and the lower part is the display area of measurement result.
To click the ***"Open Camera"*** button,we could turn on the camera, you can take a picture as a measure target by clicking the  ***"Capture"*** .You could see the picture you tacke shows on the window. If you click the ***"Close Camera"*** ， the camera will be closed.
Or you could click ***"Local Image"*** to select an image in your computer to test the program. Here we provide several standard image for you to test.
- Click the ***"Parameter Setting"*** button to pop up the parameter setting interface as shown in figure. In this interface, you can modify the parameters that the core algorithm depend on.
- Checking "Use Example" box to enter the ***"demo mode"*** which allow you to roughly experience this algorithm. In this mode, by click "Squre detect" or "Circle detect" button, program will detect the size of target squre or circle and output the result in the white text area above the main interface.
- When the input image is not a pure gray background, click the ***"Square-dectect"*** button to enter the parallel line spacing detection process, and pop up the parallel line selection form. At this time, use the mouse to select two parallel lines in the picture in the form. Pressing the ***Esc*** button will start measuring the parallel line spacing and the measurement will be displayed in the white text area. When the image area to be measured is not a pure gray background, click the Circle-dectect button to enter the circle size measurement process, and pop up the circle selection form. At this time, select a circle with the mouse frame to select the circle in the form image, and then Pressing the ***Esc*** button will start the circle size measurement, and the measurement results (radius, center position) will be displayed in the white text area.
	
	
## Some results

![image](https://github.com/swq123459/swq123456-readmePicture/blob/master/subpixel_pic/sample.png?raw=true)
![image](https://github.com/swq123459/swq123456-readmePicture/blob/master/subpixel_pic/main-origin.png?raw=true)
## How it Works
- my paper: SUBPIXEL EDGE DETECTION BASED ON EDGE GRADIENT DIRECTIONAL INTERPOLATION AND ZERNIKE MOMENT
http://www.dpi-proceedings.com/index.php/dtcse/article/view/24488

