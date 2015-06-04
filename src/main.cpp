///////////////////////////////////////
//Developed By	:- Saiyad Anwar Ali  //
//Contact No.	:- +91 7869550197    //
//Email Address	:- 13Anwar2@gmail.com//
///////////////////////////////////////
#include<stdio.h>
#include<iostream>
#include<opencv2\objdetect\objdetect.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<opencv2\imgproc\imgproc.hpp>
#include<vector>
#include<Windows.h>

using namespace cv;
using namespace std;

int fcount=0;
int c=0, f=0, c1=0, f1=0, area;

int main()
{ 
	CascadeClassifier hand_cascade, palm_cascade;
	int iLastX = -1; 
	int iLastY = -1;
 	int screenx = GetSystemMetrics(SM_CXSCREEN);
	int screeny = GetSystemMetrics(SM_CYSCREEN);
	float handPos, minFWidth, maxFWidth, minFHeight, maxFHeight;
	float avgPos,avgPos1 = 0;
	bool MediaControlModeEnabled = false;
	bool PresentationModeEnabled = false;
	bool Fist = false;
	bool Palm = false;
	
	if(!hand_cascade.load("fist.xml")) {
		printf("Error loading cascade file for fist");
		return 1;
	}

	if(!palm_cascade.load("palm.xml")) {
		printf("Error loading cascade file for palm");
		return 1;
 	}
	
	VideoCapture capture(0);
	if(!capture.isOpened())
	{
 	 	 printf("error to initialize camera");
		return 1;
	}

	Mat cap_img,gray_img;
	vector<Rect> hands, palms;
	vector<vector<Point> > contours;
	Mat frame;
	
	while(1)
	{
		capture >> cap_img;
		waitKey(1);
		cvtColor(cap_img, gray_img, CV_BGR2GRAY);
		cv::equalizeHist(gray_img,gray_img);

		if(Fist){
		hand_cascade.detectMultiScale(gray_img, hands, 1.1, 10, CV_HAAR_SCALE_IMAGE | CV_HAAR_DO_CANNY_PRUNING, cvSize(0,0), cvSize(200,200));
		}

		if(Palm){
		palm_cascade.detectMultiScale(gray_img, palms, 1.1, 10, CV_HAAR_SCALE_IMAGE | CV_HAAR_DO_CANNY_PRUNING, cvSize(0,0), cvSize(100,100));
		}
		
		char TestStr[100];   
		sprintf(TestStr,"Developed by - Anwar");  
		putText(cap_img , TestStr, Point(10,470), FONT_HERSHEY_PLAIN, 2, Scalar(255,255,255),1,1);
		putText(cap_img , "To Enable or Disable Presentation Control Press 'd'", Point(10,20), FONT_HERSHEY_PLAIN, 1, Scalar(255,255,255),1,1);
		putText(cap_img , "To Enable or Disable Media Control Press 't'", Point(10,40), FONT_HERSHEY_PLAIN, 1, Scalar(255,255,255),1,1);
		//putText(cap_img , "To Enable or Disable Fist 'f'", Point(10,60), FONT_HERSHEY_PLAIN, 1, Scalar(255,255,255),1,1);
		//putText(cap_img , "To Enable or Disable Palm 'p'", Point(10,80), FONT_HERSHEY_PLAIN, 1, Scalar(255,255,255),1,1);


		for(int i=0; i < hands.size();i++)
		{
			Point pt1(hands[i].x+hands[i].width, hands[i].y+hands[i].height);
			Point pt2(hands[i].x,hands[i].y);			
			rectangle(cap_img, pt1, pt2, cvScalar(0,255,0), 2, 8, 0);
		}

		for(int k=0; k < palms.size();k++)
		{
			Point pt1(palms[k].x+palms[k].width, palms[k].y+palms[k].height);
			Point pt2(palms[k].x,palms[k].y);			
			rectangle(cap_img, pt1, pt2, cvScalar(0,255,225), 2, 8, 0);
		}

		for(size_t i = 0; i < hands.size(); i++)
		{
			avgPos = (hands[i].x + hands[i].width/2);
			avgPos = avgPos / hands.size();
			for(size_t j = 0; j < hands.size(); j++)
				{
					avgPos1 = (hands[j].y + hands[j].height/2);
					avgPos1 = avgPos1 / hands.size();
				}
		}

		int posX = int((avgPos)*(screenx/640.0));
		int posY = int((avgPos1*screeny/480.0));
			


		if(MediaControlModeEnabled && PresentationModeEnabled == false){
			capture.read(frame);
			maxFWidth = (0.65) * capture.get(CV_CAP_PROP_FRAME_WIDTH);
			minFWidth = (0.35) * capture.get(CV_CAP_PROP_FRAME_WIDTH);

			maxFHeight = (0.65) * capture.get(CV_CAP_PROP_FRAME_HEIGHT);
			minFHeight = (0.35) * capture.get(CV_CAP_PROP_FRAME_HEIGHT);

			if(hands.size() < 1)
			{
				c++;
				if(c <= 1)
				{
					f=1;
					printf("PAUSE\n");
				}
			}
			else
			{
				if(f == 1)
				{
					keybd_event(VK_SPACE, 0x32, 0, 0);				
					f=0;
					c=0;
					printf("PLAY\n");
				}
			}

			if(palms.size() < 1)
			{
				c1++;
				if(c1 <= 1)
				{
					f1=1;
					printf("FULLSCREEN OFF\n");
				}
			}
			else
			{
				if(f1 == 1)
				{
					keybd_event(VK_F11, 0x7A, 0, 0); 				
					f1=0;
					c1=0;
					printf("FULLSCREEN ON\n");
				}
			}

			if(avgPos > maxFWidth && hands.size() >= 1){
				keybd_event(VK_LEFT, 0x32, 0, 0);
			 	printf("BACKWARD\n");
			} 
			 
			else  if(avgPos < minFWidth && hands.size() >= 1){
				keybd_event(VK_RIGHT, 0x32, 0, 0);
				printf("FORWARD\n");
			}

			if(avgPos1 > maxFHeight && hands.size() >= 1){
				keybd_event(VK_DOWN, 0x32, 0, 0);
				printf("VOLUME DOWN\n");
			}

			else if(avgPos1 < minFHeight && hands.size() >= 1){
				keybd_event(VK_UP, 0x32, 0, 0);
				printf("VOLUME UP\n");
			}
		}

		
		
		if(MediaControlModeEnabled == true && PresentationModeEnabled == false){
			putText(cap_img , "Media Conrol Mode Enabled", Point(10,100), FONT_HERSHEY_PLAIN, 1, Scalar(0,255,0),1,1);
		}

		else{
			putText(cap_img , "Media Conrol Mode Disabled", Point(10,100), FONT_HERSHEY_PLAIN, 1, Scalar(0,0,255),1,1);
		}

		if(PresentationModeEnabled == true && MediaControlModeEnabled == false){
			putText(cap_img , "Presentation Mode Enabled", Point(10,120), FONT_HERSHEY_PLAIN, 1, Scalar(0,255,0),1,1);
		}

		else{
			putText(cap_img , "Presentation Mode Disabled", Point(10,120), FONT_HERSHEY_PLAIN, 1, Scalar(0,0,255),1,1);
		}

		

		if(PresentationModeEnabled && MediaControlModeEnabled == false){
			long double vel;
			static double dis=0;
			double dis1=0;
			long double x,y;
			int xx,yy;

			fcount++;
			x = (iLastX-posX);
			xx = (posX-iLastX);
			//x=x*x;
			y = (iLastY-posY);
			yy = (iLastY-posY);
			//y=y*y;
			dis1=sqrt((x*x)+(y*y));
			dis=dis+dis1;
			if(fcount==4)
			{
			dis=dis/4;
			vel=dis/.587;
			//Cleanup
			fcount=0;
			dis=0;
			//cout<<"vel "<<vel<<endl;
			double angle = atan2(y, xx);
			angle = (angle * 180 / 3.1415926) + (angle > 0 ? 0 : 360);
	
        
	   if (iLastX >= 0 && iLastY >= 0 && posX >= 0 && posY >= 0)
	   {
			if((vel > 120) && ((angle >= 315) || (angle <= 45)) && hands.size() >= 1){
			printf("Right\n");
			//keybd_event(VK_RIGHT, 0x32, 0, 0);
			keybd_event(VK_RIGHT,0,KEYEVENTF_EXTENDEDKEY|0,0);
			keybd_event(VK_RIGHT,0,KEYEVENTF_EXTENDEDKEY|KEYEVENTF_KEYUP,0);
			keybd_event(VK_NEXT,0,KEYEVENTF_EXTENDEDKEY|0,0);
			keybd_event(VK_NEXT,0,KEYEVENTF_EXTENDEDKEY|KEYEVENTF_KEYUP,0);
			
			}

			if((vel > 120) && ((angle >= 135) && (angle <= 225)) && hands.size() >= 1){
			printf("Left\n");
			//keybd_event(VK_LEFT, 0x32, 0, 0);
			keybd_event(VK_LEFT,0,KEYEVENTF_EXTENDEDKEY|0,0);
			keybd_event(VK_LEFT,0,KEYEVENTF_EXTENDEDKEY|KEYEVENTF_KEYUP,0);
			keybd_event(VK_PRIOR,0,KEYEVENTF_EXTENDEDKEY|0,0);
			keybd_event(VK_PRIOR,0,KEYEVENTF_EXTENDEDKEY|KEYEVENTF_KEYUP,0);
			}
		}
	}
	iLastX = posX;
	iLastY = posY;
}

		switch(waitKey(33)){

			case 27: //'esc' key has been pressed, exit program.
				return 0;
			case 116: //'t' has been pressed. this will toggle media control
				MediaControlModeEnabled = !MediaControlModeEnabled;
				Fist = !Fist;
				Palm = !Palm;
				if(MediaControlModeEnabled == false) cout<<"Media Control Disabled."<<endl;
				else cout<<"Media Control Enabled."<<endl;
				break;
			case 100: //'d' has been pressed. this will toggle presentation control
				PresentationModeEnabled = !PresentationModeEnabled;
				Fist = !Fist;
				if(PresentationModeEnabled == false) cout<<"Presentation Mode Disabled."<<endl;
				else cout<<"Presentation Mode Enabled."<<endl;
				break;
		}
		
		imshow("Result", cap_img);
	}
	return 0;
}
