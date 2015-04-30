#include "RbeVirtualFence.hpp"

#include <string>
#include <exception>

#include <ViNotion/VideoInputVideoFile.hpp>
#include <ViNotion/Image.hpp>
#include <ViNotion/VideoOutputDisplay.hpp>
#include <ViNotion/Draw.hpp>
#include <ViNotion/Font.hpp>
#include <ViNotion/Timer.hpp>
#include <ViNotion/ImageFile.hpp>

#include "src/core/Engine.hpp"
#include "src/core/Object.hpp"
#include "src/core/Event.hpp"
#include "src/core/Rule.hpp"
#include "src/core/EventContainer.hpp"
#include "src/core/Misc.hpp"

#include "vinotion/VirtualFencing/VirtualFencing.hpp"

#include "src/gui/RuleProcessingPanel.hpp"
#include "src/gui/RbeGeneralContainer.hpp"


RbeVirtualFence::RbeVirtualFence(QWidget *parent) :
  QDialog(parent)
{
  
  RuleProcessingPanel *panel = new RuleProcessingPanel();
  //QDialog
  //  panel->show();
  //  panel->raise();
  //  panel->activateWindow();
  
  videoFilePath = "";
  engine = new Rbe::Engine();
  engine->readContextFile("./data/.temp/contexts.xml");
  engine->readRuleFile("./data/.temp/rules.xml");  
}

int RbeVirtualFence::run()
{  
  
  
  //2 time check the videoPath
  //to avoid user press cancel
  if(videoFilePath == "")  
    return 1;
  
  
  try
  {       
    // the timer
    Vi::Timer frameTimer;
    
    // The markup font for drawing text
    Vi::Font markupFont;
    markupFont.init(NULL, 0, 14);
    
    // the video input 
    Vi::VideoInputVideoFile videoInput;
    videoInput.open(videoFilePath.toStdString());
    videoInput.setLoop(false);
    
    // Create display related stuff
    unsigned int markupFrameWidth = videoInput.getWidth();
    unsigned int markupFrameHeight = videoInput.getHeight();
    Vi::VideoOutputDisplay vidDisplay(640, 640, markupFrameWidth, markupFrameHeight, 0);
    
    // markup to be displayed on output
    Vi::Image<> currentFrameMarkup;
    currentFrameMarkup.size(markupFrameWidth, markupFrameHeight);
    currentFrameMarkup.clear();
    
    ///dai code///: disable clone video
    
    // the video output
    Vi::VideoOutputVideoFile outputFile;
    unsigned int bitrate = 5000000;
    outputFile.open(videoInput.getWidth(), videoInput.getHeight(), "./data/.temp/VirtualFence/result.avi", Vi::Frac<>(25), bitrate);
    
    
    // the frame counter
    unsigned int frameCounter = 0;
    
    // the virtual fencing processing
    VirtualFencing virtualFencing(videoInput.getWidth(), videoInput.getHeight(), "./data/.temp/VirtualFence/config.ini"); 
    
    ///dai code///
    virtualFencing.setEngine(engine);
    
    // the current frame
    Vi::Image<> currentFrame;
    
    // ================
    // === Let's go ===
    // ================
    while(!vidDisplay.getQuit())
    {
      if(!vidDisplay.getPaused())
      {
        if(!videoInput.read(currentFrame))
          break;
        
        // start the timer 
        frameTimer.start();
        
        // do the processing 
        virtualFencing.process(currentFrame, frameCounter);
        
        ///dai code/// : process rule
        engine->processRule();
        
        // draw overlay on objects, restricted area and trip wires
        virtualFencing.drawMaskOverlay(currentFrame);
        
        ///dai code///
        //disable origin drawTripwire
        ///virtualFencing.drawTripWires(currentFrame, Vi::YCC_CYAN);
        
        ///dai code///
        virtualFencing.drawRbeTripwire(currentFrame,Vi::YCC_CYAN);
        
        
        virtualFencing.drawOverlayOnObjects(currentFrame);
        
        // +++++++++++++++
        // +++ DISPLAY +++
        // +++++++++++++++
        
        ///dai code/// : disable the original detection display
        /*
        for (unsigned int i = 0; i < virtualFencing.mTracksVF.size(); i++)
        {          
          // object in restricted area
          if (virtualFencing.mTracksVF[i].mInRestrictedArea)
          {
            drawBbox(currentFrame, virtualFencing.mTracksVF[i].mBbox, Vi::YCC_RED);
            markupFont.drawText(currentFrame, "IN AREA", virtualFencing.mTracksVF[i].mBbox.p1, Vi::YCC_RED);
          }          
          // object crossing tripwire - draw the trip-wire in red           
          if (virtualFencing.mTracksVF[i].mCrossed)
            virtualFencing.drawTripWires(currentFrame, Vi::YCC_RED);
        }
        */
        ///dai code/// : Custom draw
        
        for (unsigned int i = 0; i < virtualFencing.mTracksVF.size(); i++)
        {          
          Rbe::Object *object = engine->getObjects()[i];          
          
          int objectID = -1;
          int contextID = -1;
          Rbe::Event *event = NULL;
          for(int j = 0 ; j < engine->getRules().size(); j++)
          {
            Rbe::EventContainer *container = engine->getRules()[j]->getEventContainer();
            for(int a = 0 ; a < container->mContainers.size(); a++)
            {
              this->loadSub(objectID,contextID,event, container->mContainers[a],object);
            }
            for(int k = 0 ; k < container->mResultQueue.size(); k++)
            {
              Rbe::QueueStruct *qT = &container->mResultQueue[k];
              if(qT->result == true)
              {
                for(int l=0; l < qT->objectsID.size(); l++)
                {
                  int oID = qT->objectsID[l];
                  if(object->getId() == oID)
                  {
                    objectID = oID;
                    event = qT->eventSource;
                    if(event->getType() == Rbe::Event::CROSSING_TRIPWIRE)
                    {
                      contextID = qT->contextID;
                    }
                  }
                  
                }
              }
            }
          }
          
          if(objectID != -1)
          {          
            drawBbox(currentFrame, virtualFencing.mTracksVF[i].mBbox, Vi::YCC_RED);
            if(event != NULL)
            {
              markupFont.drawText(currentFrame, event->getTypeString() , virtualFencing.mTracksVF[i].mBbox.p1, Vi::YCC_RED);   
              
              if(event->getTypeString() == "CROSSING_TRIPWIRE")
                virtualFencing.drawRbeTripwire2(currentFrame, Vi::YCC_RED,contextID);
            }
          }          
          
          Vi::Point<int> p;
          p.x = 10;
          p.y = 10;
          //markupFont.drawText(currentFrame, , p, Vi::YCC_ORANGE);   
          
        }
        
        ///dai code/// : draw trajectory
        unsigned length = 10;
        virtualFencing.drawObjectsTrajectories(currentFrame,length,Vi::YCC_YELLOW);
        
        ///dai code/// :disable cout the timer        
        /*
        frameTimer.get("Processing");
        */
        
        //clean the eventQueue
        engine->cleanRuleEventResultQueue();
        
        frameCounter++;
      }
      
      // create markup display
      currentFrameMarkup.subcopy(currentFrame, 0, 0);
      vidDisplay.write(currentFrameMarkup);
      
      ///dai code/// : disable clone video
      
      // write to video file
      outputFile.write(currentFrameMarkup);
      
    }
    
    ///dai code/// : disable clone video
    
    // close output video file
    outputFile.close();    
    
  }
  catch (const std::exception &e)
  {
    std::cout << "Error: " << e.what() << std::endl;
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

void RbeVirtualFence::loadSub(int &objectID,int &contextID, Rbe::Event *event,Rbe::EventContainer *container,Rbe::Object *object)
{
  for(int b = 0 ; b < container->mContainers.size(); b++)
  {
    this->loadSub(objectID,contextID,event, container->mContainers[b],object);
  }
  
  for(int k = 0 ; k < container->mResultQueue.size(); k++)
  { 
    Rbe::QueueStruct *qT = &container->mResultQueue[k];
    if(qT->result == true)
    {
      for(int l=0; l < qT->objectsID.size(); l++)
      {
        int oID = qT->objectsID[l];
        if(object->getId() == oID)
        {
          objectID = oID;
          event = qT->eventSource;
          if(event != NULL)
          {
            if(event->getType() == Rbe::Event::CROSSING_TRIPWIRE)
            {
              contextID = qT->contextID;
            }
          }
        }
        
      }
    }
  }
}
