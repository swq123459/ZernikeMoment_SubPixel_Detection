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

Bacause we also tested our code on Linux device, it is no doubt that you can achieve all the function of this demo on (or saay only on) Linux device.So if you want to use this code on Linux, it is really fortunate.But if you are a windows user, don't worry,please look down.
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


## How it Works


