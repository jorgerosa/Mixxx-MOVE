
/*
Developer: Jorge Rosa (jorge.bigarte@gmail.com) - With love to: ASCR
Website: https://sites.google.com/site/jorgerosaportfolio
Mixxx DarkMetal Skin: http://www.mixxx.org/forums/viewtopic.php?f=8&t=6052
*/


/// SHARED VARS:
Ogre::String mDeviceName[100];
Ogre::String mDeviceStatus[100];
int mDevice = 0;
int mTotalDevices = 0;
Ogre::Real freq[100]; /// Positions for peakmeters
bool beatFromAudio = false;
bool animFromAudio = false;
bool soundWave = false;

RtAudio rtaudio;
RtAudio::DeviceInfo info;



struct rtData{
    unsigned char channels     = 2;
    unsigned int sampleRate    = 44100;
    RtAudioFormat sampleFormat = RTAUDIO_SINT16;
    unsigned int bufferFrames  = 512;
    short* displayBuffer       = NULL;
};


/// This 'inout_Callback' function will be called every 512 samples.
/// Since we've setup a 44100Hz stream, this function will be called every 1000/(44100/512) == 0.0116 Seconds (11.6 milliseconds).
int inout_Callback(void* outputBuffer, void* inputBuffer, unsigned int nBufferFrames, double streamTime, RtAudioStreamStatus status, void* userData){
    rtData* da = (rtData*)userData;
    memcpy(outputBuffer, inputBuffer, da->bufferFrames*da->channels*sizeof(short));               /// Send audio to headphones.
    memcpy(da->displayBuffer, (short*)outputBuffer, da->bufferFrames*da->channels*sizeof(short)); /// Send audio to display.
    return 0;
};




class Audio : public Engine{

    public:
    Audio();  // Constructor
    ~Audio(); // Destructor
    void initializeDevices();
    void initializeAudioStream();
    void doLoop();
    void destroy();

    rtData data;
    std::map<int, std::string> apiMap;
    std::vector<RtAudio::Api> apis;

    private:

};

Audio::Audio(){};  // Constructor (Can accept arguments, but this one does not)
Audio::~Audio(){}; // Destructor (do not accept arguments)





void Audio::initializeDevices(){


    std::cout << "------------------------------------------------" << std::endl;
    std::cout << "Audio: Initializing..."                           << std::endl;
    std::cout << "------------------------------------------------" << std::endl;

    for (int i=0; i<100; i++){ freq[i] = 0.0; };
    for (int i=0; i<100; i++){ mDeviceName[i]   = ""; };
    for (int i=0; i<100; i++){ mDeviceStatus[i] = ""; };
    /// if (rtaudio.getDeviceCount()<1){ exit(0); };


    try {

    /// mDevice = 1; /// <----------------------------------------- DEFAULT DEVICE --- WORKS ONLY IN MY CASE (TO BE DISABLED) -------------------------------------------------
    mDevice = rtaudio.getDefaultInputDevice();

    apiMap[RtAudio::MACOSX_CORE]    = "OS-X Core Audio";
    apiMap[RtAudio::WINDOWS_ASIO]   = "Windows ASIO";
    apiMap[RtAudio::WINDOWS_DS]     = "Windows Direct Sound";
    apiMap[RtAudio::WINDOWS_WASAPI] = "Windows WASAPI";
    apiMap[RtAudio::UNIX_JACK]      = "Jack Client";
    apiMap[RtAudio::LINUX_ALSA]     = "Linux ALSA";
    apiMap[RtAudio::LINUX_PULSE]    = "Linux PulseAudio";
    apiMap[RtAudio::LINUX_OSS]      = "Linux OSS";
    apiMap[RtAudio::RTAUDIO_DUMMY]  = "RtAudio Dummy";

    RtAudio::getCompiledApi(apis);
    std::cout << "RtAudio version: " << RtAudio::getVersion() << std::endl;
    std::cout << "RtAudio internal compiled APIs:" << std::endl;
    for(unsigned int i=0; i<apis.size(); i++){ std::cout << i+1 << ") " << apiMap[apis[i]] << std::endl; };
    std::cout << "Current API in use: " << apiMap[rtaudio.getCurrentApi()] << std::endl;
    std::cout << std::endl;

    mTotalDevices = rtaudio.getDeviceCount(); /// Returns 3 (only in my case)

    std::cout << "Devices found: " << mTotalDevices << std::endl;
    std::cout << "Testing now ..." << std::endl;

    for (unsigned int i=0; i<mTotalDevices; i++){
    info = rtaudio.getDeviceInfo(i);

    std::cout << std::endl;
    std::cout << "Device Name: " << info.name << std::endl;
    if (info.probed == false){
    mDeviceName[i] = info.name;
    mDeviceStatus[i] = "Failed";
    std::cout << "Probe Status: FAILED!" << std::endl;
    };
    if (info.probed == true){
    mDeviceName[i] = info.name;
    mDeviceStatus[i] = "Success";
    std::cout << "Probe Status: SUCCESS!" << std::endl;
    std::cout << "Output Channels: " << info.outputChannels << std::endl;
    std::cout << "Input Channels: "  << info.inputChannels  << std::endl;
    std::cout << "Duplex Channels: " << info.duplexChannels << std::endl;
    if (info.isDefaultOutput){ std::cout << "This is the default output device." << std::endl; } else { std::cout << "This is NOT the default output device." << std::endl; };
    if (info.isDefaultInput ){ std::cout << "This is the default input device."  << std::endl; } else { std::cout << "This is NOT the default input device."  << std::endl; };
    if (info.nativeFormats == 0 ){ std::cout << "No natively supported data formats! (???)";   } else { std::cout << "Natively supported data formats: ";  };
    if (info.nativeFormats & RTAUDIO_SINT8  ){ std::cout << "8-bit int ";    };
    if (info.nativeFormats & RTAUDIO_SINT16 ){ std::cout << "16-bit int ";   };
    if (info.nativeFormats & RTAUDIO_SINT24 ){ std::cout << "24-bit int ";   };
    if (info.nativeFormats & RTAUDIO_SINT32 ){ std::cout << "32-bit int ";   };
    if (info.nativeFormats & RTAUDIO_FLOAT32){ std::cout << "32-bit float "; };
    if (info.nativeFormats & RTAUDIO_FLOAT64){ std::cout << "64-bit float "; };
    };

    if (info.sampleRates.size()<1){
    std::cout << "No supported sample rates found!" << std::endl;
    std::cout << std::endl;
    } else {
    std::cout << "Supported sample rates: "; for (unsigned int j=0; j<info.sampleRates.size(); j++){ std::cout << info.sampleRates[j] << " "; };
    std::cout << std::endl;
    };

    };

    } catch (exception& ex) {

    std::cout << "------------------------------------------------" << std::endl;
    std::cerr << "Audio: ERROR - " << ex.what()                     << std::endl;
    std::cout << "------------------------------------------------" << std::endl;

    };

};



void Audio::initializeAudioStream(){

    std::cout << "------------------------------------------------" << std::endl;
    std::cout << "Audio: Streaming..."                              << std::endl;
    std::cout << "------------------------------------------------" << std::endl;

    /// Delete any already opened stream:
    try { if(rtaudio.isStreamRunning()){ rtaudio.stopStream(); }; } catch (exception& e) { };
    try { if(rtaudio.isStreamOpen()){ rtaudio.closeStream(); };   } catch (exception& e) { };



    try {

    if(mDeviceStatus[mDevice]=="Success" && mDevice<mTotalDevices){

    /// Clear possible previous data:
    data.channels      = 2;
    data.sampleRate    = 44100;
    data.sampleFormat  = RTAUDIO_SINT16;
    data.bufferFrames  = 512;
    data.displayBuffer = NULL;

    /// Now creates a new stream:
    RtAudio::StreamParameters inParams;
    inParams.deviceId  = mDevice; /// rtaudio.getDefaultInputDevice();
    inParams.nChannels = data.channels;

    RtAudio::StreamParameters outParams;
    outParams.deviceId  = mDevice; /// rtaudio.getDefaultOutputDevice();
    outParams.nChannels = data.channels;

    rtaudio.openStream(&outParams, &inParams, data.sampleFormat, data.sampleRate, &data.bufferFrames, &inout_Callback, (void*)&data, NULL, NULL);
    rtaudio.startStream();

    /// Initialize display buffer:
    data.displayBuffer = new short[data.bufferFrames*data.channels];
    memset(data.displayBuffer, 0, data.bufferFrames*data.channels*sizeof(short));

    } else {

    std::cout << "------------------------------------------------" << std::endl;
    std::cout << "Audio: Device N." << mDevice+1 << " failed!"        << std::endl;
    std::cout << "------------------------------------------------" << std::endl;

    };

    std::cout << "------------------------------------------------" << std::endl;
    std::cout << "Audio: Done."                                     << std::endl;
    std::cout << "------------------------------------------------" << std::endl;

    } catch (exception& ex) {

    std::cout << "------------------------------------------------" << std::endl;
    std::cerr << "Audio: ERROR - " << ex.what()                     << std::endl;
    std::cout << "------------------------------------------------" << std::endl;

    };

};



void Audio::doLoop(){

    try {
    if(data.displayBuffer != NULL){
    // for(int i=0;  == 512
    for(int i=0; i<100 && i<data.bufferFrames; i++){
    /// Calculate bottom rectangles (peaks) new positions:
    freq[i] = (data.displayBuffer[i*10]/20000.0) - 0.0001;
    };
    };
    } catch (exception& e) { };

};



void Audio::destroy(){

    /// Destroy all...
    /// Delete any already opened stream:
    try { if(rtaudio.isStreamRunning()){ rtaudio.stopStream(); }; } catch (exception& e) { };
    try { if(rtaudio.isStreamOpen()){ rtaudio.closeStream(); };   } catch (exception& e) { };

};



