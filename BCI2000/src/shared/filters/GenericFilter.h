////////////////////////////////////////////////////////////////////////////////
// $Id: GenericFilter.h 8296 2024-08-01 16:12:02Z mellinger $
// Description: This file declares a purely abstract GenericFilter interface
//   which all BCI2000 filters are supposed to implement.
//
// $BEGIN_BCI2000_LICENSE$
//
// This file is part of BCI2000, a platform for real-time bio-signal research.
// [ Copyright (C) 2000-2023: BCI2000 team and many external contributors ]
//
// BCI2000 is free software: you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the Free Software
// Foundation, either version 3 of the License, or (at your option) any later
// version.
//
// BCI2000 is distributed in the hope that it will be useful, but
//                         WITHOUT ANY WARRANTY
// - without even the implied warranty of MERCHANTABILITY or FITNESS FOR
// A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along with
// this program.  If not, see <http://www.gnu.org/licenses/>.
//
// $END_BCI2000_LICENSE$
////////////////////////////////////////////////////////////////////////////////
#ifndef GENERIC_FILTER_H
#define GENERIC_FILTER_H

#include "Directory.h"
#include "Environment.h"
#include "GenericVisualization.h"
#include "Uncopyable.h"
#include <vector>
#include <list>
#include <map>
#include <set>
#include <typeinfo>
// #includes needed for every filter, so they are put here for
// convenience.
#include "BCIRegistry.h"
#include "BCIStream.h"
#include "GenericSignal.h"

class GenericFilter : public Directory::Node, public Environment, private Uncopyable
{
  protected:
    GenericFilter();

  public:
    virtual ~GenericFilter();

  protected:
    // Traditionally, Parameters and States are announced from the Filter's
    // constructor. For greater flexibility, we are migrating towards a separate
    // Publish() method, but will maintain compatibility with existing code.
    virtual void Publish()
    {
    }
    // AutoConfig() allows filters to update parameter values according to hardware
    // settings, or input properties.
    // AutoConfig() may access parameters arbitrarily, but changes to parameters
    // are ignored unless a parameter's value is set to "auto".
    // Also, AutoConfig() changes are ignored if the AutoConfig parameter is
    // nonexistent, or set to 0, but the function is called nevertheless, so it may
    // be used for hardware initialization which is independent of parameter settings.
    virtual void AutoConfig(const SignalProperties &Input)
    {
    }
    // Preflight will always check
    virtual void Preflight(const SignalProperties &Input, SignalProperties &Output) const = 0;
    // Initialize() performs initialization required when parameter settings
    // have changed.
    virtual void Initialize(const SignalProperties &Input, const SignalProperties &Output) = 0;
    // StartRun() performs initialization required when starting a new run.
    virtual void StartRun()
    {
    }
    // StopRun() performs actions required when a run ends.
    virtual void StopRun()
    {
    }

    virtual void Process(const GenericSignal &Input, GenericSignal &Output) = 0;
    virtual void Resting(const GenericSignal &Input, GenericSignal &Output)
    {
    }
    virtual void Resting()
    {
    }
    virtual void Halt()
    {
    }

    // Override this to always enable/disable timing measurement for Process() calls.
    virtual bool TimedCalls() const;

  public:
    // This function is called once at instantiation, and after each Initialize().
    //  If it returns false at instantiation, no visualization parameter is created for the filter.
    //  If it returns false at initialization time, no default visualization is displayed,
    //  independently of the visualization parameter's value.
    //  (The filter itself may still maintain its own visualization object(s).)
    virtual bool AllowsVisualization() const
    {
        return true;
    }

  public:
    struct PerformanceData
    {
        int count;
        double totalDuration;
        double maxDuration;
        double minDuration;
        double blockDuration;
    };
    const PerformanceData &PerformanceData() const;
    void EnableProfiling(bool);

  private:
    static void SetAuxThreadData(void *);

    struct Private;
    Private *p;

  public: // Calling interface to virtual functions -- allows for setting up context.
    void CallPublish();
    void CallAutoConfig(const SignalProperties &Input);
    void CallPreflight(const SignalProperties &Input, SignalProperties &Output) const;
    void CallInitialize(const SignalProperties &Input, const SignalProperties &Output);
    void CallStartRun();
    void CallStopRun();
    void CallProcess(const GenericSignal &Input, GenericSignal &Output);
    void CallResting(const GenericSignal &Input, GenericSignal &Output);
    void CallResting();
    void CallHalt();

  public:
    // The following elements provide means to make the existence of a filter
    // class known to the framework at runtime, without the need to make changes
    // in framework code.
    // A registrar is an auto-created class known to the framework code
    // that allows for instantiation of its associated class without knowledge
    // of the associated class' name etc.
    // A separate Registrar class per filter -- as opposed to a static instance
    // of each filter in the filter's cpp file -- is necessary because
    // - the standard puts functionality inside the GenericFilter constructor
    //   that requires more than is granted at initialization time, and
    // - we don't want to restrict ourselves to a single instance per filter.
    //
    // To use the mechanism, all that is required is a line that says
    //   RegisterFilter( MyFilterClass, FilterPosition );
    // somewhere in the filter's cpp file where FilterPosition is a
    // symbol that, by string comparison, will determine the filter's
    // position in the filter sequence.
  public:
    class Registrar
    {
      public:
        Registrar(bool inAutoDelete = true);
        Registrar(const char *inPos, int inPriority, bool inAutoDelete);
        virtual ~Registrar();
        const std::string &Position() const
        {
            return mPos;
        }
        virtual const std::type_info &Typeid() const = 0;
        virtual GenericFilter *NewInstance() const = 0;
        static void Clear();

      public:
        struct less
        {
            bool operator()(const Registrar *a, const Registrar *b) const;
        };
        friend struct less;

        typedef std::set<Registrar *, Registrar::less> RegistrarSet_;
        static RegistrarSet_ &Registrars();

      private:
        std::string mPos;
        int mPriority;
        size_t mInstance;
        static size_t sCreatedInstances;

        struct AutoDeleteSet : public std::set<Registrar *>
        {
            ~AutoDeleteSet();
            void Clear();
        };
        static AutoDeleteSet &AutoDeleteInstance();
    };
    typedef Registrar::RegistrarSet_ RegistrarSet;

  public:
    // Register a filter for automatic instantiation.
    template <typename T> class FilterRegistrar : public Registrar
    {
      public:
        FilterRegistrar(Directory::Node *pNode) : mpNode(pNode)
        {
        }
        FilterRegistrar(const char *inPos, int inPriority, bool inAutoDelete = false)
            : Registrar(inPos, inPriority, inAutoDelete), mpNode(GenericFilter::Directory())
        {
        }
        Directory::Node *node() const
        {
            return mpNode;
        }
        virtual const std::type_info &Typeid() const
        {
            return typeid(T);
        }
        virtual GenericFilter *NewInstance() const
        {
            GenericFilter::SetAuxThreadData(mpNode);
            auto t = new T;
            GenericFilter::SetAuxThreadData(nullptr);
            return t;
        }

      private:
        Directory::Node *mpNode;
    };

    // Get available filters' position strings.
    static const std::string &GetFirstFilterPosition();
    static const std::string &GetLastFilterPosition();

    // This container holds all instantiated filters.
    typedef std::list<GenericFilter *> FiltersType;
    static FiltersType &AllFilters();

    // Get info about a filter chain.
    struct ChainEntry
    {
        EncodedString position, name;
    };
    class ChainInfo : public std::vector<ChainEntry>
    {
      public:
        std::ostream &InsertInto(std::ostream &);
        std::istream &ExtractFrom(std::istream &);
    };
    class Chain
    {
      public:
        Chain()
        {
        }
        Chain(const Registrar::RegistrarSet_ &);
        void Add(Registrar *);
        void ClearRegistrars();

        ChainInfo Info();
        // Instantiate all registered filters once.
        void Instantiate();
        // Dispose of all filter instances not passed to another filter with PassFilter<>().
        void Dispose();
        // Apply the single filter functions to all filters instantiated and not passed
        // to another filter with PassFilter<>().
        void OnPublish();
        void OnPreflight(const SignalProperties &Input, SignalProperties &Output);
        void OnInitialize();
        void OnStartRun();
        void OnProcess(const GenericSignal &Input, GenericSignal &Output, bool inResting = false);
        void OnStopRun();
        void OnResting();
        void OnHalt();

        // Get the first filter instance of a given type, e.g.:
        // MyFilter* myFilter = GenericFilter::GetFilter<MyFilter>();
        template <typename T> T *GetFilter()
        {
            T *filterFound = NULL;
            FiltersType::iterator i = mOwnedFilters.begin();
            while (i != mOwnedFilters.end() && filterFound == NULL)
            {
                filterFound = dynamic_cast<T *>(*i);
                ++i;
            }
            return filterFound;
        }
        // Get the first filter instance of a given type, and remove it
        // from the owned filters list. This is meant for building a hierarchy of
        // GenericFilter instances.
        template <typename T> T *PassFilter()
        {
            T *filter = GetFilter<T>();
            mOwnedFilters.remove(filter);
            return filter;
        }

      private:
        // These are filters managed by the GenericFilter class:
        // Instantiation, Disposal, and application of filter functions.
        FiltersType mOwnedFilters;

        typedef std::map<GenericFilter *, GenericSignal> SignalsType;
        SignalsType mOwnedSignals;

        Registrar::RegistrarSet_ mRegistrars;
    
    public:
        // Filter hooks are called on behalf of the filter given as an argument.
        // With the exception of OnDispose(), the hook function is called _after_ the filter has been called for processing.
        // Default visualization and signal sharing are implemented as filter hooks.
        struct FilterHook
        {
            virtual ~FilterHook() {}
            virtual void OnInstantiate(GenericFilter*, const std::string& position) {}
            virtual void OnDispose(GenericFilter*) {}
            virtual void OnPublish(GenericFilter*) {}
            virtual void OnPreflight(GenericFilter*, const SignalProperties& Input, const SignalProperties& Output) {}
            virtual void OnInitialize(GenericFilter*, const SignalProperties& Input, const SignalProperties& Output) {}
            virtual void OnProcess(GenericFilter*, const GenericSignal& Input, const GenericSignal& Output) {}
            virtual void OnResting(GenericFilter*, const GenericSignal& Input, const GenericSignal& Output) {}
            virtual void OnResting(GenericFilter*) {}
            virtual void OnStartRun(GenericFilter*) {}
            virtual void OnStopRun(GenericFilter*) {}
            virtual void OnHalt(GenericFilter*) {}
        };
        static void AddFilterHook(FilterHook*);
        static void RemoveFilterHook(FilterHook*);

      private:
        static std::vector<FilterHook*> sFilterHooks;
    };
    static Chain &RootChain();
    static Directory::Node *Directory();

    static ChainInfo GetChainInfo()
    {
        return RootChain().Info();
    }

    // Instantiate all registered filters once.
    static void InstantiateFilters()
    {
        RootChain().Instantiate();
    }
    // Dispose of all filter instances not passed to another filter with PassFilter<>().
    static void DisposeFilters()
    {
        RootChain().Dispose();
    }
    // Apply the single filter functions to all filters instantiated and not passed
    // to another filter with PassFilter<>().
    static void PublishFilters()
    {
        RootChain().OnPublish();
    }
    static void PreflightFilters(const SignalProperties &Input, SignalProperties &Output)
    {
        RootChain().OnPreflight(Input, Output);
    }
    static void InitializeFilters()
    {
        RootChain().OnInitialize();
    }
    static void StartRunFilters()
    {
        RootChain().OnStartRun();
    }
    static void ProcessFilters(const GenericSignal &Input, GenericSignal &Output, bool resting = false)
    {
        RootChain().OnProcess(Input, Output, resting);
    }
    static void StopRunFilters()
    {
        RootChain().OnStopRun();
    }
    static void RestingFilters()
    {
        RootChain().OnResting();
    }
    static void HaltFilters()
    {
        RootChain().OnHalt();
    }

    // Create a new instance of the same type as the argument pointer.
    static GenericFilter *NewInstance(const GenericFilter *);
    template <typename T> static T *GetFilter()
    {
        return RootChain().GetFilter<T>();
    }
    // Get the first filter instance of a given type, and remove it
    // from the owned filters list. This is meant for building a hierarchy of
    // GenericFilter instances.
    template <typename T> static T *PassFilter()
    {
        return RootChain().PassFilter<T>();
    }
};

#endif // GENERIC_FILTER_H
