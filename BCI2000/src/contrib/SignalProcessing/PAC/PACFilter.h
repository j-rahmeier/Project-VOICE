////////////////////////////////////////////////////////////////////////////////
// Authors: Will Engelhardt (engelhardt@neurotechcenter.org)
// Description: PACFilter header
////////////////////////////////////////////////////////////////////////////////

#ifndef INCLUDED_PACFILTER_H  // makes sure this header is not included more than once
#define INCLUDED_PACFILTER_H

#include "GenericFilter.h"
#include "FilterCombination.h"
#include "HilbertFilter.h"
#include <deque>
#include <future>

class PACFilter : public GenericFilter
{
 public:
  PACFilter();
  ~PACFilter();
  void Publish() override;
  void AutoConfig(const SignalProperties& Input) override;
  void Preflight( const SignalProperties& Input, SignalProperties& Output ) const override;
  void Initialize( const SignalProperties& Input, const SignalProperties& Output ) override;
  void StartRun() override;
  void Process( const GenericSignal& Input, GenericSignal& Output ) override;
  void StopRun() override;
  void Halt() override;

 private:
   // Use this space to declare any PACFilter-specific methods and member variables you'll need
  // QPolarChart* chart;
  // QChartView* chartView;
  // PACDemoWidget* m_ui;
   class Private
   {
   public:
     Private(int, int&, int, int, int, bool);
     void Initialize();
     void StartRun();
     GenericSignal& Process(const GenericSignal& Input, GenericSignal& Output, bool);
   private:
     void ProcessChannel(const GenericSignal&, int);
     const double PI;
     int mDecimation;
     int& mBlockCounter;
     int mWindowSize;
     int nChannels;
     int nBins;
     bool mIncludeDeviation;

     class ChannelData
     {
     public:
       ChannelData(Private&, int, int&);
       void   addAmplitude(int& angle, double& amplitude, int& counter);
       void   calculateMinValue(double& maxValue);
       void   calculateMaxValue(double& maxValue);
       double getAmplitude(int& angle, int& thisBlock);
       double getAmplitudeSquared(int& angle);
       void   clearData();
     private:
       Private& parent;
       void   init(int elementSize);
       struct Angle
       {
         double sum, sumSquared;
         //for averaging
         uint64_t numPastElements = 0;
         double pastAverage, pastAverageSquare;
         //define for calculating max
         bool operator <(const Angle& b) const {
           return pastAverage < b.pastAverage;
         }
       };
       std::vector<Angle> mAngles;
       struct DataPoint
       {
         double amplitude;
         double squared;
         int blockCounter;
       };
       std::vector<std::deque<DataPoint>> mAllData;
       int mWindowSize;
     };
     std::vector<ChannelData> mAllChannels;
   };
   Private* p;
   int mDelay, mDelayCounter;
   std::list<double> outputAmplitudes;
   double overallMaxValue, overallMinValue;
   bool mAverageSignal;
   int mBlockCounter;
   int mTrialNumber;
};

#endif // INCLUDED_PACFILTER_H
