EXECUTABLE = SleepWell

INCLUDES = -I.. -I../Classes

SOURCES = main.cpp \
        ../Classes/AppDelegate.cpp \
        ../Classes/B2DebugDraw/B2DebugDraw.cpp \
        ../Classes/B2DebugDraw/GLES-Render.cpp \
        ../Classes/BAContactListener.cpp \
        ../Classes/BaseLayer.cpp \
        ../Classes/Entities/Abstract/Pulse.cpp \
        ../Classes/Entities/Collectables/Collectable.cpp \
        ../Classes/Entities/CollectableSpawnPoint.cpp \
        ../Classes/Entities/Enemies/BadNeuron.cpp \
        ../Classes/Entities/Enemies/Enemy.cpp \
        ../Classes/Entities/EnemySpawnPoint.cpp \
        ../Classes/Entities/Player.cpp \
        ../Classes/Entities/Static/Nerve.cpp \
        ../Classes/GameObject.cpp \
        ../Classes/GamePlay.cpp \
        ../Classes/GameState.cpp \
        ../Classes/HudApneaBar.cpp \
        ../Classes/HudPowerBar.cpp \
        ../Classes/Scenes/GameoverScene.cpp \
        ../Classes/Scenes/LevelTransition.cpp \
        ../Classes/Scenes/TitleScene.cpp

COCOS_ROOT = ../../..
include $(COCOS_ROOT)/cocos2dx/proj.linux/cocos2dx.mk

SHAREDLIBS += -lcocos2d
COCOS_LIBS = $(LIB_DIR)/libcocos2d.so

$(TARGET): $(OBJECTS) $(STATICLIBS) $(COCOS_LIBS) $(CORE_MAKEFILE_LIST)
	@mkdir -p $(@D)
	$(LOG_LINK)$(CXX) $(CXXFLAGS) $(OBJECTS) -o $@ $(SHAREDLIBS) $(STATICLIBS)

$(OBJ_DIR)/%.o: %.cpp $(CORE_MAKEFILE_LIST)
	@mkdir -p $(@D)
	$(LOG_CXX)$(CXX) $(CXXFLAGS) $(INCLUDES) $(DEFINES) $(VISIBILITY) -c $< -o $@

$(OBJ_DIR)/%.o: ../%.cpp $(CORE_MAKEFILE_LIST)
	@mkdir -p $(@D)
	$(LOG_CXX)$(CXX) $(CXXFLAGS) $(INCLUDES) $(DEFINES) $(VISIBILITY) -c $< -o $@
