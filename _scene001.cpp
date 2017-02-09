
/*
Developer: Jorge Rosa (jorge.bigarte@gmail.com) - With love to: ASCR
Website: https://sites.google.com/site/jorgerosaportfolio
Mixxx DarkMetal Skin: http://www.mixxx.org/forums/viewtopic.php?f=8&t=6052
*/

/// SHARED VARS:
/// (...)


class Scene001 : public Engine{

    public:
    Scene001();  // Constructor
    ~Scene001(); // Destructor
    void loadScene();
    void createButtons();
    int doLoop();
    void deleteScene();
    void destroy();

    private:
    Ogre::SceneNode* nodeBase;
    std::vector<Ogre::Entity*> entities;

    /// Camera Movements
    Ogre::Vector3 mTranslateVector = Ogre::Vector3::ZERO;
    Ogre::Real mCamSpeed  = 150.0f;
    bool zoom = true;
    float beat[3];
    bool beat1 = true;
    bool beat2 = false;
    bool beatOnce = false;
    float tb = 0.0f;
    bool pressOnce = true;

    /// Sound Wave: (display on beat)
    Ogre::Vector3 waveScale = Ogre::Vector3(1.0,1.0,1.0);

    /// Peakmeters:
    Rectangle2D* rectRow[100];
    Ogre::MaterialPtr mMat;

    /// Lights
    Ogre::Light* light1;
    Ogre::Vector3 color1 = Ogre::Vector3(1.0,0.9,0.8);
    Ogre::Light* light2;
    Ogre::Vector3 color2 = Ogre::Vector3(0.3,0.3,0.3);

    bool doNow[100];

};

Scene001::Scene001(){};  // Constructor (Can accept arguments, but this one does not)
Scene001::~Scene001(){}; // Destructor (do not accept arguments)





void Scene001::loadScene(){

    beat[1] = 0.0f;
    beat[2] = 0.0f;

    for (int i=0; i<100; i++){ doNow[i] = false; };


    /// Attach ALL loaded meshes to this node:
    nodeBase = mSmgr->getRootSceneNode()->createChildSceneNode("nodeBaseChild");



    /// ####################################################################################################################
    /// Load 1st ".scene" file:
    /// ####################################################################################################################

    std::cout << "------------------------------------------------" << std::endl;
    std::cout << "Scene: Loading Logos..."                          << std::endl;
    std::cout << "------------------------------------------------" << std::endl;

    /// Scene file:
    Ogre::DotSceneLoader* loader1 = new Ogre::DotSceneLoader();
    /// Example: loader1->parseDotScene("logo-mixxx.scene", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, mSmgr, nodeBase, "");
    loader1->parseDotScene(mLogoSceneFile, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, mSmgr, nodeBase, "");
    /// Destroy:
    delete loader1;

    /// NODES IN BASE:
    Ogre::Node::ChildNodeIterator it1 = nodeBase->getChildIterator();
    while(it1.hasMoreElements()){

    /// NODES loop:
    Ogre::SceneNode* child1 = (Ogre::SceneNode*)it1.getNext();
    Ogre::SceneNode::ObjectIterator oi1 = child1->getAttachedObjectIterator();

    /// Apply visibility to all nodes... (Value set in "custom.xml" file)
    // child1->setVisible(true);
    // if(mLogoSceneVisible == "false"){ child1->setVisible(false); };

    /// ENTITIES loop:
    while(oi1.hasMoreElements()){
    Ogre::Entity* ent = (Ogre::Entity*)oi1.getNext();
    entities.push_back(ent); /// To be able to use these entities later, for my own other stuff


    /// Find this mesh...
    if(ent->getName() == "Logo_Mixxx"){
    Ogre::SceneNode* node = mSmgr->getSceneNode(ent->getName());
    if(mLogoSceneVisible == "false"){ node->setVisible(false); }; /// Apply visibility (Value set in "custom.xml" file)
    ent->setCastShadows(false);        // Cast shadows?
    ent->setMaterialName("Mat_Mixxx"); // Material
    };

    /// Find this mesh...
    if(ent->getName() == "Logo_JDark"){
    Ogre::SceneNode* node = mSmgr->getSceneNode(ent->getName());
    if(mLogoSceneVisible == "false"){ node->setVisible(false); }; /// Apply visibility (Value set in "custom.xml" file)
    ent->setCastShadows(false);        // Cast shadows?
    ent->setMaterialName("Mat_JDark"); // Material
    };

    };

    };

    std::cout << "------------------------------------------------" << std::endl;
    std::cout << "Scene: Logos Loaded Successfully."                << std::endl;
    std::cout << "------------------------------------------------" << std::endl;




    /// ####################################################################################################################
    /// Load 2nd ".scene" file:
    /// ####################################################################################################################

    std::cout << "------------------------------------------------" << std::endl;
    std::cout << "Scene: Loading Background Objects..."             << std::endl;
    std::cout << "------------------------------------------------" << std::endl;

    /// Scene file:
    Ogre::DotSceneLoader* loader2 = new Ogre::DotSceneLoader();
    /// Example: loader2->parseDotScene("scenario.scene", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, mSmgr, nodeBase, "");
    loader2->parseDotScene(mMainSceneFile, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, mSmgr, nodeBase, "");
    /// Destroy:
    delete loader2;

    /// NODES IN BASE:
    Ogre::Node::ChildNodeIterator it2 = nodeBase->getChildIterator();
    while(it2.hasMoreElements()){

    /// NODES loop:
    Ogre::SceneNode* child2 = (Ogre::SceneNode*)it2.getNext();
    Ogre::SceneNode::ObjectIterator oi2 = child2->getAttachedObjectIterator();

    /// ENTITIES loop:
    while(oi2.hasMoreElements()){
    Ogre::Entity* ent = (Ogre::Entity*)oi2.getNext();
    entities.push_back(ent); /// To be able to use these entities later, for my own other stuff...

    /// Find this mesh...
    if(ent->getName() == "Earth"){
    Ogre::SceneNode* node = mSmgr->getSceneNode(ent->getName());
    if(mMainSceneVisible == "false"){ node->setVisible(false); }; /// Apply visibility (Value set in "custom.xml" file)
    ent->setCastShadows(true);         // Cast shadows?
    ent->setMaterialName("Mat_Earth"); // Material
    /// Create Animation:
    Ogre::Animation* anim = mSmgr->createAnimation("Earth_Rotating", 4000);
    anim->setRotationInterpolationMode(Ogre::Animation::RIM_LINEAR); // RIM_LINEAR, RIM_SPHERICAL
    Ogre::NodeAnimationTrack* track = anim->createNodeTrack(0, node);
    Ogre::TransformKeyFrame* key;
    key = track->createNodeKeyFrame(0);
    key->setRotation(Ogre::Quaternion(Ogre::Degree(0),   Ogre::Vector3::UNIT_Y)); // Rotation
    key = track->createNodeKeyFrame(1000);
    key->setRotation(Ogre::Quaternion(Ogre::Degree(90),  Ogre::Vector3::UNIT_Y)); // Rotation
    key = track->createNodeKeyFrame(2000);
    key->setRotation(Ogre::Quaternion(Ogre::Degree(180), Ogre::Vector3::UNIT_Y)); // Rotation
    key = track->createNodeKeyFrame(3000);
    key->setRotation(Ogre::Quaternion(Ogre::Degree(270), Ogre::Vector3::UNIT_Y)); // Rotation
    key = track->createNodeKeyFrame(4000);
    key->setRotation(Ogre::Quaternion(Ogre::Degree(360), Ogre::Vector3::UNIT_Y)); // Rotation
    /// Aplly Animations:
    animstate[0] = mSmgr->createAnimationState("Earth_Rotating");
    animstate[0]->setEnabled(true);
    animstate[0]->setLoop(true);
    };

    /// Find this mesh...
    if(ent->getName() == "Moon"){
    Ogre::SceneNode* node = mSmgr->getSceneNode(ent->getName());
    if(mMainSceneVisible == "false"){ node->setVisible(false); }; /// Apply visibility (Value set in "custom.xml" file)
    ent->setCastShadows(true);        // Cast shadows?
    ent->setMaterialName("Mat_Moon"); // Material
    /// Create Animation:
    Ogre::Animation* anim = mSmgr->createAnimation("Moon_Moving", 500);
    anim->setRotationInterpolationMode(Ogre::Animation::RIM_LINEAR); // RIM_LINEAR, RIM_SPHERICAL
    Ogre::NodeAnimationTrack* track = anim->createNodeTrack(0, node);
    Ogre::TransformKeyFrame* key;
    key = track->createNodeKeyFrame(0);
    key->setTranslate(Ogre::Vector3( 350.0, 4.0, 100.0)); // Position
    key = track->createNodeKeyFrame(500);
    key->setTranslate(Ogre::Vector3(-350.0, 4.0, 100.0)); // Position
    /// Aplly Animations:
    animstate[2] = mSmgr->createAnimationState("Moon_Moving");
    animstate[2]->setEnabled(true);
    animstate[2]->setLoop(true);
    };

    /// Find this mesh...
    if(ent->getName() == "Plane01"){
    Ogre::SceneNode* node = mSmgr->getSceneNode(ent->getName());
    if(mMainSceneVisible == "false"){ node->setVisible(false); }; /// Apply visibility (Value set in "custom.xml" file)
    ent->setCastShadows(false);                 // Cast shadows?
    ent->setMaterialName("Mat_Atmosphere");     // Material
    node->setScale(Ogre::Vector3(2.5, 1.0, 2.5));     // Scale
    node->setPosition(Ogre::Vector3(0.0, 0.0, 70.0)); // Position
    };

    /// Find this mesh...
    if(ent->getName() == "Plane02"){
    Ogre::SceneNode* node = mSmgr->getSceneNode(ent->getName());
    if(mMainSceneVisible == "false"){ node->setVisible(false); }; /// Apply visibility (Value set in "custom.xml" file)
    ent->setCastShadows(false);                      // Cast shadows?
    ent->setMaterialName("Mat_Sun");                 // Material
    ent->setRenderQueueGroupAndPriority(Ogre::RENDER_QUEUE_3, 1); /// Render group and priority
    node->setScale(Ogre::Vector3(1.0, 1.0, 1.0));          // Scale
    node->setPosition(Ogre::Vector3(-110.0, 40.0, 300.0)); // Position
    };

    /// Find this mesh...
    if(ent->getName() == "Plane03"){
    Ogre::SceneNode* node = mSmgr->getSceneNode(ent->getName());
    if(mMainSceneVisible == "false"){ node->setVisible(false); }; /// Apply visibility (Value set in "custom.xml" file)
    ent->setCastShadows(false);                  // Cast shadows?
    ent->setMaterialName("Mat_Space_3");         // Material
    ent->setRenderQueueGroupAndPriority(Ogre::RENDER_QUEUE_3, 1); /// Render group and priority
    node->setScale(Ogre::Vector3(13.0, 1.0, 13.0));    // Scale
    node->setPosition(Ogre::Vector3(0.0, 0.0, 500.0)); // Position
    };

    /// Find this mesh...
    if(ent->getName() == "Plane04"){
    Ogre::SceneNode* node = mSmgr->getSceneNode(ent->getName());
    if(mMainSceneVisible == "false"){ node->setVisible(false); }; /// Apply visibility (Value set in "custom.xml" file)
    ent->setCastShadows(false);                  // Cast shadows?
    ent->setMaterialName("Mat_Space_4");         // Material
    ent->setRenderQueueGroupAndPriority(Ogre::RENDER_QUEUE_3, 1); /// Render group and priority
    node->setScale(Ogre::Vector3(17.0, 1.0, 17.0));    // Scale
    node->setPosition(Ogre::Vector3(0.0, 0.0, 700.0)); // Position
    };

    /// Find this mesh...
    if(ent->getName() == "Plane05"){
    Ogre::SceneNode* node = mSmgr->getSceneNode(ent->getName());
    /// if(mMainSceneVisible == "false"){ node->setVisible(false); }; /// Apply visibility (Value set in "custom.xml" file)
    ent->setCastShadows(false);                 // Cast shadows?
    ent->setMaterialName("Mat_SoundWave");      // Material
    ent->setRenderQueueGroupAndPriority(Ogre::RENDER_QUEUE_3, 1); /// Render group and priority
    node->setScale(Ogre::Vector3(1.5, 1.0, 1.5));     // Scale
    node->setPosition(Ogre::Vector3(0.0, 0.0, 70.0)); // Position
    };

    };

    };

    std::cout << "------------------------------------------------" << std::endl;
    std::cout << "Scene: Background Objects Loaded Successfully."   << std::endl;
    std::cout << "------------------------------------------------" << std::endl;



    /// ####################################################################################################################
    /// Create Lights:
    /// ####################################################################################################################

    /// Light: Sun (Placed at right of earth)
    Ogre::SceneNode* node1 = mSmgr->getRootSceneNode()->createChildSceneNode();
    light1 = mSmgr->createLight("Sun_Light");
    light1->setType(Ogre::Light::LT_POINT); // Ogre::Light::LT_DIRECTIONAL - Ogre::Light::LT_POINT
    light1->setDiffuseColour(color1.x,color1.y,color1.z);  /// Apply Color
    light1->setSpecularColour(color1.x,color1.y,color1.z); /// Apply Color
    light1->setCastShadows(false); /// Cast shadows?
    node1->attachObject(light1);
    node1->setPosition(Ogre::Vector3(-80.0, 1.0, 5.0)); // Position

    /// Light: Beat (Placed in front of the logos)
    Ogre::SceneNode* node2 = mSmgr->getRootSceneNode()->createChildSceneNode();
    light2 = mSmgr->createLight("Beat_Light");
    light2->setType(Ogre::Light::LT_POINT); // Ogre::Light::LT_DIRECTIONAL - Ogre::Light::LT_POINT
    light2->setDiffuseColour(color2.x,color2.y,color2.z);  /// Apply Color
    light2->setSpecularColour(color2.x,color2.y,color2.z); /// Apply Color
    light2->setCastShadows(false); /// Cast shadows?
    node2->attachObject(light2);
    node2->setPosition(Ogre::Vector3(0.0, 0.0, -250.0)); // Position


    /// ####################################################################################################################
    /// Create Particles:
    /// ####################################################################################################################

    /// Particles:
    Ogre::SceneNode* nodep = mSmgr->getRootSceneNode()->createChildSceneNode();
    nodep->setPosition(Ogre::Vector3(0.0, -34.0, 0.0));
    ps[0] = mSmgr->createParticleSystem("Fountain_1", "Particle_Fountain");
    ps[0]->setEmitting(0);
    nodep->attachObject(ps[0]);


    /// ####################################################################################################################
    /// Create 2D Rectangles: (Peakmeters)
    /// ####################################################################################################################

    /// Draw the rectangles row:
    for(int i=0; i<100; i++){
    rectRow[i] = new Rectangle2D(true);
    rectRow[i]->setCorners(0.0,0.0,0.0,0.0); // left, top, right, bottom, updateAABB
    rectRow[i]->setMaterial("Mat_Peak"); /// Material
    rectRow[i]->setCastShadows(false);   /// Cast shadows?
    rectRow[i]->setRenderQueueGroupAndPriority(Ogre::RENDER_QUEUE_4, 1); /// Render group and priority
    Ogre::String n = Ogre::StringConverter::toString(i);
    SceneNode* node = mSmgr->getRootSceneNode()->createChildSceneNode("Peak_"+n);
    node->attachObject(rectRow[i]);
    // node->setPosition(Ogre::Vector3(0.0, 0.0, 50.0));
    };



};




void Scene001::createButtons(){
    Ogre::OverlayManager& overlayManager = Ogre::OverlayManager::getSingleton();
    // Create an overlay
    Ogre::Overlay* overlay = overlayManager.create("OverlayName");
    // Create a panel
    Ogre::OverlayContainer* panel = static_cast<Ogre::OverlayContainer*>( overlayManager.createOverlayElement( "Panel", "PanelName" ) );
    panel->setPosition( 0.0, 0.0 );
    panel->setDimensions( 0.1, 0.1 );
    panel->setMaterialName( "BaseWhite" );
    // Add the panel to the overlay
    overlay->add2D( panel );
    // Show the overlay
    overlay->show();
};



int Scene001::doLoop(){

    /// Timer stuff:
    mTimeCurrentFrame = static_cast<float>(mRoot->getTimer()->getMicroseconds())/1000000; /// Returns number of senconds past from start of the app
    mDeltaTime = mTimeCurrentFrame - mTimeLastFrame; /// time pasted between each render
    mTimeLastFrame = mTimeCurrentFrame;

    /// Get FPS:
    Ogre::Real lastfps = mWin->getLastFPS();
    Ogre::String fps = Ogre::StringConverter::toString(lastfps);

    /// Render a frame:
    if(!mRoot->renderOneFrame()){ return false; };

    /// Render:
    Ogre::WindowEventUtilities::messagePump();
    mRoot->renderOneFrame();

    if(!mWin->isActive() && !mWin->isVisible()){ /* mWin->update(); */ }; // Updates all stuff, even if app is running minimized or in the background!


    /// ----------------------------------------------------------------------------------------------------------------------------------


    /// listenInputs(); Keyboard + Mouse + Window Close Button
    /// OIS Key codes: http://hackage.haskell.org/package/HGamer3D-OIS-Binding-0.1.0/src/include/EnumKeyCode.h

    /// Each time you want to get input device events (probably once per frame), you follow these simple steps:
    /// 1) call the capture() method on the device in question. 2) Interrogate the device directly.
    mKeyboard->capture();
    mMouse->capture();

    /// Set time between keys press: (Example: Set in the keys: 0.5 == Half Second)
    if(mTimeUntilNextToggle>=0.0){ mTimeUntilNextToggle -= mDeltaTime; };


    /// Exit?... Close button on window clicked:
    if(mWin->isClosed()){
    mTimeUntilNextToggle = 0.5;
    std::cout << "BUTTON Close Window: Bye!..." << std::endl;
    exitAll = true;
    };

    /// Exit?... Pressed Esc key:
    if(mKeyboard->isKeyDown(OIS::KC_ESCAPE) && mTimeUntilNextToggle<=0.0){
    mTimeUntilNextToggle = 0.5;
    std::cout << "KEY Escape: Bye!..." << std::endl;
    exitAll = true;
    };

    /// Particles: Fountain
    if(mKeyboard->isKeyDown(OIS::KC_F1)){
    ps[0]->setEmitting(1000);
    } else {
    ps[0]->setEmitting(0);
    };

    /// TODO...
    if(mKeyboard->isKeyDown(OIS::KC_F11) && mTimeUntilNextToggle<=0.0){
    mTimeUntilNextToggle = 0.5;
    deleteScene();
    std::cout << "KEY F11: Scene Objects Deleted!..." << std::endl;
    };


    /// ----------------------------------------------------------------------------------------------------------------------------------

    /// Stuff RELATED with the audio class:


    devicesWereInitialized += 1;
    if(devicesWereInitialized > 9999){ devicesWereInitialized = 9999; }

    /// Initialize audio devices only now, and do it only once!...
    if(devicesWereInitialized <= 50) {
    mTempTexts = "Searching and testing available audio devices ...";
    };
    if(devicesWereInitialized == 50){
    audio->initializeDevices();
    mTempTexts = "";
    };

    /// Connect to default audio device only now, and do it only once!...
    if(devicesWereInitialized > 50 && devicesWereInitialized <= 100) {
    Ogre::String n = Ogre::StringConverter::toString(mDevice+1); // Converts numbers into string
    mTempTexts = "Connecting to audio device N." + n + ": " + mDeviceName[mDevice] + " ...";
    };
    if(devicesWereInitialized == 100){
    audio->initializeAudioStream();
    mTempTexts = "";
    };

    /// Both above audio functions were initialized?... If YES, then these keys are now available:
    if(devicesWereInitialized > 150){

    if(doNow[0] == true){
    doNow[0] = false;
    if(mDeviceStatus[mDevice]=="Success" && mDevice<mTotalDevices){
    audio->initializeAudioStream();
    mTempTexts = "";
    };
    if(mDeviceStatus[mDevice]=="Failed" && mDevice<mTotalDevices){
    Ogre::String n = Ogre::StringConverter::toString(mDevice+1); // Converts numbers into string
    mTempTexts = "ERROR: Cant connect to audio device N." + n + ": " + mDeviceName[mDevice]; };
    };


    if(mKeyboard->isKeyDown(OIS::KC_O) && mTimeUntilNextToggle<=0.0){
    mTimeUntilNextToggle = 1.0;
    mDevice -= 1; if(mDevice<0){ mDevice = 0; };
    Ogre::String n = Ogre::StringConverter::toString(mDevice+1); // Converts numbers into string
    mTempTexts = "Connecting to audio device N." + n + ": " + mDeviceName[mDevice] + " ...";
    doNow[0] = true;
    };

    if(mKeyboard->isKeyDown(OIS::KC_P) && mTimeUntilNextToggle<=0.0){
    mTimeUntilNextToggle = 1.0;
    mDevice += 1; if(mDevice>=mTotalDevices){ mDevice = 0; };
    Ogre::String n = Ogre::StringConverter::toString(mDevice+1); // Converts numbers into string
    mTempTexts = "Connecting to audio device N." + n + ": " + mDeviceName[mDevice] + " ...";
    doNow[0] = true;
    };

    };


    /// ----------------------------------------------------------------------------------------------------------------------------------


    /// Set between FullScreen/Window modes:
    if(mKeyboard->isKeyDown(OIS::KC_F) && mTimeUntilNextToggle<=0.0){
    mTimeUntilNextToggle = 0.5;
    bool fs = mWin->isFullScreen();
    if(fs == false){ mWin->setFullscreen(true,  800, 600); std::cout << "KEY F: Full Screen Mode." << std::endl; };
    if(fs == true ){ mWin->setFullscreen(false, 600, 480); std::cout << "KEY F: Windowed Mode." << std::endl;    };
    };


    /// ----------------------------------------------------------------------------------------------------------------------------------
    /// BEATS (Make the Zoom IN/OUT for the 3D Visual Effects)
    /// ----------------------------------------------------------------------------------------------------------------------------------

    if((mKeyboard->isKeyDown(OIS::KC_RETURN) || mKeyboard->isKeyDown(OIS::KC_NUMPADENTER)) && beat1 == true && pressOnce == true && mTimeUntilNextToggle<=0.0){
    mTimeUntilNextToggle = 0.1; /// Allowing fast tap here
    timerBeat.reset();
    beat[1] = 0.0f;
    beat[2] = 0.0f;
    std::cout << "Time between beats: 0 (Disabled)" << std::endl;
    beat1 = false;
    beat2 = true;
    pressOnce = false;
    mCam->setPosition(Ogre::Vector3(0.0, 1.0, -85.0));
    color2 = Ogre::Vector3(0.3,0.3,0.3);
    };

    if((mKeyboard->isKeyDown(OIS::KC_RETURN) || mKeyboard->isKeyDown(OIS::KC_NUMPADENTER)) && beat2 == true && pressOnce == true && mTimeUntilNextToggle<=0.0){
    mTimeUntilNextToggle = 0.1; /// Allowing fast tap here
    beat[2] = ((timerBeat.getMicroseconds()/1000.0) - beat[1]);
    std::cout << "Time between beats: " << beat[2] << std::endl;
    beat1 = true;
    beat2 = false;
    pressOnce = false;
    mCam->setPosition(Ogre::Vector3(0.0, 1.0, -85.0));
    color2 = Ogre::Vector3(0.3,0.3,0.3);
    };

    if(!mKeyboard->isKeyDown(OIS::KC_RETURN) && !mKeyboard->isKeyDown(OIS::KC_NUMPADENTER)){
    pressOnce = true;
    };

    if(mKeyboard->isKeyDown(OIS::KC_SPACE) && mTimeUntilNextToggle<=0.0){
    mTimeUntilNextToggle = 0.1; /// Allowing fast tap here
    beat[1] = 0.0f;
    beat[2] = 0.0f;
    beatOnce = true;
    beat1 = true;
    beat2 = false;
    timerBeat.reset();
    mCam->setPosition(Ogre::Vector3(0.0, 1.0, -85.0));
    color2 = Ogre::Vector3(0.3,0.3,0.3);
    std::cout << "Beat once!" << std::endl;
    // zoom = false;
    };

    if((mKeyboard->isKeyDown(OIS::KC_NUMPAD0) || mKeyboard->isKeyDown(OIS::KC_0)) && mTimeUntilNextToggle<=0.0){
    mTimeUntilNextToggle = 0.5;
    beat[1] = 0.0f;
    beat[2] = 0.0f;
    beat1 = true;
    beat2 = false;
    timerBeat.reset();
    mCam->setPosition(Ogre::Vector3(0.0, 1.0, -85.0));
    color2 = Ogre::Vector3(0.3,0.3,0.3);
    std::cout << "Time between beats: 0 (Disabled)" << std::endl;
    // zoom = false;
    };

    if((mKeyboard->isKeyDown(OIS::KC_NUMPAD1) || mKeyboard->isKeyDown(OIS::KC_1)) && mTimeUntilNextToggle<=0.0){
    mTimeUntilNextToggle = 0.5;
    beat[1] = 0.0f;
    beat[2] = 900.0f;
    beat1 = true;
    beat2 = false;
    timerBeat.reset();
    mCam->setPosition(Ogre::Vector3(0.0, 1.0, -85.0));
    color2 = Ogre::Vector3(0.3,0.3,0.3);
    std::cout << "Time between beats: " << beat[2] << std::endl;
    // zoom = false;
    };

    if((mKeyboard->isKeyDown(OIS::KC_NUMPAD2) || mKeyboard->isKeyDown(OIS::KC_2)) && mTimeUntilNextToggle<=0.0){
    mTimeUntilNextToggle = 0.5;
    beat[1] = 0.0f;
    beat[2] = 800.0f;
    beat1 = true;
    beat2 = false;
    timerBeat.reset();
    mCam->setPosition(Ogre::Vector3(0.0, 1.0, -85.0));
    color2 = Ogre::Vector3(0.3,0.3,0.3);
    std::cout << "Time between beats: " << beat[2] << std::endl;
    // zoom = false;
    };

    if((mKeyboard->isKeyDown(OIS::KC_NUMPAD3) || mKeyboard->isKeyDown(OIS::KC_3)) && mTimeUntilNextToggle<=0.0){
    mTimeUntilNextToggle = 0.5;
    beat[1] = 0.0f;
    beat[2] = 700.0f;
    beat1 = true;
    beat2 = false;
    timerBeat.reset();
    mCam->setPosition(Ogre::Vector3(0.0, 1.0, -85.0));
    color2 = Ogre::Vector3(0.3,0.3,0.3);
    std::cout << "Time between beats: " << beat[2] << std::endl;
    // zoom = false;
    };

    if((mKeyboard->isKeyDown(OIS::KC_NUMPAD4) || mKeyboard->isKeyDown(OIS::KC_4)) && mTimeUntilNextToggle<=0.0){
    mTimeUntilNextToggle = 0.5;
    beat[1] = 0.0f;
    beat[2] = 600.0f;
    beat1 = true;
    beat2 = false;
    timerBeat.reset();
    mCam->setPosition(Ogre::Vector3(0.0, 1.0, -85.0));
    color2 = Ogre::Vector3(0.3,0.3,0.3);
    std::cout << "Time between beats: " << beat[2] << std::endl;
    // zoom = false;
    };

    if((mKeyboard->isKeyDown(OIS::KC_NUMPAD5) || mKeyboard->isKeyDown(OIS::KC_5)) && mTimeUntilNextToggle<=0.0){
    mTimeUntilNextToggle = 0.5;
    beat[1] = 0.0f;
    beat[2] = 500.0f;
    beat1 = true;
    beat2 = false;
    timerBeat.reset();
    mCam->setPosition(Ogre::Vector3(0.0, 1.0, -85.0));
    color2 = Ogre::Vector3(0.3,0.3,0.3);
    std::cout << "Time between beats: " << beat[2] << std::endl;
    // zoom = false;
    };

    if((mKeyboard->isKeyDown(OIS::KC_NUMPAD6) || mKeyboard->isKeyDown(OIS::KC_6)) && mTimeUntilNextToggle<=0.0){
    mTimeUntilNextToggle = 0.5;
    beat[1] = 0.0f;
    beat[2] = 400.0f;
    beat1 = true;
    beat2 = false;
    timerBeat.reset();
    mCam->setPosition(Ogre::Vector3(0.0, 1.0, -85.0));
    color2 = Ogre::Vector3(0.3,0.3,0.3);
    std::cout << "Time between beats: " << beat[2] << std::endl;
    // zoom = false;
    };

    if((mKeyboard->isKeyDown(OIS::KC_NUMPAD7) || mKeyboard->isKeyDown(OIS::KC_7)) && mTimeUntilNextToggle<=0.0){
    mTimeUntilNextToggle = 0.5;
    beat[1] = 0.0f;
    beat[2] = 300.0f;
    beat1 = true;
    beat2 = false;
    timerBeat.reset();
    mCam->setPosition(Ogre::Vector3(0.0, 1.0, -85.0));
    color2 = Ogre::Vector3(0.3,0.3,0.3);
    std::cout << "Time between beats: " << beat[2] << std::endl;
    // zoom = false;
    };

    if((mKeyboard->isKeyDown(OIS::KC_NUMPAD8) || mKeyboard->isKeyDown(OIS::KC_8)) && mTimeUntilNextToggle<=0.0){
    mTimeUntilNextToggle = 0.5;
    beat[1] = 0.0f;
    beat[2] = 200.0f;
    beat1 = true;
    beat2 = false;
    timerBeat.reset();
    mCam->setPosition(Ogre::Vector3(0.0, 1.0, -85.0));
    color2 = Ogre::Vector3(0.3,0.3,0.3);
    std::cout << "Time between beats: " << beat[2] << std::endl;
    // zoom = false;
    };

    if((mKeyboard->isKeyDown(OIS::KC_NUMPAD9) || mKeyboard->isKeyDown(OIS::KC_9)) && mTimeUntilNextToggle<=0.0){
    mTimeUntilNextToggle = 0.5;
    beat[1] = 0.0f;
    beat[2] = 100.0f;
    beat1 = true;
    beat2 = false;
    timerBeat.reset();
    mCam->setPosition(Ogre::Vector3(0.0, 1.0, -85.0));
    color2 = Ogre::Vector3(0.3,0.3,0.3);
    std::cout << "Time between beats: " << beat[2] << std::endl;
    // zoom = false;
    };

    if(mKeyboard->isKeyDown(OIS::KC_ADD) && mTimeUntilNextToggle<=0.0){
    mTimeUntilNextToggle = 0.5;
    beat[2] -= 50.0f;
    if(beat[2]<=50.0f){ beat[2]=50.0f; };
    std::cout << "Time between beats: " << beat[2] << std::endl;
    };

    if((mKeyboard->isKeyDown(OIS::KC_SUBTRACT) || mKeyboard->isKeyDown(OIS::KC_MINUS)) && mTimeUntilNextToggle<=0.0){
    mTimeUntilNextToggle = 0.5;
    beat[2] += 50.0f;
    if(beat[2]>=500000.0f){ beat[2]=500000.0f; };
    std::cout << "Time between beats: " << beat[2] << std::endl;
    };




    /// ----------------------------------------------------------------------------------------------------------------------------------



    /// Apply the new bottom positions (calculated in audio class loop) for the 2D Rectangles:
    Ogre::Real rLeft  = -0.9990;
    Ogre::Real rRight = -0.9810;
    for(int i=0; i<100; i++){
    rectRow[i]->setCorners(rLeft, 0.0001, rRight, freq[i]); // left, top, right, bottom, updateAABB
    rLeft  -= -0.0200;
    rRight -= -0.0200;
    };


    /// ----------------------------------------------------------------------------------------------------------------------------------



    /// Zoom IN and OUT the camera:
    float cp = mCam->getPosition().z;
    mTranslateVector = Ogre::Vector3::ZERO;
    Ogre::Real moveScale = mCamSpeed*mDeltaTime;
    Ogre::Degree mRotX = Degree(0);
    Ogre::Degree mRotY = Degree(0);
    // tb = timerBeat.getMicroseconds()/1000.0;


    /// DO ANIMATIONS from input audio:

    /// Camera:
    if(freq[0]< -0.02 || freq[0]> 0.02){
    mTranslateVector.z += -(abs(freq[0])); /// Getting near to Earth (zoom in)
    /// Colors:
    color2.x += (15.0*mDeltaTime)/2; /// Set Color
    color2.y += (15.0*mDeltaTime)/2; /// Set Color
    color2.z += (15.0*mDeltaTime)/2; /// Set Color
    } else {
    mTranslateVector.z += +(moveScale/2);  /// Getting far from Earth (zoom out)
    /// Colors:
    color2.x -= (15.0*mDeltaTime)/2; /// Set Color
    color2.y -= (15.0*mDeltaTime)/2; /// Set Color
    color2.z -= (15.0*mDeltaTime)/2; /// Set Color
    };


    /// DO ANIMATIONS from keyboard (beats):

    if(zoom == true && ((beatOnce == true) || (beat[2]>0.0f && (timerBeat.getMicroseconds()/1000.0)>=beat[2]))){
    /// Camera:
    mTranslateVector.z += -(moveScale); /// Getting near to Earth: (zoom in)
    /// Colors:
    color2.x += 15.0*mDeltaTime; /// Set Color
    color2.y += 15.0*mDeltaTime; /// Set Color
    color2.z += 15.0*mDeltaTime; /// Set Color
    if(cp >= -75.0){ timerBeat.reset(); zoom = false; }; /// Near as possible
    };

    if(zoom == false){
    /// Camera:
    mTranslateVector.z += +(moveScale); /// Getting far from Earth: (zoom out)
    /// Colors:
    color2.x -= 15.0*mDeltaTime; /// Set Color
    color2.y -= 15.0*mDeltaTime; /// Set Color
    color2.z -= 15.0*mDeltaTime; /// Set Color
    if(cp <= -85.0){ zoom = true; beatOnce = false; }; /// Far as possible
    };


    /// Secure the above colors limits:
    if(color2.x<=0.3){ color2.x = 0.3; };
    if(color2.y<=0.3){ color2.y = 0.3; };
    if(color2.z<=0.3){ color2.z = 0.3; };
    if(color2.x>=1.0){ color2.x = 1.0; };
    if(color2.y>=1.0){ color2.y = 1.0; };
    if(color2.z>=1.0){ color2.z = 1.0; };

    /// Aplly Colors:
    light2->setDiffuseColour( color2.x,color2.y,color2.z); /// Apply Color
    light2->setSpecularColour(color2.x,color2.y,color2.z); /// Apply Color

    /// Sound Wave:
    Ogre::SceneNode* node = mSmgr->getSceneNode("Plane05");
    if(soundWave == true){
    waveScale = node->getScale();
    waveScale.x += mDeltaTime*(mCamSpeed/2.0);
    waveScale.z += mDeltaTime*(mCamSpeed/2.0);
    if(waveScale.x>=10.0 || waveScale.z>=10.0){ waveScale.x = 1.5; waveScale.z = 1.5; soundWave = false; };
    node->setScale(Ogre::Vector3(waveScale.x, 1.0, waveScale.z)); // Scale
    };
    if(soundWave == false){
    node->setScale(Ogre::Vector3(1.5, 1.0, 1.5)); // Scale
    };



    /// ----------------------------------------------------------------------------------------------------------------------------------


    /// All calculated. Now lets apply changes to the camera:
    mCam->yaw(mRotX);
    mCam->pitch(mRotY);
    mCam->moveRelative(mTranslateVector);

    /// Secure the camera distance limits on Z axis:
    Ogre::Vector3 cpos = mCam->getPosition();
    if(cpos.z>=-75.0){ cpos.z = -75.0; mCam->setPosition(Ogre::Vector3(cpos.x,cpos.y,cpos.z)); soundWave = true;  }; /// Near to earth
    if(cpos.z<=-85.0){ cpos.z = -85.0; mCam->setPosition(Ogre::Vector3(cpos.x,cpos.y,cpos.z)); soundWave = false; }; /// Far from earth



    /// ----------------------------------------------------------------------------------------------------------------------------------


    /// Mouse listener:
    /*
    const OIS::MouseState& mms = mMouse->getMouseState();
    /// Clear texts:
    // cTexts->addTexts("","");
    /// handle right button mouse press...
    if(mms.buttonDown(OIS::MB_Left)){
    /// 1) Update and calculate window dimensions:
    windowResized(mWin);
    /// 2) Update and calculate mouse cursor position:
    Ogre::Real cx = Ogre::Real(mms.X.abs)/Ogre::Real(mms.width);
    Ogre::Real cy = Ogre::Real(mms.Y.abs)/Ogre::Real(mms.height);
    };
    */


    /// ----------------------------------------------------------------------------------------------------------------------------------


    /// In case window was resized...
    /// 1) Update and calculate window dimensions:
    windowResized(mWin);
    Ogre::Real w = mWin->getWidth();
    Ogre::Real h = mWin->getHeight();
    /// 2) Update all text related elements dimensions:
    mPanel->setDimensions(w,h);
    for(int i=0; i<=3; i++){
    mTextArea[i]->setWidth(w-50);
    mTextArea[i]->setHeight(h-50);
    };
    mTextAreaInfos->setWidth(w-50);
    mTextAreaInfos->setHeight(h-50);
    /// 3) Apply custom texts:
    try { /// <--- Just in case has foreign characters and "myfont.ttf" or the "fonts.fontdef" cant handle it...
    mTextArea[0]->setCaption(mTextsLine[0]);
    mTextArea[1]->setCaption(mTextsLine[1]);
    mTextArea[2]->setCaption(mTextsLine[2]);
    mTextArea[3]->setCaption(mTextsLine[3]);
    mTextArea[0]->_update(); /// Needed to force to update some settings. Such as: mTextArea[0]->setSpaceWidth()
    mTextArea[1]->_update(); /// Needed to force to update some settings. Such as: mTextArea[0]->setSpaceWidth()
    mTextArea[2]->_update(); /// Needed to force to update some settings. Such as: mTextArea[0]->setSpaceWidth()
    mTextArea[3]->_update(); /// Needed to force to update some settings. Such as: mTextArea[0]->setSpaceWidth()
    } catch (exception& ex) { };
    /// 4) Apply system infos texts:
    mTextAreaInfos->setCaption(mTempTexts);
    mTextAreaInfos->_update(); /// Needed to force to update some settings. Such as: mTextArea[0]->setSpaceWidth()


    /// ----------------------------------------------------------------------------------------------------------------------------------


    /// Updating Animations:
    if(animstate[0]){ animstate[0]->addTime(mDeltaTime*30);  }; /// Earth anim
    if(animstate[1]){ animstate[1]->addTime(mDeltaTime*180); }; /// Clouds anim
    if(animstate[2]){ animstate[2]->addTime(mDeltaTime*10);  }; /// Moon anim

};




void Scene001::deleteScene(){
    /// Delete All Loaded objects for this scene...
};



void Scene001::destroy(){
    /// Destroy all...
    /// Destroy rectangles:
    for(int i=0; i<100; i++){ if(rectRow[i]){ delete rectRow[i]; }; };
};



