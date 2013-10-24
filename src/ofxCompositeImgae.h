#pragma once

#include "ofMain.h"
#include "ofxOpenGLContextScope.h"

namespace ofxCompositeImage
{
    class Setting;
    class Material;
    class Composer;
};

#pragma mark - class ofxCompositeImageSetting

class ofxCompositeImage::Setting
{

public:

    Setting();
    Setting(float width, float height, ofColor backgroundColor)
    {
        this->width = width;
        this->height = height;
        this->backgroundColor = backgroundColor;
    }
    
    float getWidth() { return width; }
    float getHeight() { return height; }
    ofColor getBackgroundColor() { return backgroundColor; }
    
private:

    float width;
    float height;
    ofColor backgroundColor;
};

class ofxCompositeImage::Material
{

public:
    
    Material() {};
    ~Material() {};
    
    Material(ofImage img, float x, float y, float width, float height)
    {
        this->image = img;
        this->x = x;
        this->y = y;
        this->width = width;
        this->height = height;
    }
    
    void setup(ofImage img, float x, float y, float width, float height)
    {
        this->image = img;
        this->x = x;
        this->y = y;
        this->width = width;
        this->height = height;
    }
    
    void setImage(ofImage img) { this->image = img; }
    
    void setPosition(float x, float y)
    {
        this->x = x;
        this->y = y;
    };
    
    void setWidth(float width) { this->width = width; }
    void setHeight(float height) { this->height = height; }
    
    void resize()
    {
        image.resize(width, height);
    }
    
    void draw()
    {
        image.draw(x, y);
    }
    
private:

    ofImage image;
    float x, y, width, height;
};


# pragma mark - class ofxCompositeImgae

class ofxCompositeImage::Composer : public ofThread
{

public:

    Composer() { ofxOpenGLContextScope::setup(); }
    ~Composer();

    void addLayers(vector<Material> layers, Setting setting);
    
    void threadedFunction();
    
    ofEvent<ofImage> compositionComplete;
    
private:
    
    vector< vector<Material> > tasks;
    vector<Setting> settings;
    
    void composite();
};