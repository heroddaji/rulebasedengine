######################################
## use qt core, gui, xml components ##
######################################
QT += core
QT += gui
QT += xml

########################################
## set up sources, headers, resources ##
########################################
TARGET = rulebasedengine
TEMPLATE = app

HEADERS += \
    src/core/Rule.hpp \
    src/core/ObjectFrame.hpp \
    src/core/Object.hpp \
    src/core/Misc.hpp \
    src/core/EventContainer.hpp \
    src/core/Event.hpp \
    src/core/Engine.hpp \
    src/core/ContextTripwire.hpp \
    src/core/ContextArea.hpp \
    src/core/Context.hpp \
    src/core/Action.hpp \
    src/gui/RbeXmlHandler.hpp \
    src/gui/RbeVisualizeWidget_RuleTree.hpp \
    src/gui/RbeVisualizeWidget_PolygonItem.hpp \
    src/gui/RbeVisualizeWidget_LineItem.hpp \
    src/gui/RbeVisualizeWidget_GraphicsView.hpp \
    src/gui/RbeVisualizeWidget_GraphicsScene.hpp \
    src/gui/RbeVisualizeWidget.hpp \
    src/gui/RbeVirtualFence.hpp \
    src/gui/RbePropertyTree.hpp \
    src/gui/RbeItemTree_WidgetItem.hpp \
    src/gui/RbeItemTree.hpp \
    src/gui/RbeGeneralItem.hpp \
    src/gui/RbeGeneralContainer.hpp \
    src/gui/MainWindow.hpp \
    extra/QTFFmpegWrapper/QVideoEncoder.h \
    extra/QTFFmpegWrapper/QVideoDecoder.h \
    vinotion/VirtualFencing/VirtualFencing.hpp \
    vinotion/VirtualFencing/TripWire.hpp \
    vinotion/VirtualFencing/TrackedObjectVirtualFencingParams.hpp \
    vinotion/VirtualFencing/TrackedObjectVirtualFencing.hpp \
    vinotion/VirtualFencing/RuleBasedEngine.hpp \
    vinotion/VirtualFencing/ResultFile.hpp \
    vinotion/VirtualFencing/Recording.hpp \
    vinotion/VirtualFencing/ContextFilter.hpp \
    src/core/EventFilter.hpp \
    src/gui/RuleProcessingPanel.hpp

SOURCES += \
    src/core/Rule.cpp \
    src/core/ObjectFrame.cpp \
    src/core/Object.cpp \
    src/core/EventContainer.cpp \
    src/core/Event.cpp \
    src/core/Engine.cpp \
    src/core/ContextTripwire.cpp \
    src/core/ContextArea.cpp \
    src/core/Context.cpp \
    src/core/Action.cpp \
    src/gui/main.cpp \
    src/gui/RbeXmlHandler.cpp \
    src/gui/RbeVisualizeWidget_RuleTree.cpp \
    src/gui/RbeVisualizeWidget_PolygonItem.cpp \
    src/gui/RbeVisualizeWidget_LineItem.cpp \
    src/gui/RbeVisualizeWidget_GraphicsView.cpp \
    src/gui/RbeVisualizeWidget_GraphicsScene.cpp \
    src/gui/RbeVisualizeWidget.cpp \
    src/gui/RbeVirtualFence.cpp \
    src/gui/RbePropertyTree.cpp \
    src/gui/RbeItemTree_WidgetItem.cpp \
    src/gui/RbeItemTree.cpp \
    src/gui/RbeGeneralItem.cpp \
    src/gui/RbeGeneralContainer.cpp \
    src/gui/MainWindow.cpp \
    extra/QTFFmpegWrapper/QVideoEncoder.cpp \
    extra/QTFFmpegWrapper/QVideoDecoder.cpp \
    vinotion/VirtualFencing/VirtualFencing.cpp \
    vinotion/VirtualFencing/TripWire.cpp \
    vinotion/VirtualFencing/TrackedObjectVirtualFencing.cpp \
    vinotion/VirtualFencing/RuleBasedEngine.cpp \
    vinotion/VirtualFencing/ResultFile.cpp \
    vinotion/VirtualFencing/Recording.cpp \
    vinotion/VirtualFencing/ContextFilter.cpp \
    src/gui/help.cpp \
    src/core/EventFilter.cpp \
    src/gui/RuleProcessingPanel.cpp

FORMS += \
    src/gui/RuleProcessingPanel.ui

RESOURCES += \
    src/gui/Resources.qrc

##############################################
## use additional qtpropertybrowser project ##
##############################################
include(extra/qtpropertybrowser/src/qtpropertybrowser.pri)

###########################
## link to vinotion libs ##
###########################
LIBS += -lViNotion
LIBS += -lBackgroundSubtraction
LIBS += -lVImageProcessing
LIBS += -lTracking
LIBS += -lMultipleHypothesesTracking 
LIBS += -lGenericObject
LIBS += -lSettings
LIBS += -lFeatures
LIBS += -lboost_thread

#####################
## link to libXml2 ##
#####################
LIBS += -lxml2
INCLUDEPATH += /usr/include/libxml2

OTHER_FILES += \
    TODO \
    BUGS.txt
