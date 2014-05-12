AVblocks_5
==========

* OF 0.8.0 synth and audio tools


Addons needed : 
____________
ofxMidi

Copyright (c) 2013 Dan Wilcox <danomatika@gmail.com>

https://github.com/danomatika/ofxMidi 
____________
ofxUI

Copyright (C) 2012 Syed Reza Ali (www.syedrezaali.com)

https://github.com/rezaali/ofxUI
____________

Parameters scheme
=================



/* ===== .h file    ===== */ 


//  define ofParameter

ofParameter<bool> masterPower; 


// declare listener function 

void synthmasterPower(bool & _value);



/* =====  .cpp file =====*/


// setup

// bind ofParameter to listening function

masterPower.addListener(this, &testApp::synthmasterPower);

// add GUI element (inside setup)

gui->addWidgetDown(new ofxUIToggle(32, 32, false, "Master Power"));

// outside setup

// implement listener function

void testApp::synthmasterPower(bool & _value){ 

// let the synth do something with the parameter

mysynth.setParam (0, masterPower);} // here parameter number and parameter name are synth defined AVparameters.h

// and bind Gui widget to ofParameter changes

ofxUIToggle * t = (ofxUIToggle*) guiOSC->getWidget("ON-OFF"); t->setValue(masterPower);

// inside Gui event listener function, link GUI events with ofParameters

else if(e.widget->getName() == "Master Power")
    {
        ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
        masterPower = (toggle->getValue());
    }
