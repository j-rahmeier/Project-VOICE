////////////////////////////////////////////////////////////////////////////////
// Authors: Will Engelhardt (engelhardt@neurotechcenter.org)
// Description: PACFilter implementation
////////////////////////////////////////////////////////////////////////////////

#include "PACFilter.h"
#include "BCIStream.h"
using namespace std;

constexpr int MAXIMUM_ANGLE = 360;

RegisterFilter( PACFilter, 2.B );


PACFilter::PACFilter()
  : mDelayCounter(0),
  p(nullptr)
  //mMutableDecimation(false)
{
}
PACFilter::Private::Private(int dec, int& counter, int winSize, int binSize, int _nChannels, bool dev)
  : PI(2 * acos(0.0)),
  mDecimation(dec),
  mBlockCounter(counter),
  mWindowSize(winSize),
  nBins(binSize),
  nChannels(_nChannels),
  mIncludeDeviation(dev)
{
}

PACFilter::~PACFilter()
{
  Halt();
}

void
PACFilter::Publish()
{
 BEGIN_PARAMETER_DEFINITIONS

   "Filtering:PACFilter int AngleDecimation= auto auto 1 90 // Number of degrees of resolution (e.g., 1 = every 1 degree)",
   "Filtering:PACFilter float SlidingWindow= 0 0 0 % // Amount of time to hold past data for",
   "Filtering:PACFilter int IncludeDeviation= 0 0 0 1 // Enable to pass standard deviation as 2nd half of output (boolean)",

 END_PARAMETER_DEFINITIONS

 BEGIN_STATE_DEFINITIONS

   "PAC_TrialNumber       16 0 0 0",
   "PAC_TrialType         8 0 0 0",

 END_STATE_DEFINITIONS
}
void
PACFilter::AutoConfig(const SignalProperties& Input)
{
  Parameter("AngleDecimation") = 10;
}

void
PACFilter::Preflight( const SignalProperties& Input, SignalProperties& Output ) const
{
  Output = Input;

  //assumes filter combination gives both filters same channels
  Output.SetChannels(Input.Channels() / 2);
  //output one z value for each phase
  Output.SetElements(MAXIMUM_ANGLE / Parameter("AngleDecimation").ToNumber() * (Parameter("IncludeDeviation") + 1) ); //amplitude, deviation
  //Output.SetElements(Input.Elements());

  float delay = Parameter("Amplitude/HilbertFilter/Delay");
  float delay2 = Parameter("Phase/HilbertFilter/Delay");
  if (delay != delay2)
    bcierr << "The two Hilbert Filters have mismatched Delay parameters. Please make them the same. " << endl;

  if (Parameter("Phase/HilbertFilter/OutputSignal") != 2)
    bcierr << "The Phase Filter must have the Hilbert Filter output phase. Change the OutputSignal parameter" << endl;
  if (Parameter("Amplitude/HilbertFilter/OutputSignal") != 1)
    bcierr << "The Amplitude Filter must have the Hilbert Filter output amplitude. Change the OutputSignal parameter" << endl;

  Parameter("SlidingWindow");
  State("PAC_TrialNumber");
  State("PAC_TrialType");

}

void
PACFilter::Initialize(const SignalProperties& Input, const SignalProperties& Output)
{
  int dec = Parameter("AngleDecimation").ToNumber();
  int winSize = floor(Parameter("SlidingWindow").InSampleBlocks());
  bool includeDev = Parameter("IncludeDeviation");
  int binSize = Output.Elements() / (includeDev + 1);
  int nChannels = Output.Channels();

  double intpart;
  if (modf((double)MAXIMUM_ANGLE / (double)dec, &intpart) != 0.0)
  {
    dec = MAXIMUM_ANGLE / intpart;
    Parameter("AngleDecimation") = dec;
    bciout << "PAC AngleDecimation does not create evenly spaced bins. Changing to " << MAXIMUM_ANGLE / intpart << endl;
  }

  //two delays are the same, just take one
  int offset = static_cast<int>(0.5 + Input.Elements()
                * Parameter("Amplitude/HilbertFilter/Delay").InSampleBlocks());
  mDelay = 2 * offset + 1;

  if (p == nullptr)
  {
    delete p;
  }
  p = new Private(dec, mBlockCounter, winSize, binSize, nChannels, includeDev);
  p->Initialize();
}
void
PACFilter::Private::Initialize()
{
  ChannelData d(*this, nBins, mWindowSize); //don't include deviation
  mAllChannels.clear();
  for (int ch = 0; ch < nChannels; ch++)
  {
    mAllChannels.push_back(d); //create copy for each channel
  }
}

void
PACFilter::StartRun()
{
  mDelayCounter = 0;
  //mDecSet = false;
  overallMaxValue = 0;
  overallMinValue = INFINITY;
  mBlockCounter = 0; 
  mTrialNumber = 1;
  State("PAC_TrialNumber") = mTrialNumber;

  p->StartRun();
}
void
PACFilter::Private::StartRun()
{
  for (auto ch = mAllChannels.begin(); ch != mAllChannels.end(); ch++)
    ch->clearData();
}

void
PACFilter::Process( const GenericSignal& Input, GenericSignal& Output )
{
  //to keep track of data for sliding window
  mBlockCounter++;

  //wait for Hilbert Filter output
  if (mDelayCounter < mDelay)
  {
    //fill output with zeros
    for (int ch = 0; ch < Output.Channels(); ch++)
    {
      for (int el = 0; el < Output.Elements(); el++)
      {
        Output(ch, el) = 0;
      }
    }
    //update counter
    mDelayCounter += Input.Elements();
    return;
  }

  //check if we need to reset all channels
  bool resetTrial = State("PAC_TrialNumber") != mTrialNumber;
  if (resetTrial)
  {
    mTrialNumber++;
    State("PAC_TrialNumber") = mTrialNumber;
  }
  //get new max value across all channels for every block
  /*overallMaxValue = 0;
  overallMinValue = INFINITY;
  for (int ch = 0; ch < Output.Channels(); ch++)
  {
    ChannelData* channel = &mAllChannels[ch];
    channel->calculateMaxValue(overallMaxValue);
    channel->calculateMinValue(overallMinValue);
  }*/
  //bciout << Output.Channels() << endl;
  Output = p->Process(Input, Output, resetTrial);
}
void
PACFilter::Private::ProcessChannel(const GenericSignal& Input, int ch2)
{
  int phaseCh = ch2;
  int ampCh = ch2 + nChannels;
  //bciout << ch2 << endl;
  for (int el = 0; el < Input.Elements(); el++)
  {

    //round to nearest int 0-359 (MAXIMUM_ANGLE)
    int angle = floor(Input(phaseCh, el) * MAXIMUM_ANGLE / (2 * PI) + MAXIMUM_ANGLE / 2);
    //round according to decimation
    angle = (angle / mDecimation); //int division

    double amplitude = Input(ampCh, el);
    mAllChannels[ch2].addAmplitude(angle, amplitude, mBlockCounter);
  }
  return;
}
GenericSignal&
PACFilter::Private::Process(const GenericSignal& Input, GenericSignal& Output, bool resetTrial)
{
  //vector<future<void>> future;
  ////bciout << Output.Channels() << endl;
  //for (int ch1 = 0; ch1 < Output.Channels(); ++ch1)
  //{
  //  //auto result = std::async([&]() { return ProcessChannel(8); });
  //  future.push_back(std::async([&, ch1]() { return ProcessChannel(Input, ch1); }));
  //}
  //for (int ch = 0; ch < Output.Channels(); ch++)
  //{
  //  future[ch].get();
  //   //int s = future[ch].get();
  //   //bciout << s << endl;
  //}
  //order data points by angle 
  //first half of input signal is phase, second is amplitude
  for (int ch = 0; ch < Output.Channels(); ch++)
  {
    ChannelData* channel = &mAllChannels[ch];
    int phaseCh = ch;
    int ampCh = ch + nChannels;

    //reset data if new trial
    if (resetTrial)
    {
      channel->clearData();
    }

    for (int el = 0; el < Input.Elements(); el++)
    {
      //round to nearest int 0-359 (MAXIMUM_ANGLE)
      int angle = floor(Input(phaseCh, el) * MAXIMUM_ANGLE / (2 * PI) + MAXIMUM_ANGLE / 2);
      //round according to decimation
      angle = (angle / mDecimation); //int division

      double amplitude = Input(ampCh, el);
      channel->addAmplitude(angle, amplitude, mBlockCounter);
    }
    //calculate std dev and mean for z-score
    double mean = 0;
    for (int ang = 0; ang < nBins; ang++)
    {
      mean += channel->getAmplitude(ang, mBlockCounter);
    }
    mean /= nBins;
    double s = 0;
    for (int el = 0; el < Output.Elements() / ((int)mIncludeDeviation + 1); el++)
    {
      s += pow((channel->getAmplitude(el, mBlockCounter) - mean), 2);
    }
    s = sqrt(s / (nBins - 1));

    //amplitude is first half, deviation is second half (maybe)
    for (int el = 0; el < Output.Elements() / ((int)mIncludeDeviation + 1); el++)
    {
      //Output(ch, el) = dataMap->at(el);
      double elAmp = channel->getAmplitude(el, mBlockCounter);
      Output(ch, el) = (elAmp - mean) / s;

      if (mIncludeDeviation)
        Output(ch, el + Output.Elements() / 2) = sqrt(channel->getAmplitudeSquared(el) - elAmp * elAmp); //calc deviation of amplitudes from mean
    }
  }
  return Output;
}

void
PACFilter::StopRun()
{
}

void
PACFilter::Halt()
{
  if (p != nullptr) {
    delete p;
    p = nullptr;
  }
}

//Channel Data Class functions
PACFilter::Private::ChannelData::ChannelData(Private& p, int elementSize, int& windowSize)
  : mWindowSize(windowSize),
  parent(p)
{
  init(elementSize);
}
void
PACFilter::Private::ChannelData::init(int elementSize)
{
  mAngles.clear();
  vector<Angle> defaultVec(elementSize, { 0, 0 });
  mAngles = defaultVec;

  mAllData.clear();
  deque<DataPoint> defaultQ;
  vector<deque<DataPoint>> dV(elementSize, defaultQ);
  mAllData = dV;
}

void
PACFilter::Private::ChannelData::addAmplitude(int& angle, double& amplitude, int& counter)
{
  Angle* angleData = &mAngles[angle];
  if (mWindowSize == 0)
  {
    //averaging, weight past values to give new value an accurate representation
    double newAverage = (angleData->numPastElements * angleData->pastAverage + amplitude) / (1 + angleData->numPastElements);
    double newAverageSquare = (angleData->numPastElements * angleData->pastAverage + (amplitude * amplitude)) / (1 + angleData->numPastElements);
    angleData->numPastElements++;
    angleData->pastAverage = newAverage;
    angleData->pastAverageSquare = newAverageSquare;
  }
  else
  {
    double sumSq = amplitude * amplitude;
    angleData->sum += amplitude;
    angleData->sumSquared += sumSq;
    mAllData[angle].push_back({ amplitude, sumSq, counter });
  }
}

void
PACFilter::Private::ChannelData::calculateMaxValue(double& maxValue)
{
  double thisChannelMax = (*max_element(mAngles.begin(), mAngles.end())).pastAverage;
  maxValue = max(maxValue, thisChannelMax);
}
void
PACFilter::Private::ChannelData::calculateMinValue(double& minValue)
{
  double thisChannelMin = (*min_element(mAngles.begin(), mAngles.end())).pastAverage;
  minValue = min(minValue, thisChannelMin);
}

double
PACFilter::Private::ChannelData::getAmplitude(int& angle, int& thisBlock)
{
  //easy if averaging
  if (mWindowSize == 0)
  {
    return mAngles[angle].pastAverage;
  }

  //if sliding window, get rid of old values
  if (mAllData[angle].empty())
    return 0;
  DataPoint* firstEl = &mAllData[angle].front();
  //pop all elements that are too old
  while (thisBlock - firstEl->blockCounter > mWindowSize)
  {
    mAngles[angle].sum -= firstEl->amplitude; //don't count it in average
    mAngles[angle].sumSquared -= firstEl->squared;
    mAllData[angle].pop_front();
    if (mAllData[angle].empty())
      return 0;
    firstEl = &mAllData[angle].front(); //next element
  }

  return mAngles[angle].sum / mAllData[angle].size();
  //return mAngles[angle].pastAverage;
}

double
PACFilter::Private::ChannelData::getAmplitudeSquared(int& angle)
{
  if (mWindowSize == 0)
    return mAngles[angle].pastAverageSquare;
  else
    return mAngles[angle].sumSquared / mAllData[angle].size();
}

void
PACFilter::Private::ChannelData::clearData()
{
  //keep size while deleting elements
  int elSize = mAngles.size();
  init(elSize);
}