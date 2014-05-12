/********************************************************************************** 
 
 Copyright (C) 2012 Syed Reza Ali (www.syedrezaali.com)
 
 Permission is hereby granted, free of charge, to any person obtaining a copy of
 this software and associated documentation files (the "Software"), to deal in
 the Software without restriction, including without limitation the rights to
 use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 of the Software, and to permit persons to whom the Software is furnished to do
 so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 
 **********************************************************************************/

#ifndef OFXUI_SLIDER
#define OFXUI_SLIDER

#include "ofxUIWidgetWithLabel.h"


class ofxUISlider : public ofxUIWidgetWithLabel
{
public:
    
    
    ofxUISlider() : ofxUIWidgetWithLabel()
    {
    
    }
    
    ofxUISlider(string _name, float _min, float _max, float _value, float w, float h, float x = 0, float y = 0) : ofxUIWidgetWithLabel()
    {
        useReference = false;
        init(_name, _min, _max, &_value, w, h, x, y);
    }

    ofxUISlider(string _name, float _min, float _max, float *_value, float w, float h, float x = 0, float y = 0) : ofxUIWidgetWithLabel()
    {
        useReference = true;
        init(_name, _min, _max, _value, w, h, x, y);
    }
    
    // DON'T USE THE NEXT CONSTRUCTORS
    // This is maintained for backward compatibility and will be removed on future releases
    
    ofxUISlider(float x, float y, float w, float h, float _min, float _max, float _value, string _name) : ofxUIWidgetWithLabel()
    {
        useReference = false;         
        init(_name, _min, _max, &_value, w, h, x, y);
//        ofLogWarning("OFXUISLIDER: DON'T USE THIS CONSTRUCTOR. THIS WILL BE REMOVED ON FUTURE RELEASES.");
    }
    
    ofxUISlider(float w, float h, float _min, float _max, float _value, string _name) : ofxUIWidgetWithLabel()
    {
        useReference = false;         
        init(_name, _min, _max, &_value, w, h, 0, 0);
//        ofLogWarning("OFXUISLIDER: DON'T USE THIS CONSTRUCTOR. THIS WILL BE REMOVED ON FUTURE RELEASES.");
    }
    
    ofxUISlider(float x, float y, float w, float h, float _min, float _max, float *_value, string _name) : ofxUIWidgetWithLabel()
    {
        useReference = true; 
        init(_name, _min, _max, _value, w, h, x, y);
//        ofLogWarning("OFXUISLIDER: DON'T USE THIS CONSTRUCTOR. THIS WILL BE REMOVED ON FUTURE RELEASES.");
    }
    
    ofxUISlider(float w, float h, float _min, float _max, float *_value, string _name) : ofxUIWidgetWithLabel()
    {
        useReference = true; 
        init(_name, _min, _max, _value, w, h, 0, 0);
//        ofLogWarning("OFXUISLIDER: DON'T USE THIS CONSTRUCTOR. THIS WILL BE REMOVED ON FUTURE RELEASES.");
    }
    
    ~ofxUISlider()
    {
        if(!useReference)
        {
            delete valueRef; 
        }
    }   
    
    virtual void init(string _name, float _min, float _max, float *_value, float w, float h, float x, float y)
    {
        renderMode = 2; // indicates how to render sliders
        
        rect = new ofxUIRectangle(x,y,w,h);
        name = string(_name);  				
		if(w > h)
		{
			kind = OFX_UI_WIDGET_SLIDER_H;  			
		}
		else 
		{
			kind = OFX_UI_WIDGET_SLIDER_V;  			
		}
        
		paddedRect = new ofxUIRectangle(-padding, -padding, w+padding*2.0, h+padding);
		paddedRect->setParent(rect);     
        
        draw_fill = true; 
        
        value = *_value;                                               //the widget's value
        if(useReference)
        {
            valueRef = _value; 
        }
        else
        {
            valueRef = new float(); 
            *valueRef = value; 
        }

		max = _max;
		min = _min; 
        labelPrecision = 2;    
        
		if(value > max)
		{
			value = max; 
		}
		if(value < min)
		{
			value = min; 
		}
		
		value = ofxUIMap(value, min, max, 0.0, 1.0, true); 
        
		if(kind == OFX_UI_WIDGET_SLIDER_H)
		{
			label = new ofxUILabel(0,h+padding,string(name+" LABEL"), string(name + ": " + ofxUIToString(max,labelPrecision)), OFX_UI_FONT_SMALL);
		}
		else 
		{
			label = new ofxUILabel(0,h+padding,string(name+" LABEL"), string(name), OFX_UI_FONT_SMALL);
		}
        

		label->setParent(label);
		label->setRectParent(rect);
        label->setEmbedded(true);
        
        //label->setYOffset(-20);
        
        //increment = fabs(max - min) / 100.0;
        
        float diff = fabs(max-min);
        if(diff<=1.0) increment = 0.01;
        else if( (diff>1.0) && (diff<=16)) increment = 0.1;
        else if( (diff>16) && (diff<=32)) increment = 0.1;
        else if( (diff>32) && (diff<=128)) increment = 0.5;
        
        bRoundedToNearestInt = false;
        bClampValue = true;
    }
    
    bool getSetClampValue()
    {
        return bClampValue;
    }
    
    void setClampValue(bool _bClampValue)
    {
        bClampValue = _bClampValue;
    }
    
    virtual void update()
    {
        if(useReference)
        {
            value = ofxUIMap(*valueRef, min, max, 0.0, 1.0, bClampValue);
            updateLabel(); 
        }

    }

    virtual void setDrawPadding(bool _draw_padded_rect)
	{
		draw_padded_rect = _draw_padded_rect; 
        label->setDrawPadding(false);
	}
    
    virtual void setDrawPaddingOutline(bool _draw_padded_rect_outline)
	{
		draw_padded_rect_outline = _draw_padded_rect_outline; 
        label->setDrawPaddingOutline(false);
	}  
    
    virtual void drawBack() 
    {
        if(draw_back)
        {
            ofxUIFill(); 
            ofxUISetColor(color_back);
            ofSetColor(0);
            rect->draw();
            
            
        }
    }
    
    virtual void drawOutline() 
    {
        if(draw_outline)
        {
            ofNoFill();
            ofxUISetColor(color_outline);
            
            ofxUIRectangle rAux = *rect;
            rAux.setX(rect->getX());
            rAux.setY(rect->getY());
            rAux.setWidth(rect->getWidth());
            rAux.setHeight( (rect->getHeight()));
            
            rAux.draw();

            //rect->draw();
        }
    }
    
    virtual void drawOutlineHighlight() 
    {
        if(draw_outline_highlight)
        {
            ofxUIRectangle rAux = *rect;
            switch(renderMode)
            {
                case 1:
                    ofNoFill();
                    ofxUISetColor(color_outline_highlight);
                    rAux.setX(rect->getX());
                    rAux.setY(rect->getY());
                    rAux.setWidth(rect->getWidth());
                    rAux.setHeight( (rect->getHeight()));
                    
                    rAux.draw();

                    //rect->draw();
                    break;
                case 2:
                    ofNoFill();
                    ofxUISetColor(OFX_UI_COLOR_AVBLOCKS_ORANGE*.75);
                    
                    rAux.setX(rect->getX());
                    rAux.setY(rect->getY());
                    rAux.setWidth(rect->getWidth());
                    rAux.setHeight( (rect->getHeight()));
                    
                    rAux.draw();
                    //rect->draw();
                    
                    break;
                    
            }
            if(kind == OFX_UI_WIDGET_SLIDER_V)
            {
                ofxUISetColor(OFX_UI_COLOR_AVBLOCKS_ORANGE);
                label->drawString(rect->getX()+rect->getWidth(),rect->getY()+rect->getHeight()-rect->getHeight()*MIN(MAX(value, 0.0), 1.0) , ofxUIToString(getScaledValue(),labelPrecision));
            }
        }
    }
    
    virtual void drawFill() 
    {
        if(draw_fill && value > 0.0)
        {			
            ofxUIFill(); 
            ofxUISetColor(color_fill); 
			if(kind == OFX_UI_WIDGET_SLIDER_H)
			{			   
                switch(renderMode)
                {
                    case 1 :
                        ofxUIDrawRect(rect->getX(), rect->getY(), rect->getWidth()*MIN(MAX(value, 0.0), 1.0), rect->getHeight());
                        break;
                    case 2:
                        ofEnableAlphaBlending();
                        ofxUISetColor(OFX_UI_COLOR_AVBLOCKS_WHITE);
                        ofxUIDrawRect(rect->getX()+rect->getWidth()*MIN(MAX(value, 0.0), 1.0),rect->getY(), 3,rect->getHeight());
                        break;
                }

			}
			else 
			{
                // VERTICAL SLIDER
                
				switch(renderMode)
                {
                    case 1 :ofxUIDrawRect(rect->getX(), rect->getY()+rect->getHeight(), rect->getWidth(), -rect->getHeight()*MIN(MAX(value, 0.0), 1.0));
                        break;
                    case 2 :
                        ofxUISetColor(OFX_UI_COLOR_AVBLOCKS_WHITE);
                        ofxUIDrawRect(rect->getX(),rect->getY()+rect->getHeight()-rect->getHeight()*MIN(MAX(value, 0.0), 1.0), rect->getWidth(), 3);
                        break;
                }
			}

        }
    }
    
    virtual void drawFillHighlight() 
    {
        if(draw_fill_highlight)
        {
            ofxUIFill(); 
            ofxUISetColor(color_fill_highlight); 
			if(kind == OFX_UI_WIDGET_SLIDER_H)
			{			   
                switch(renderMode)
                {
                    case 1 :
                        ofxUIDrawRect(rect->getX(), rect->getY(), rect->getWidth()*MIN(MAX(value, 0.0), 1.0), rect->getHeight());
                        break;
                    case 2:
                        ofEnableAlphaBlending();
                        ofxUISetColor(OFX_UI_COLOR_AVBLOCKS_ORANGE);
                        ofxUIDrawRect(rect->getX()+rect->getWidth()*MIN(MAX(value, 0.0), 1.0),rect->getY(), 3,rect->getHeight());
                        break;
                }
			}
			else
			{
				
                switch(renderMode)
                {
                    case 1 :
                        ofxUIDrawRect(rect->getX(), rect->getY()+rect->getHeight(), rect->getWidth(), -rect->getHeight()*MIN(MAX(value, 0.0), 1.0));
                        break;
                    case 2 :
                        ofEnableAlphaBlending();
                        ofxUISetColor(OFX_UI_COLOR_AVBLOCKS_ORANGE);
                        ofxUIDrawRect(rect->getX(),rect->getY()+rect->getHeight()-rect->getHeight()*MIN(MAX(value, 0.0), 1.0), rect->getWidth(), 3);
                        break;
                }

			}
            
//			if(kind == OFX_UI_WIDGET_SLIDER_V)
//			{
//            ofxUISetColor(OFX_UI_COLOR_AVBLOCKS_WHITE);
//            label->drawString(rect->getX()+padding, rect->getY(), ofxUIToString(getScaledValue(),labelPrecision));
//			}
        }
    }
        
    void mouseMoved(int x, int y ) 
    {
        if(rect->inside(x, y))
        {
            state = OFX_UI_STATE_OVER;         
        }    
        else
        {
            state = OFX_UI_STATE_NORMAL;        
        }
        stateChange();         
    }
    
    void mouseDragged(int x, int y, int button) 
    {
        if(hit && OF_KEY_COMMAND)
        {
            state = OFX_UI_STATE_DOWN;     
			input(x, y); 
			triggerEvent(this); 			
        }    
        else
        {
            state = OFX_UI_STATE_NORMAL;        
        }
        stateChange();     
    }
    
    void mousePressed(int x, int y, int button) 
    {

        if(rect->inside(x, y))
        {
            hit = true; 
            state = OFX_UI_STATE_DOWN;     
			input(x, y); 
			triggerEvent(this); 
        }    
        else
        {
            state = OFX_UI_STATE_NORMAL;        
        }
        //if(hit) if(ofGetKeyPressed(OF_KEY_ALT)) setValue(0.5);

        stateChange();
        
    }
    
    void mouseReleased(int x, int y, int button) 
    {
        if(hit)
        {
#ifdef TARGET_OPENGLES
            state = OFX_UI_STATE_NORMAL;        
#else            
            state = OFX_UI_STATE_OVER; 
#endif            
			input(x, y); 
			triggerEvent(this); 			
        }    
        else
        {
            state = OFX_UI_STATE_NORMAL;         
        }
        //if(hit) if(ofGetKeyPressed(OF_KEY_ALT)) setValue(0.5);
        stateChange();
        hit = false;
        
    }
	
    void keyPressed(int key) 
    {		
		if(state == OFX_UI_STATE_OVER || state == OFX_UI_STATE_DOWN)
		{
			switch (key) 
			{
				case OF_KEY_RIGHT:
					setValue(getScaledValue()+increment); 
					triggerEvent(this); 
					break;
                    
				case OF_KEY_UP:
					setValue(getScaledValue()+increment); 
					triggerEvent(this); 
					break;
					
				case OF_KEY_LEFT:
					setValue(getScaledValue()-increment); 					
					triggerEvent(this); 
					break;
                    
				case OF_KEY_DOWN:
					setValue(getScaledValue()-increment); 					
					triggerEvent(this);     
					break;					
                    
                case OF_KEY_SHIFT:
#if OF_VERSION_MINOR > 7
                case OF_KEY_LEFT_SHIFT:
                case OF_KEY_RIGHT_SHIFT:
#endif
                    bRoundedToNearestInt = true;
                    break;
				default:
					break;
			}
		}
    }
    
    void keyReleased(int key) 
    {
        bRoundedToNearestInt = false; 
    }
	
    void windowResized(int w, int h)
	{            
		
    }
    
    float getIncrement()
    {
        return increment; 
    }
    
    void setIncrement(float _increment)
	{
		increment = _increment; 
	}
    
	virtual void input(float x, float y)
	{
		if(kind == OFX_UI_WIDGET_SLIDER_H)
		{
			value = rect->percentInside(x, y).x; 
		}
		else 
		{
			value = 1.0-rect->percentInside(x, y).y; 
		}	
        
        if(value > 1.0)
        {
            value = 1.0;
        }
        else if(value < 0.0)
        {
            value = 0.0;
        }
        
        // round value to increment
        int divisions = int(float(getScaledValue())/float(increment));
        //printf("%s (value %f scaled is : %f  -- incr. %f) :: divisions  %d newValue %f\n",name.c_str(),value,getScaledValue(),increment,divisions,float(divisions) * float(increment));
        float wantedValue = float(divisions) * float(increment);
        
        value = ofxUIMap(wantedValue,min, max, 0.0,1.0,bClampValue);
        
        updateValueRef();
		updateLabel(); 
	}
    
    void updateValueRef()
    {
        (*valueRef) = bRoundedToNearestInt ? ceil(getScaledValue()) : getScaledValue();  
    }    

	virtual void updateLabel()
	{
		if(kind == OFX_UI_WIDGET_SLIDER_H)
		{
			label->setLabel(name + " " + ofxUIToString(getScaledValue(),labelPrecision));
		}
        label->setYOffset(6);

        
	}
	
    virtual void stateChange()
    {                
        switch (state) {
            case OFX_UI_STATE_NORMAL:
            {
                draw_fill_highlight = false;
                draw_outline_highlight = false;             
				label->unfocus(); 				
            }
                break;
            case OFX_UI_STATE_OVER:
            {
                draw_fill_highlight = false;            
                draw_outline_highlight = true;    
				label->unfocus(); 
            }
                break;
            case OFX_UI_STATE_DOWN:
            {
                draw_fill_highlight = true;            
                draw_outline_highlight = true;             
				label->focus(); 				
            }
                break;
            case OFX_UI_STATE_SUSTAINED:
            {
                draw_fill_highlight = false;            
                draw_outline_highlight = false;
				label->unfocus(); 				
            }
                break;            
                
            default:
                break;
        }
    }
	
	void setValue(float _value)
	{
		value = ofxUIMap(_value, min, max, 0.0, 1.0, bClampValue);		
        updateValueRef();        
		updateLabel(); 		
	}
		
	float getValue()
	{
		return value; 
	}
	
	float getPercentValue()
	{
		return value; 
	}
	
	float getScaledValue()
	{
		return ofxUIMap(value, 0.0, 1.0, min, max, bClampValue);
	}
    
	ofxUILabel *getLabel()
	{
		return label; 
	}
    
    void setLabelVisible(bool _labelVisible)
    {
        label->setVisible(_labelVisible);
        paddedRect->height -= label->getPaddingRect()->height;        
    }
    
    void setVisible(bool _visible)
    {
        visible = _visible; 
        label->setVisible(visible);
    }
	
	virtual void setParent(ofxUIWidget *_parent)
	{
		parent = _parent; 
        label->getRect()->setY(rect->getHeight()+padding);
        paddedRect->height = rect->getHeight() + label->getPaddingRect()->height + padding;
        paddedRect->x = -padding;
        paddedRect->y = -padding; 
        if(label->getPaddingRect()->width > paddedRect->width)
        {
            paddedRect->width = label->getPaddingRect()->width;				
        }
        updateValueRef();                
        updateLabel(); 
	}
	
    void setLabelPrecision(int _precision) {
        labelPrecision = _precision;
        updateValueRef();        
        updateLabel();
    }
    
    void setMax(float _max, bool bKeepValueTheSame = false)
    {
        setMaxAndMin(_max, min, bKeepValueTheSame);
    }

    float getMax()
    {
        return max; 
    }
    
    void setMin(float _min, bool bKeepValueTheSame = false)
    {
        setMaxAndMin(max, _min, bKeepValueTheSame);
    }
    
    float getMin()
    {
        return min;
    }
    
    ofxUIVec2f getMaxAndMin()
    {
        return ofxUIVec2f(max, min); 
    }
    
    void setMaxAndMin(float _max, float _min, bool bKeepValueTheSame = false)
    {
        max = _max;
        min = _min;
        
        if(!bKeepValueTheSame)
        {
            value = ofxUIMap(value, 0, 1.0, min, max, bClampValue);
            value = ofxUIMap(value, min, max, 0.0, 1.0, bClampValue);
            updateValueRef();
            updateLabel();
        }
    }

    bool isDraggable()
    {
        return true; 
    }
    
    void setRenderMode(int m)
    {
        renderMode=m;
    }
    
protected:    //inherited: ofxUIRectangle *rect; ofxUIWidget *parent; 
    bool bRoundedToNearestInt;
    bool bClampValue; 
    float value, increment;
    float *valueRef; 
    bool useReference;     
	float max, min;  
    int labelPrecision;

    int renderMode;
    ofxUIRectangle rAux;

    
    

}; 

#endif
