/*  File    : BCI2000OnlineLink.cpp
 */

#include "Sockets.h"
#include "Streambuf.h"
#include "Thread.h"
#include "WaitableEvent.h"
#include "Runnable.h"
#include "GenericSignal.h"

#define S_FUNCTION_LEVEL 2
#define S_FUNCTION_NAME  BCI2000OnlineLink

/*
 * Need to include simstruc.h for the definition of the SimStruct and
 * its associated macro definitions.
 */
#include "simstruc.h"

class BCI2000OnlineLink
{
public:
    BCI2000OnlineLink();
    ~BCI2000OnlineLink();
    bool Connect(const std::string&);
    void WaitForData();
    bool Connect(const SimStruct*);
    int Channels() const;
    double SampleDuration() const;
    void Outputs(real_T*);

    static const char* SetErrorStatus(const std::string&);

private:
    void ThreadFunc();

    ClientTCPSocket mSocket;
    std::atomic<bool> mConnected;
    SynchronizedObject<GenericSignal> mpSignal;
    int mChannels;
    double mSampleDuration;
    Time::Interval mTimeoutDuration;
    std::atomic<int> mElements, mIndex;

    Thread mThread;
    MemberCall<void(BCI2000OnlineLink*)> mThreadCall;

    WaitableEvent mDataAvailable;

    static char* spErrorStatus;
};

char* BCI2000OnlineLink::spErrorStatus;

const char* BCI2000OnlineLink::SetErrorStatus(const std::string& s)
{
    delete[] spErrorStatus;
    if (s.empty())
    {
        spErrorStatus = nullptr;
    }
    else
    {
        spErrorStatus = new char[s.length() + 1];
        ::memcpy(spErrorStatus, s.data(), s.length());
        spErrorStatus[s.length()] = 0;
    }
    return spErrorStatus;
}

BCI2000OnlineLink::BCI2000OnlineLink()
    : mThreadCall(&BCI2000OnlineLink::ThreadFunc, this),
    mThread(&mThreadCall, "BCI2000OnlineLink listening/receiving thread"),
    mDataAvailable(true), mIndex(0), mElements(0),
    mChannels(0), mSampleDuration(NaN<double>())
{
}

BCI2000OnlineLink::~BCI2000OnlineLink()
{
    mThread.Terminate();
}

bool BCI2000OnlineLink::Connect(const std::string& address)
{
    mSocket.Open(address);
    if (!mSocket.Connected())
        return false;
    mIndex = 0;
    mThread.Start();
    if (!mDataAvailable.Wait(TimeUtils::TimeInterval::Seconds(0.5)))
        return false;
    if (mChannels <= 0)
        return false;
    mConnected = true;
    return true;
}

bool BCI2000OnlineLink::Connect(const SimStruct* S)
{
    const mxArray* pVal0 = ssGetSFcnParam(S, 0);
    char* pAddr = ::mxArrayToString(pVal0);
    std::string addr;
    if (pAddr)
    {
        addr = pAddr;
        ::mxFree(pAddr);
    }
    if (!Connect(addr)) {
        std::string err = "Could not connect to " + addr + ", is BCI2000 running?";
        ssSetErrorStatus(S, SetErrorStatus(err));
        return false;
    }
    return true;
}

void BCI2000OnlineLink::WaitForData()
{
    mDataAvailable.Wait(mTimeoutDuration);
}

int BCI2000OnlineLink::Channels() const
{
    return mChannels;
}

double BCI2000OnlineLink::SampleDuration() const
{
    return mSampleDuration;
}

void BCI2000OnlineLink::Outputs(real_T* y)
{
    if (mIndex == 0)
        WaitForData();
    WithLocked(p = mpSignal.Const())
    {
        real_T* pOut = y;
        for (int ch = 0; ch < p->Channels(); ++ch)
            *pOut++ = p->Value(ch, mIndex);
    }
    if (++mIndex >= mElements)
        mIndex = 0;
}

void BCI2000OnlineLink::ThreadFunc()
{
    UnbufferedIO buf;
    buf.SetInput(&mSocket.Input());
    std::istream stream(&buf);
    if (stream && mSocket.Input().Wait()) // the first data to be received is a signal properties object
    {
        SignalProperties properties;
        properties.Unserialize(stream);
        mChannels = properties.Channels();
        mElements = properties.Elements();
        mSampleDuration = 1.0 / properties.SamplingRate();
        mTimeoutDuration = Time::Seconds(4 * mSampleDuration * mElements);
        mDataAvailable.Set();
    }
    while (stream && mSocket.Input().Wait()) // will be interrupted by Thread::Terminate()
    {
        mpSignal.Mutable()->Unserialize(stream); // get current signal content
        mDataAvailable.Set();
    }
    mConnected = false;
    mDataAvailable.Set();
}

/*====================*
 * S-function methods *
 *====================*/

#define MDL_CHECK_PARAMETERS
#if defined(MDL_CHECK_PARAMETERS)  && defined(MATLAB_MEX_FILE)
/*
 * Check to make sure that parameter is a string
 */
static void mdlCheckParameters(SimStruct *S)
{
    const mxArray *pVal0 = ssGetSFcnParam(S, 0);
    if (mxIsEmpty(pVal0) || !mxIsChar(pVal0)) {
        ssSetErrorStatus(S, "Parameter to BCI2000OnlineLink must be a non-empty character string");
        return;
    }
}
#endif


/* Function: mdlInitializeSizes ===============================================
 * Abstract:
 *    The sizes information is used by Simulink to determine the S-function
 *    block's characteristics (number of inputs, outputs, states, etc.).
 */
static void mdlInitializeSizes(SimStruct *S)
{
    ssSetNumSFcnParams(S, 1);  /* Number of expected parameters */
#if defined(MATLAB_MEX_FILE)
    if (ssGetNumSFcnParams(S) == ssGetSFcnParamsCount(S)) {
        mdlCheckParameters(S);
        if (ssGetErrorStatus(S) != NULL) {
            return;
        }
    } else {
        return; /* Parameter mismatch will be reported by Simulink */
    }
#endif
    ssSetSFcnParamTunable(S, 0, 0);

    ssSetNumContStates(S, 0);
    ssSetNumDiscStates(S, 0);

    if (!ssSetNumInputPorts(S, 0)) return;
    
    if (!ssSetNumOutputPorts(S, 1)) return;
    BCI2000OnlineLink link;
    if (!link.Connect(S))
        return;
    ssSetOutputPortWidth(S, 0, link.Channels());

    ssSetNumSampleTimes(S, 1);
    ssSetNumRWork(S, 0);
    ssSetNumIWork(S, 0);
    ssSetNumPWork(S, 1);
    ssSetNumModes(S, 0);
    ssSetNumNonsampledZCs(S, 0);

    ssSetOperatingPointCompliance(S, DISALLOW_SIM_STATE);

    ssSetOptions(S, 0);
}



/* Function: mdlInitializeSampleTimes =========================================
 * Abstract:
 *    This function is used to specify the sample time(s) for your
 *    S-function. You must register the same number of sample times as
 *    specified in ssSetNumSampleTimes.
 */
static void mdlInitializeSampleTimes(SimStruct *S)
{
    BCI2000OnlineLink link;
    if (link.Connect(S))
    {
        ssSetSampleTime(S, 0, link.SampleDuration());
        ssSetOffsetTime(S, 0, 0.0);
    }
}

#define MDL_START  /* Change to #undef to remove function */
#if defined(MDL_START) 
  /* Function: mdlStart =======================================================
   * Abstract:
   *    This function is called once at start of model execution. If you
   *    have states that should be initialized once, this is the place
   *    to do it.
   */
  static void mdlStart(SimStruct *S)
  {
    auto p = new BCI2000OnlineLink;
    p->Connect(S);
    p->WaitForData(); // sync mdlOutputs with next block to avoid artifacts
    ssSetPWorkValue(S, 0, p);
  }
#endif /*  MDL_START */

/* Function: mdlOutputs =======================================================
 * Abstract:
 *    In this function, you compute the outputs of your S-function
 *    block.
 */
static void mdlOutputs(SimStruct *S, int_T tid)
{
    auto p = static_cast<BCI2000OnlineLink*>(ssGetPWorkValue(S, 0));
    real_T* y = ssGetOutputPortRealSignal(S, 0);
    p->Outputs(y);
    UNUSED_ARG(tid);
}

#ifdef MATLAB_MEX_FILE
/* For now mdlG[S]etSimState are only supported in normal simulation */

/* Define to indicate that this S-Function has the mdlG[S]etOperatingPoint mothods */
#undef MDL_OPERATING_POINT
#endif

/* Function: mdlTerminate =====================================================
 * Abstract:
 *    In this function, you should perform any actions that are necessary
 *    at the termination of a simulation.  For example, if memory was
 *    allocated in mdlStart, this is the place to free it.
 */
static void mdlTerminate(SimStruct *S)
{
    auto p = static_cast<BCI2000OnlineLink*>(ssGetPWorkValue(S, 0));
    delete p;
}
/*======================================================*
 * See sfuntmpl.doc for the optional S-function methods *
 *======================================================*/

/*=============================*
 * Required S-function trailer *
 *=============================*/

#ifdef  MATLAB_MEX_FILE    /* Is this file being compiled as a MEX-file? */
#include "simulink.c"      /* MEX-file interface mechanism */
#else
#include "cg_sfun.h"       /* Code generation registration function */
#endif

