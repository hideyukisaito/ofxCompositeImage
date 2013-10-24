#include "ofxCompositeImgae.h"

using namespace ofxCompositeImage;

Composer::~Composer()
{
    if (isThreadRunning())
    {
        stopThread();
    }
}

void Composer::addLayers(vector<Material> layers, Setting setting)
{
    lock();
    tasks.push_back(layers);
    settings.push_back(setting);
    unlock();
    
    if (!isThreadRunning()) startThread();
}

void Composer::threadedFunction()
{
    ofxOpenGLContextScope scope;
    
    while (isThreadRunning())
    {
        ofLogNotice() << "is main thread? : " << isMainThread();
        if (0 < tasks.size())
        {
            lock();
            composite();
            unlock();
        }
        
        ofSleepMillis(10);
    }
}

void Composer::composite()
{
    ofFbo fbo;
    fbo.allocate((int)settings.at(0).getWidth(), (int)settings.at(0).getHeight());
    fbo.begin();
    {
        ofClear(0, 0, 0);
        ofBackground(settings.at(0).getBackgroundColor());
        
        ofPushStyle();
        {
            ofEnableAlphaBlending();
            for (int i = 0; i < tasks.at(0).size(); ++i)
            {
                Material m = tasks.at(0).at(i);
                m.resize();
                m.draw();
            }
            ofDisableAlphaBlending();
        }
        ofPopStyle();
    }
    fbo.end();
    
    ofImage output;
    fbo.readToPixels(output.getPixelsRef());
    output.update();
    output.resize(settings.at(0).getWidth(), settings.at(0).getHeight());
    
    tasks.erase(tasks.begin());
    settings.erase(settings.begin());
    
    if (tasks.empty()) stopThread();
    
    ofNotifyEvent(compositionComplete, output, this);
}