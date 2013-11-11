#include "testApp.h"

#include <cmath>
#include <iostream>

ofMesh buccoli() {
	ofMesh mesh;

	mesh.setMode(OF_PRIMITIVE_POINTS);

	for (int i=0; i<=200; ++i) {
		for (int j=0; j<= 25; ++j) {
			float y = (.7 + .2*sin(21.*j/250. * PI))*cos(i/20. * PI);
			float x = 39.*i/1000. + 1.5*sin(j/50. * PI) - 39./10 - .75;
			float z = (.7 + .2*sin(21.*j/250. *PI))*sin(-i/20. *PI);
				mesh.addVertex(ofVec3f(100*x,100*y,100*z));
		}
	}

	return mesh;
}

ofMesh fiocchiRigati() {
	ofMesh mesh;

	mesh.setMode(OF_PRIMITIVE_POINTS);

	for (int i=0; i<=80; ++i) {
		for (int j=0; j<=80; ++j) {
			float y = -15. + 30.*i/80.;
			if (20 <= i && i <=60) {
				y += 7. * pow(sin(PI * (i+40.)/40.), 3.)*pow(sin(PI * (j+110.)/100.), 9.);
			} else {
				y += 10.*cos(PI * (i+80.)/80.)*pow(sin(PI * (j + 110.)/100.), 9.);
			}

			float x = -17. + 35.*j/80. + 4.*sin(PI * i/80.)*sin(PI * (j-10.)/120.)
						- 4.*sin(PI * i/80.)*sin(PI * (70.-j)/120.);
			float z = 3.*sin(PI * (i+10.)/20.)*pow(sin(PI * j/80.), 1.5)
						- 0.7*pow( (sin( 3.*PI* j/8.) + 1.)/2., 4.);
			mesh.addVertex(ofVec3f(20*x, 20*y, 20*z));
		}
	}

	return mesh;
}

ofMesh tortellini() {
	ofMesh mesh;

	mesh.setMode(OF_PRIMITIVE_POINTS);

	for (int i=0; i<=120; ++i) {
		for (int j=0; j<=60; ++j) {
			double alpha = 0.2*sin( PI * i /120.) + j/400.;
			double beta = cos(PI * j/60. * (2.7 + 0.2*pow(sin(PI * i /120.), 50.)) + 1.4* PI);

			float x = pow(.5, 1. + .5*sin(PI * i / 120.)) * cos(PI * (11.*i-60.)/600.)
						*(1.35 + (3. + sin(PI*i/120.))*alpha*beta);
			float y = 0.5*sin(PI * (11.*i-60.)/600.)*(1.35 + (0.6 + sin(PI*i/120.))*alpha*beta);
			float z = 0.15 + i/1200. + 0.5*(0.8 * sin(PI * i /120.) + j/400.)*sin(PI * i/120.)
						* sin (PI * j/60. *(2.7 + 0.2*pow(sin(PI*i/120.), 50.)) + 1.4 * PI);
			mesh.addVertex(ofVec3f(400*x, 400*y, 400*z));
		}
	}

	return mesh;
}

ofMesh spirali() {
	ofMesh mesh;

	mesh.setMode(OF_PRIMITIVE_TRIANGLES);

	const int DIM_I = 100;
	const int JMAX = 120;

	for (int j=0; j<=JMAX; ++j) {
		for (int i=0; i<DIM_I; ++i) {
			float a = 2.5 + 2.*cos(PI * i/50.)+0.1*cos(PI * i/5.);
			float z = a*cos(PI * j/30.);
			float y = a*sin(PI * j/30.);
			float x = -12.5 + 2.5 + 2.*sin(PI * i/50.) + 0.1*sin(PI * i/5.) + j/6.;
			mesh.addVertex(ofVec3f(50*x, 50*y, 50*z));
		}
	}

	// for each j,i create face (j,i) -> (j,i+1) -> (j+1, i+1) -> (j+1, i),
	// using 2 triangles.  Store normal of first rectangle to compute vertex normals in next step.
	vector<ofVec3f> surfaceNormals;
	for (int j=0; j<JMAX; ++j) {
		int rowstart = j*DIM_I;
		for (int i=0; i<DIM_I; ++i) {
			int n0 = rowstart + i;
			int n1 = rowstart + (i+1)%(DIM_I);
			int n2 = n1 + (DIM_I);
			int n3 = n0 + (DIM_I);

			mesh.addTriangle(n0,n1,n2);
			mesh.addTriangle(n0,n2,n3);

			ofVec3f vec0 = mesh.getVertex(n0);
			ofVec3f vec01 = mesh.getVertex(n1)-vec0;
			ofVec3f vec02 = mesh.getVertex(n2)-vec0;

			surfaceNormals.push_back(vec01.crossed(vec02).normalize());

		}
	}

	// set vertex normal to the average of the surrounding surface normals:
	// j=0 boundary vertices
	for(int i=0; i<DIM_I; ++i) {
		int lowright = i;
		int lowleft = (i+DIM_I-1)%(DIM_I);
		mesh.addNormal(((surfaceNormals[lowright] + surfaceNormals[lowleft])/2).normalize());
	}
	// j=1 to j=JMAX-1
	for(int j=1; j<JMAX; ++j) {
		int toprow = (j-1)*DIM_I;
		for(int i=0; i<DIM_I; ++i) {
			int topright = toprow + i;
			int topleft = toprow + (i+DIM_I-1)%DIM_I;
			int lowright = topright + DIM_I;
			int lowleft = topleft + DIM_I;
			mesh.addNormal(((surfaceNormals[lowright] + surfaceNormals[topright]
			                 + surfaceNormals[topleft] + surfaceNormals[lowleft])/4).normalize());
		}
	}
	// j=JMAX boundary vertices
	for(int i=0; i<DIM_I; ++i) {
		int topright = (JMAX-1)*(DIM_I) + i;
		int topleft = (JMAX-1)*(DIM_I) + (i+DIM_I-1)%(DIM_I);
		mesh.addNormal(((surfaceNormals[topright]+surfaceNormals[topleft])/2.).normalize());
	}

	return mesh;
}

//--------------------------------------------------------------
void testApp::setup(){
	ofLog(OF_LOG_NOTICE, "test setup");

	ofSetGlobalAmbientColor(ofColor(128,50,50));
	spotlight.setPointLight();
	spotlight.setGlobalPosition(-400,400,400);
	fixedlight.setDiffuseColor(ofColor(100,100,200));
	fixedlight.setGlobalPosition(400,400,400);
	spotlight.setDiffuseColor(ofColor(200,200,50));

	meshes.push_back(buccoli());
	meshes.push_back(fiocchiRigati());
	meshes.push_back(tortellini());
	meshes.push_back(spirali());

	curMesh = meshes.begin();

	ofEnableDepthTest();
	glEnable(GL_POINT_SMOOTH); // use circular points instead of square points
	glPointSize(4); // make the points bigger
}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){
	ofBackgroundGradient(ofColor::gray, ofColor::black, OF_GRADIENT_CIRCULAR);

	fixedlight.enable();

	cam.begin();
	spotlight.enable();
	curMesh->draw();
	spotlight.disable();
	cam.end();

	fixedlight.disable();

}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	switch(key) {
	case OF_KEY_RIGHT:
		++curMesh;
		if(curMesh == meshes.end())
			curMesh = meshes.begin();
		break;
	case OF_KEY_LEFT:
		if(curMesh == meshes.begin())
			curMesh = meshes.end();

		--curMesh;
		break;
	}
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

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

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}
