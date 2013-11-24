#include "testApp.h"

unsigned long long initialTime = 0;
int frameTime = 0;

//--------------------------------------------------------------
void testApp::setup(){
    // ========================================================================
    // WRITE SCENE NAME TO START CAPTURING
    // IT WILL CREATE A SCENE DIRECTORY AND PUT FRAMES INTO IT AFTER PRESSING R
    // ========================================================================
    sceneName = "take-02";
    step = 4;
    ofSetFrameRate(30);
    // ========================================================================
    // ========================================================================
    
    
    ofDirectory dir;
    dir.createDirectory(sceneName);
    
    recording = false;
	
	// enable depth->video image calibration
	kinect.setRegistration(true);
    
	kinect.init();
	//kinect.init(true); // shows infrared instead of RGB video image
	//kinect.init(false, false); // disable video image (faster fps)
	
	kinect.open();		// opens first available kinect
	//kinect.open(1);	// open a kinect by id, starting with 0 (sorted by serial # lexicographically))
	//kinect.open("A00362A08602047A");	// open a kinect using it's unique serial #
	
	// print the intrinsic IR sensor values
	if(kinect.isConnected()) {
		ofLogNotice() << "sensor-emitter dist: " << kinect.getSensorEmitterDistance() << "cm";
		ofLogNotice() << "sensor-camera dist:  " << kinect.getSensorCameraDistance() << "cm";
		ofLogNotice() << "zero plane pixel size: " << kinect.getZeroPlanePixelSize() << "mm";
		ofLogNotice() << "zero plane dist: " << kinect.getZeroPlaneDistance() << "mm";
	}
    
    nearThreshold = 0;
	farThreshold = 1000;
    
    // zero the tilt on startup
	angle = 0;
	kinect.setCameraTiltAngle(angle);
    
    finis.loadSound("finis-terre.wav");
    finis.play();
    finis.setPaused(!recording);
}

//--------------------------------------------------------------
void testApp::update(){
	ofBackground(0, 0, 0);
	
	kinect.update();
}

//--------------------------------------------------------------
void testApp::draw(){
    ofSetColor(255, 255, 255);
    stringstream reportStream;
    
    easyCam.begin();
    drawPointCloud();
    easyCam.end();
    
    reportStream << "fps: " << ofGetFrameRate() << endl << "recording: " << recording << endl
    << "frame time: " << frameTime << endl;
    ofDrawBitmapString(reportStream.str(), 20, 652);
}

//--------------------------------------------------------------
void testApp::drawPointCloud() {
    //if (kinect.isFrameNewDepth()) {
        int w = 640;
        int h = 480;
        ofMesh mesh;
        mesh.setMode(OF_PRIMITIVE_POINTS);
        int id = 0;
        
        vertData.clear();
        colData.clear();
        
        for(int y = 0; y < h; y += step) {
            for(int x = 0; x < w; x += step) {
                mesh.addVertex(kinect.getWorldCoordinateAt(x, y));
                mesh.addColor(kinect.getColorAt(x,y));
                id++;
            }
        }
        
        vertData = ofBuffer((char*)mesh.getVertices().data(), mesh.getNumVertices()*sizeof(ofVec3f));
        colData = ofBuffer((char*)mesh.getColors().data(), mesh.getNumColors()*sizeof(ofFloatColor));
    if (recording)
        frameTime = (int) (ofGetSystemTimeMicros() - initialTime)/3000;
    
        if (vertData.size() > 0 && recording)
            TO.start(vertData, colData, sceneName, frameTime);
        
        glPointSize(3);
        ofPushMatrix();
        // the projected points are 'upside down' and 'backwards'
        ofScale(1, -1, -1);
        ofTranslate(0, 0, -1000); // center the points a bit
        ofEnableDepthTest();
        mesh.drawVertices();
        ofDisableDepthTest();
        ofPopMatrix();
    //}
}

//--------------------------------------------------------------
void testApp::exit() {
	kinect.setCameraTiltAngle(0); // zero the tilt on exit
	kinect.close();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    switch (key) {
    case 'a':
        farThreshold += 10;
        ofLogNotice() << "sensor-emitter dist: " << farThreshold;
        break;
        
    case 'z':
        farThreshold -=10;
        ofLogNotice() << "sensor-emitter dist: " << farThreshold;
        break;
        
    case 's':
        nearThreshold +=10;
        ofLogNotice() << "sensor-emitter dist: " << nearThreshold;
        break;
        
    case 'x':
        nearThreshold -=10;
        ofLogNotice() << "sensor-emitter dist: " << nearThreshold;
        break;
        
    case 'o':
        kinect.setCameraTiltAngle(angle); // go back to prev tilt
        kinect.open();
        break;
        
    case 'c':
        kinect.setCameraTiltAngle(0); // zero the tilt
        kinect.close();
        break;
    
    case 'r':
        recording = !recording;
        finis.setPositionMS(0);
        finis.setPaused(!recording);
        initialTime = ofGetSystemTimeMicros();
        break;
        
    case OF_KEY_UP:
        angle++;
        if(angle>30) angle=30;
        kinect.setCameraTiltAngle(angle);
        break;
        
    case OF_KEY_DOWN:
        angle--;
        if(angle<-30) angle=-30;
        kinect.setCameraTiltAngle(angle);
        break;
    }

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}
