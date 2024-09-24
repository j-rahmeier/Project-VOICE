////////////////////////////////////////////////////////////////////////////////
// Authors: Brunnerlab@DESKTOP-F8KRI7F.wucon.wustl.edu
// Description: CCEPFilter header
////////////////////////////////////////////////////////////////////////////////

#ifndef INCLUDED_CCEPFilter_H  // makes sure this header is not included more than once
#define INCLUDED_CCEPFilter_H

#include "GenericFilter.h"
#include "Expression.h"
#include "DeviceConfiguration.h"
#include <thread>
#include <queue>
#include <vector>

class CCEPFilter : public GenericFilter
{
 public:
  CCEPFilter();
  ~CCEPFilter();
  void Publish() override;
  void AutoConfig(const SignalProperties& Input) override;
  void Preflight( const SignalProperties& Input, SignalProperties& Output ) const override;
  void Initialize( const SignalProperties& Input, const SignalProperties& Output ) override;
  void StartRun() override;
  void Process( const GenericSignal& Input, GenericSignal& Output ) override;
  void StopRun() override;
  void Halt() override;

 private:
   double FilterCaptionToValue(const char* inCaption) const;
   //copied from P3TemporalFilter
   
   //GenericSignal mVisSignal;
   //GenericVisualization mVis2;
   bool mVisualize;
   int mNumChannels;
   SignalProperties mOutputProperties, mEpochProperties, mBaselineProperties;
   int mEpochsToAverage, mNumberOfSequences;
   bool mSingleEpochMode;
   int mLastStimulusCode;
   Expression mOnsetExpression;
   bool mPreviousExpressionValue;
   GenericSignal mAllData;
   int mBaselineSamples;

   //visualization
   struct Private;
   Private* p;

   //new
   StimulatorConfiguration* mStimulator;
   int mOnsetCounter;
   int mOnsetPeriod;

   //classes from P3TemporalFilter
   class ReverseBuffer //should have same capabilities as EpochBuffer, but prioritizes front
   {
   public:
     ReverseBuffer(const SignalProperties & inProperties)
       : mQueue(), mCursor(0), mFinalData(inProperties), mOffset(0), mFinalized(false)
     {}
     //copy constructor
     /*ReverseBuffer(const ReverseBuffer& og)
     {
       mCursor = og.mCursor;
       mFinalData = og.mFinalData;
       mOffset = 0;
       mFinalized = og.mFinalized;
       auto newQ = new std::queue<double*>();
       for (int i = 0; i < newQ->size(); i++)
       {
         double* otherData = newQ->front();
         double* data = new double[mFinalData.Channels()];
         for (int ch = 0; ch < mFinalData.Channels(); ++ch)
           data[ch] = otherData[ch];
         mQueue.push(data);
         newQ->pop();
       }
     }*/
     const GenericSignal& PopData()
     {
       for (int sm = 0; sm < mFinalData.Elements(); ++sm)
       {
         if (!mQueue.empty())
         {
           double* data = mQueue.front();
           for (int ch = 0; ch < mFinalData.Channels(); ++ch)
             mFinalData(ch, sm) = data[ch];
           mQueue.pop();
         }
         else
         {
           bciout << "Not enough time to fill baseline buffer" << std::endl;
           break;
         }
       }
       mCursor = 0;
       mFinalized = false;
       return mFinalData;
     }
     int EpochDone() const
     {
       return mCursor >= mFinalData.Elements();
     }
     ReverseBuffer& Finalize(const GenericSignal& s, int offset)
     {
       //bciwarn << "Finalizing!" << std::endl;
       mOffset = s.Elements() - offset;
       Process(s);
       mFinalized = true;
       return *this;
     }
     ReverseBuffer& Process(const GenericSignal& s)
     {
       if (!mFinalized)
       {
         //first copy what we need
         int samplesToCopy = s.Elements() - mOffset;
         for (int sm = 0; sm < samplesToCopy; ++sm)
         {
           double* data = new double[s.Channels()];
           for (int ch = 0; ch < s.Channels(); ++ch)
             data[ch] = s(ch, sm);
           mQueue.push(data);
         }
         //then check what we need to get rid of
         int sampleOverflow = samplesToCopy - (mFinalData.Elements() - mCursor);
         mCursor += samplesToCopy;
         mOffset = 0;
         if (sampleOverflow > 0)
         {
           int numToPop = std::min(sampleOverflow, mCursor);
           for (int i = 0; i < numToPop; i++)
           {
             mQueue.pop();
           }
           mCursor -= numToPop;
         }
         //bciout << "Sample size: " << mQueue.size() << std::endl;
       }
       return *this;
     }
   private:
     GenericSignal mFinalData;
     int mCursor, mOffset;
     std::queue<double*> mQueue;
     bool mFinalized;
   };
   class EpochBuffer
   {
   public:
     EpochBuffer(const SignalProperties & inProperties, int offset)
       : mData(inProperties), mCursor(0), mOffset(offset)
     {
     }
     ~EpochBuffer()
     {
     }
     const GenericSignal& Data() const
     {
       return mData;
     }
     int EpochDone() const
     {
       return mCursor >= mData.Elements();
     }
     EpochBuffer& Process(const GenericSignal & s)
     {
       int samplesToCopy = std::min(mData.Elements() - mCursor, s.Elements() - mOffset);
       if (samplesToCopy > 0)
       {
         for (int ch = 0; ch < s.Channels(); ++ch)
           for (int sm = 0; sm < samplesToCopy; ++sm)
             mData(ch, mCursor + sm) = s(ch, mOffset + sm);
         mCursor += samplesToCopy;
         mOffset = 0;
       }
       return *this;
     }
     EpochBuffer& ProcessPrioritizeNew(const GenericSignal& s)
     {
       //first check if we have enough room for whole new signal
       int sampleOverflow = (s.Elements() - mOffset) - (mData.Elements() - mCursor);
       bciout << "Sample overflow: " << sampleOverflow << std::endl;
       if (sampleOverflow > 0) //can't copy whole signal
       {
         if (mCursor - sampleOverflow < 0)
         {
           bciwarn << "Not enough elements to shift. mCursor: " << mCursor << std::endl;

           SignalProperties sp = s.Properties();
           sp.SetElements(mData.Elements());
           GenericSignal sig = GenericSignal();
           sig.SetProperties(sp);

           //have to set it with correct values
           for (int ch = 0; ch < mData.Channels(); ++ch)
             for (int sm = 0; sm < mData.Elements(); ++sm)
               sig(ch, sm) = s(ch, sm + s.Elements() - mData.Elements());
           return Process(sig);
         }
         Shift(sampleOverflow);
       }
       return Process(s);
     }
     void Shift(int shift)
     {
        //need to write-over beginning to make room for new block
        for (int ch = 0; ch < mData.Channels(); ++ch)
          for (int sm = 0; sm < mData.Elements() - shift; ++sm)
            mData(ch, sm) = mData(ch, sm + shift); //this is inefficient way to constantly do this
        mCursor -= shift;
     }
     /*void Clear()
     {
       mCursor = 0;
     }*/

   private:
     GenericSignal mData;
     int mCursor, mOffset;
   };
   //EpochBuffer* mBaselineBuffer;
   ReverseBuffer* mBaselineBuffer;

   class EpochSet : public std::set<EpochBuffer*>
   {
   public:
     EpochSet()
     {
     }
     ~EpochSet()
     {
       clear();
     }
     void clear()
     {
       for (iterator i = begin(); i != end(); ++i)
         delete* i;
       std::set<EpochBuffer*>::clear();
     }
   };
   EpochSet mEpochs;
   class ReverseEpochSet : public std::set<ReverseBuffer*>
   {
   public:
     ReverseEpochSet()
     {
     }
     ~ReverseEpochSet()
     {
       clear();
     }
     void clear()
     {
       for (iterator i = begin(); i != end(); ++i)
         delete* i;
       std::set<ReverseBuffer*>::clear();
     }
   };
   ReverseEpochSet mBaselineEpochs;

   typedef std::map<EpochBuffer*, ReverseBuffer*> EpochMap;
   EpochMap mEpochMap;

   class DataSum : public GenericSignal
   {
   public:
     DataSum(const SignalProperties& s) : GenericSignal(s), mCount(0)
     {
     }
     int Count() const
     {
       return mCount;
     }
     DataSum& Add(const GenericSignal& s)
     {
       for (int i = 0; i < s.Channels(); ++i)
         for (int j = 0; j < s.Elements(); ++j)
           (*this)(i, j) += s(i, j);
       ++mCount;
       return *this;
     }

   private:
     int mCount;
   };
   DataSum* mEpochSums; 
   DataSum* mBaselineSums;
};

#endif // INCLUDED_CCEPFilter_H
