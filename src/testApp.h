#pragma once

#include "ofMain.h"
#include "ofxKinect.h"
#include "threadedObject.h"

class testApp : public ofBaseApp{

	public:
    // ========== System Functions ==========
		void setup();
		void update();
		void draw();
        void exit();
        
        void keyPressed(int key);
        void mouseDragged(int x, int y, int button);
        void mousePressed(int x, int y, int button);
        void mouseReleased(int x, int y, int button);
        void windowResized(int w, int h);
		
    // ========== Kinect Functions ==========
        void drawPointCloud();
        ofxKinect kinect;
    
        int nearThreshold;
        int farThreshold;
    
        int angle;
    
        ofEasyCam easyCam;
    
    // ========== File Functions ==========
        bool recording;
        string sceneName;
        threadedObject TO;
        ofBuffer vertData;
        ofBuffer colData;
        
        int step;
    
    // ========== File Functions ==========
    
    ofSoundPlayer finis;
};
