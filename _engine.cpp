
/*
Developer: Jorge Rosa (jorge.bigarte@gmail.com) - With love to: ASCR
Website: https://sites.google.com/site/jorgerosaportfolio
Mixxx DarkMetal Skin: http://www.mixxx.org/forums/viewtopic.php?f=8&t=6052
*/

/// SHARED VARS:

/// Ogre - OIS: (input devices)
OIS::InputManager* mInputManager;
OIS::Mouse* mMouse;
OIS::Keyboard* mKeyboard;
OIS::JoyStick* mJoy;

/// Ogre - Variables:
Ogre::Root* mRoot;
Ogre::RenderWindow* mWin;
Ogre::SceneManager* mSmgr;
Ogre::Viewport* mViewport;
Ogre::Camera* mCam;
Ogre::String mSettingsPath      = "Data/";
Ogre::String mResourcesPath     = "Data/Files/";
Ogre::String mLogoSceneFile     = "logo-mixxx.scene"; /// (Value set in "settings.xml" file)
Ogre::String mLogoSceneVisible  = "true";             /// (Value set in "settings.xml" file)
Ogre::String mMainSceneFile     = "scene-001.scene";  /// (Value set in "settings.xml" file)
Ogre::String mMainSceneVisible  = "true";             /// (Value set in "settings.xml" file)
Ogre::String mTextsLine[10]     = "";                 /// (Value set in "settings.xml" file)
Ogre::String mMainFont          = "RationalInteger";  /// (Value set in "settings.xml" file)
Ogre::String mMainFontSize      = "20";               /// (Value set in "settings.xml" file)

/// Ogre - Timer Variables:
Ogre::Real mTimeCurrentFrame    = 0;
Ogre::Real mTimeLastFrame       = 0;
Ogre::Real mDeltaTime           = 0; // mTimeSinceLastFrame  = 0;
Ogre::Real mTimeUntilNextToggle = 0;
Ogre::Real mTimeKeyUpPressed    = 0;

/// Ogre - Texts:
Ogre::Overlay* mOverlay;                      // Create a overlay element
Ogre::OverlayManager* mOverlayMgr;            // Create a overlay manager
Ogre::OverlayContainer* mPanel;               // Create a overlay panel
Ogre::TextAreaOverlayElement* mTextArea[10];  // Create a text area (Custom Texts)
Ogre::TextAreaOverlayElement* mTextAreaInfos; // Create a text area (System Infos Texts)
Ogre::String mTempTexts = "";

/// Animations:
Ogre::AnimationState* animstate[10];
/// Particles:
Ogre::ParticleSystem* ps[10];
/// Timers:
Ogre::Timer timerBeat;

/// Custom Settings: (wide string, because will (possibly) receive foreign characters (portuguese) from xml)
std::wstring arrayCustomSettings[10] = {};

/// Other Variables:
bool exitAll = false;
int devicesWereInitialized = 0;


Ogre::String caption;



class Engine{

    public:
    Engine();  // Constructor
    ~Engine(); // Destructor
    void loadCustomSettings(); /// Load: Custom Settings from .XML file
    std::string convertWideStringToString(std::wstring wstr); /// Allow incoming foreign characters from .XML file
    void createVideoAndInputs(); /// Initialize: Video + Keyboard + Mouse
    void listenInputs();         /// Listen Keyboard
    void windowResized(RenderWindow* rw);
    void createEnvironment();
    void destroy();

    private:

};

Engine::Engine(){};  // Constructor (Can accept arguments, but this one does not)
Engine::~Engine(){}; // Destructor (do not accept arguments)


void Engine::loadCustomSettings(){

    /// RESET array:
    for(int i=0; i<10; i++){ arrayCustomSettings[i] = L""; };

    /// Create path:
    std::string settingsLoadPath = mSettingsPath + "settings.xml";

    std::cout << "------------------------------------------------" << std::endl;
    std::cout << "Engine: Loading Custom Settings..."               << std::endl;
    std::cout << "------------------------------------------------" << std::endl;

    /// Load data:
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(settingsLoadPath.c_str(), pugi::parse_minimal, pugi::encoding_auto); /// pugi::encoding_latin1
    if(result){ /// Handles errors: File cot found + Malformed XML + Etc... (voids this app to crash!)

    /// Load data to an array:
    pugi::xml_node n = doc.child(L"root").child(L"settings");
    arrayCustomSettings[0] = n.attribute(L"logoSceneFile").as_string();
    arrayCustomSettings[1] = n.attribute(L"logoSceneVisible").as_string();
    arrayCustomSettings[2] = n.attribute(L"mainSceneFile").as_string();
    arrayCustomSettings[3] = n.attribute(L"mainSceneVisible").as_string();
    arrayCustomSettings[4] = n.attribute(L"textsLine1").as_string();
    arrayCustomSettings[5] = n.attribute(L"textsLine2").as_string();
    arrayCustomSettings[6] = n.attribute(L"textsLine3").as_string();
    arrayCustomSettings[7] = n.attribute(L"textsLine4").as_string();
    arrayCustomSettings[8] = n.attribute(L"mainFont").as_string();
    arrayCustomSettings[9] = n.attribute(L"mainFontSize").as_string();

    /// Loaded! Free memory now:
    if(n != NULL && !n.empty()){ n.empty(); };
    if(doc != NULL && !doc.empty()){ doc.empty(); };

    /// Apply loaded data to internal vars:
    mLogoSceneFile    = convertWideStringToString(arrayCustomSettings[0]);
    mLogoSceneVisible = convertWideStringToString(arrayCustomSettings[1]);
    mMainSceneFile    = convertWideStringToString(arrayCustomSettings[2]);
    mMainSceneVisible = convertWideStringToString(arrayCustomSettings[3]);
    mTextsLine[0]     = Ogre::UTFString(arrayCustomSettings[4]);
    mTextsLine[1]     = Ogre::UTFString(arrayCustomSettings[5]);
    mTextsLine[2]     = Ogre::UTFString(arrayCustomSettings[6]);
    mTextsLine[3]     = Ogre::UTFString(arrayCustomSettings[7]);
    mMainFont         = convertWideStringToString(arrayCustomSettings[8]);
    mMainFontSize     = convertWideStringToString(arrayCustomSettings[9]);

    std::cout << "------------------------------------------------" << std::endl;
    std::cout << "Engine: Custom Settings Loaded Successfully."     << std::endl;
    std::cout << "------------------------------------------------" << std::endl;

    } else {

    std::cout << "------------------------------------------------" << std::endl;
    std::cout << "Engine: ERROR Loading Custom Settings!"           << std::endl;
    std::cout << "------------------------------------------------" << std::endl;

    };

};



/// WString -- TO --> String
std::string Engine::convertWideStringToString(std::wstring wstr){
    if(wstr.empty()){
    return "";
    };
    if(!wstr.empty()){
    std::wstring tmp = L"";
    tmp += wstr;
    std::string text(tmp.begin(), tmp.end());
    return text;
    };
};





void Engine::createVideoAndInputs(){

    std::cout << "------------------------------------------------" << std::endl;
    std::cout << "Engine: Creating Video..."                        << std::endl;
    std::cout << "------------------------------------------------" << std::endl;

    /// Load plugins and ogre configs from config file:
    Ogre::String pluginsPath;
    pluginsPath = mResourcesPath+"plugins.cfg";
    mRoot = OGRE_NEW Root(pluginsPath, mResourcesPath+"ogre.cfg", mResourcesPath+"ogre.log");

    /// Auto fill render options:
    const Ogre::RenderSystemList& rsl = mRoot->getAvailableRenderers();
    if(rsl.size()){
    Ogre::RenderSystem* r = rsl.at(0);
    mRoot->setRenderSystem(r);
    r->setConfigOption("Colour Depth", "32");
    r->setConfigOption("VSync", "No");
    r->setConfigOption("Full Screen", "No");
    r->setConfigOption("Video Mode", "600 x 480");
    r->setConfigOption("FSAA", "6"); /// QUALITY STUFF: Antialiasing ---> 0,2,4,6,8,16 -> Big FPS decrease !!!
    /// r->setConfigOption("Display Frequency", "60");
    };

    /// Load Resources (meshes, textures, data, audios, etc, etc) - Paths are coming from the ".cfg" file:
    ConfigFile cf;
    cf.load(mResourcesPath+"resources.cfg");
    // Go through all sections & settings in the file
    ConfigFile::SectionIterator seci = cf.getSectionIterator();
    Ogre::String secName, typeName, archName;
    while (seci.hasMoreElements()){
    secName = seci.peekNextKey();
    ConfigFile::SettingsMultiMap* settings = seci.getNext();
    ConfigFile::SettingsMultiMap::iterator i;
    for(i=settings->begin(); i!=settings->end(); ++i){
    typeName = i->first;
    archName = i->second;
    ResourceGroupManager::getSingleton().addResourceLocation(archName, typeName, secName);
    };
    };

    // Setup the rendering window:
    mWin = mRoot->initialise(true, "MOVE - Mixxx Ogre3D Visual Effects"); /// The first parameter is whether or not Ogre should create a RenderWindow for you.
    // Set default mipmap level: (note: some APIs ignore this)

    /// mWin->setFullscreen(true, 1080, 1920);

    /// QUALITY STUFF ---> Big FPS decrease !!!
    Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(2); // The default value is 0
    Ogre::MaterialManager::getSingleton().setDefaultTextureFiltering(TFO_ANISOTROPIC); // TFO_NONE | TFO_ANISOTROPIC
    Ogre::MaterialManager::getSingleton().setDefaultAnisotropy(2); // Default is 1 (no anisotropy)

    /// initialise ALL resource groups: (so, they need to be all before this line)
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

    std::cout << "------------------------------------------------" << std::endl;
    std::cout << "Engine: Video Created Successfully."              << std::endl;
    std::cout << "------------------------------------------------" << std::endl;



    std::cout << "------------------------------------------------" << std::endl;
    std::cout << "Engine: Initializing OIS..."                      << std::endl;
    std::cout << "------------------------------------------------" << std::endl;
    /// Ogre::LogManager::getSingletonPtr()->logMessage("Engine: Initializing OIS...");

    size_t windowHnd = 0;
    std::ostringstream windowHndStr;
    mWin->getCustomAttribute("WINDOW", &windowHnd);
    windowHndStr << windowHnd;
    OIS::ParamList pl;
    pl.insert(std::make_pair(Ogre::String("WINDOW"), windowHndStr.str()));

    /// Show/Hide Mouse Cursor:
    #if defined OIS_WIN32_PLATFORM
    pl.insert(std::make_pair(std::string("w32_mouse"),    std::string("DISCL_FOREGROUND" )));
    pl.insert(std::make_pair(std::string("w32_mouse"),    std::string("DISCL_NONEXCLUSIVE")));
    pl.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_FOREGROUND")));
    pl.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_NONEXCLUSIVE")));
    #elif defined OIS_LINUX_PLATFORM
    pl.insert(std::make_pair(std::string("x11_mouse_grab"),    std::string("false")));
    pl.insert(std::make_pair(std::string("x11_mouse_hide"),    std::string("false")));
    pl.insert(std::make_pair(std::string("x11_keyboard_grab"), std::string("false")));
    pl.insert(std::make_pair(std::string("XAutoRepeatOn"),     std::string("true")));
    #endif

    mInputManager = OIS::InputManager::createInputSystem(pl);
    mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject(OIS::OISKeyboard, false)); // false = disabled
    mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject(OIS::OISMouse, false));          // false = disabled

    // Set initial mouse clipping size:
    windowResized(mWin);

    std::cout << "------------------------------------------------" << std::endl;
    std::cout << "Engine: OIS Initialized Successfully."            << std::endl;
    std::cout << "------------------------------------------------" << std::endl;
};



/// On Window resizing: Adjust mouse clipping area
void Engine::windowResized(RenderWindow* rw){
    unsigned int width, height, depth;
    int left, top;
    rw->getMetrics(width, height, depth, left, top);

    const OIS::MouseState& ms = mMouse->getMouseState();
    ms.width = width;
    ms.height = height;
};



/// OIS::KeyListener:
void Engine::listenInputs(){
    // Listening...
};


void Engine::createEnvironment(){

    std::cout << "------------------------------------------------" << std::endl;
    std::cout << "Engine: Creating Texts..."                        << std::endl;
    std::cout << "------------------------------------------------" << std::endl;

    /// Initialize Texts:
    mOverlayMgr = Ogre::OverlayManager::getSingletonPtr();
    mPanel = static_cast<Ogre::OverlayContainer*>(mOverlayMgr->createOverlayElement("Panel", "Container_1"));
    mPanel->setMetricsMode(Ogre::GMM_PIXELS);
    mPanel->setPosition(0,0);
    mPanel->setDimensions((Real)mWin->getWidth(), (Real)mWin->getHeight());
    // mPanel->setMaterialName("Mat_Something");

    int textPosY = 80;
    int charHeight = Ogre::StringConverter::parseReal(mMainFontSize); /// Converts string into numbers
    for(int i=0; i<=3; i++){
    /// Texts Area: (Custom texts)
    Ogre::String n = Ogre::StringConverter::toString(i); /// Converts numbers into string
    mTextArea[i] = static_cast<TextAreaOverlayElement*>(mOverlayMgr->createOverlayElement("TextArea", "Custom_Text_"+n));
    mTextArea[i]->setMetricsMode(Ogre::GMM_PIXELS);
    mTextArea[i]->setPosition(0,textPosY);
    // mTextArea[i]->setDimensions(w,h);
    mTextArea[i]->setWidth((Ogre::Real)mWin->getWidth());
    mTextArea[i]->setHeight((Ogre::Real)mWin->getHeight());
    mTextArea[i]->setCharHeight(charHeight);
    mTextArea[i]->setFontName(mMainFont); /// Name must be defined in "fonts.fontdef" and in "settings.xml"
    /// The next code line only works if set in ".fontdef" file + the font MUST have that characters included, else... Crashes!
    /// mTextArea[i]->setParameter("code_points", "33-166 192-255"); /// By default Ogre only creates code_points from 33 to 166 (needed to be set for foreign idioms characters)
    mTextArea[i]->setSpaceWidth(10.0);
    mTextArea[i]->setColourTop(ColourValue(0.9, 0.9, 1.0));
    mTextArea[i]->setColourBottom(ColourValue(0.5, 0.5, 0.6));
    mTextArea[i]->setAlignment(Ogre::TextAreaOverlayElement::Center);
    mTextArea[i]->setHorizontalAlignment(Ogre::GHA_CENTER); /// Ogre::GHA_LEFT || Ogre::GHA_CENTER || Ogre::GHA_RIGHT
    mTextArea[i]->setVerticalAlignment(Ogre::GVA_CENTER);   /// Ogre::GVA_TOP || Ogre::GVA_CENTER || Ogre::GVA_BOTTOM
    mTextArea[i]->setCaption("");
    textPosY += (charHeight+2);
    };
    /// Text Area: (Info Texts)
    mTextAreaInfos = static_cast<TextAreaOverlayElement*>(mOverlayMgr->createOverlayElement("TextArea", "Info_Texts_1"));
    mTextAreaInfos->setMetricsMode(Ogre::GMM_PIXELS);
    mTextAreaInfos->setPosition(0,textPosY);
    // mTextAreaInfos->setDimensions(w,h);
    mTextAreaInfos->setWidth((Ogre::Real)mWin->getWidth());
    mTextAreaInfos->setHeight((Ogre::Real)mWin->getHeight());
    mTextAreaInfos->setCharHeight(charHeight);
    mTextAreaInfos->setFontName(mMainFont); /// Name must be defined in "fonts.fontdef" and in "settings.xml"
    /// The next code line only works if set in ".fontdef" file + the font MUST have that characters included, else... Crashes!
    /// mTextArea[i]->setParameter("code_points", "33-166 192-255"); /// By default Ogre only creates code_points from 33 to 166 (needed to be set for foreign idioms characters)
    mTextAreaInfos->setSpaceWidth(10.0);
    mTextAreaInfos->setColourTop(ColourValue(0.9, 0.3, 0.3));
    mTextAreaInfos->setColourBottom(ColourValue(0.5, 0.2, 0.2));
    mTextAreaInfos->setAlignment(Ogre::TextAreaOverlayElement::Center);
    mTextAreaInfos->setHorizontalAlignment(Ogre::GHA_CENTER); /// Ogre::GHA_LEFT || Ogre::GHA_CENTER || Ogre::GHA_RIGHT
    mTextAreaInfos->setVerticalAlignment(Ogre::GVA_CENTER);   /// Ogre::GVA_TOP || Ogre::GVA_CENTER || Ogre::GVA_BOTTOM
    mTextAreaInfos->setCaption("");
    /// Finally:
    mOverlay = mOverlayMgr->create("Overlay_1");
    mOverlay->add2D(mPanel);
    for(int i=0; i<=3; i++){ mPanel->addChild(mTextArea[i]); };
    mPanel->addChild(mTextAreaInfos);
    mOverlay->show();



    std::cout << "------------------------------------------------" << std::endl;
    std::cout << "Engine: Texts Created Successfully."              << std::endl;
    std::cout << "------------------------------------------------" << std::endl;


    /// Meshes World:
    mSmgr = mRoot->createSceneManager(ST_GENERIC, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

    /// Global Light:
    mSmgr->setAmbientLight(Ogre::ColourValue(0.0, 0.0, 0.0));

    /// Shadows:
    mSmgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE); /// SHADOWTYPE_STENCIL_ADDITIVE or SHADOWTYPE_STENCIL_MODULATIVE
    mSmgr->setShadowTextureSize(512);
    mSmgr->setShadowTextureCount(1);
    mSmgr->setShadowTextureSelfShadow(false);
    mSmgr->setShadowColour(ColourValue(0.0, 0.0, 0.0));

    /// Create Camera:
    mCam = mSmgr->createCamera("Camera_N1");
    mCam->setPosition(Ogre::Vector3(0.0, 1.0, -85.0));
    // cam->setDirection(0, 0, -1);
    mCam->lookAt(Ogre::Vector3(0.0, 1.0, 0.0));
    mCam->setNearClipDistance(5.0);
    mCam->setFarClipDistance(5000.0);
    mCam->setCastShadows(false); // Cast shadows?

    /// Add camera to viewport:
    mViewport = mWin->addViewport(mCam);

    /// Must be after camera added to viewport:
    mCam->setAspectRatio(Ogre::Real(mViewport->getActualWidth()) / Ogre::Real(mViewport->getActualHeight())); // Set to the correct ratio
    /// mCam->setFOVy(Ogre::Radian(Ogre::Degree(10.0).valueRadians())); /// The 2D feeling

    /// Set Viewport Settings: (Viewport must be placed AFTER Camera was added)
    mViewport->setAutoUpdated(true);
    mViewport->setBackgroundColour(Ogre::ColourValue(0.0, 0.0, 0.0, 0.0)); /// Background colors: r,g,b,a
    // viewport->setOverlaysEnabled(true);

};


void Engine::destroy(){
    /// Destroy all...
    if(mWin){ mWin->destroy(); };
};

