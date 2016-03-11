/******************************************************************************/
/*!
 * \author Austin Morris
 * \date   2015-04-15
 *
 * \copyright All content 2015 DigiPen (USA) Corporation, all rights reserved.
 */
/******************************************************************************/
#include "Events.h"
#include "Core/Component.h"
#include "Core/Object.h"
#include "StandardEvents.h"

namespace YTE
{
  namespace Event
  {
    ZilchDefineType(Event, "Event", YTELibrary, builder, type)
    {
      ZilchBindConstructor(builder, type, YTE::Event::Event, ZilchNoNames);
      type->HandleManager = ZilchManagerId(Zilch::PointerManager);
      ZilchBindDestructor(builder, type, YTE::Event::Event);
    }

    BaseEventHandler::BaseEventHandler() : mActiveInvokeLoops(0)
    {
    }

    void RegisterListener(Zilch::Call& call, Zilch::ExceptionReport& report)
    {
      Zilch::String eventName = call.Get<Zilch::String>(0);
      Zilch::Delegate& func = call.GetDelegate(1);

      auto sender = reinterpret_cast<YTE::Event::BaseEventHandler*>(
        call.GetHandle(Zilch::Call::This).Dereference());

      auto receiver = reinterpret_cast<YTE::Event::BaseEventHandler*>(
        func.ThisHandle.Dereference());

      auto callback = new ZilchEventCallback();
      callback->state = call.GetState();
      callback->func = func;
      sender->RegisterListener(eventName.c_str(), callback, *receiver);
    }

    ZilchDefineType(BaseEventHandler, "BaseEventHandler",
      YTELibrary, builder, type)
    {
      type->HandleManager = ZilchManagerId(Zilch::PointerManager);
    
      ZilchBindConstructor(builder, type, BaseEventHandler, nullptr);
      ZilchBindMethod(builder, type, &BaseEventHandler::Trigger,
        ZilchNoOverload, "Trigger", nullptr);
      // TODO@@@ (Austin): Rename it correctly in Zilch and fix any breaks
      ZilchBindMethod(builder, type, &BaseEventHandler::StopListening,
        ZilchNoOverload, "Deregister", nullptr);

      Zilch::ParameterArray parameters;
      parameters.push_back(ZilchTypeId(Zilch::String));
      parameters.push_back(Zilch::Core::GetInstance().AnyDelegateType);

    
      builder.AddBoundFunction(type, 
                               "RegisterListener",
                               YTE::Event::RegisterListener,
                               parameters,
                               ZilchTypeId(void), 
                               Zilch::FunctionOptions::None);
    }

    bool BaseEventHandler::HandleGoodbyes()
    {
      assert(mActiveInvokeLoops >= 0);

      // don't handle goodbyes if mListeners is being iterated through as
      // changing it during iteration is undefined
      if (mActiveInvokeLoops > 0)
      {
        return false;
      }

      // go through all the goodbyes and remove the each handler from the
      // correct container.
      for (ConversationGoodbye& goodbye : mGoodbyes)
      {
        if (goodbye.mRoleOfHandler == ConversationRole::Listener)
        {
          std::vector<Listener>& listeners = mListeners[goodbye.mEventName];
          for (auto iter = listeners.begin(); iter != listeners.end(); ++iter)
          {
            if (iter->mHandler == goodbye.mHandler)
            {
              delete iter->mCallback;
              listeners.erase(iter);
              break;
            }
          }
        }
        else
        {
          assert(goodbye.mRoleOfHandler == ConversationRole::Speaker);

          std::vector<BaseEventHandler*>& speakers =
            mSpeakers[goodbye.mEventName];
          for (auto iter = speakers.begin(); iter != speakers.end(); ++iter)
          {
            if (*iter == goodbye.mHandler)
            {
              speakers.erase(iter);
              break;
            }
          }
        }
      }

      mGoodbyes.clear();
      return true;
    }

    void BaseEventHandler::Trigger(const std::string &eventName, Event *e)
    {
      if (eventName == Events::WindowFocusLostOrGained)
      {
        std::cout << "Window Event\n";
      }

      HandleGoodbyes();
      ++mActiveInvokeLoops;

      for (Listener& listener : mListeners[eventName])
      {
        // check if this listener has said goodbye
        // ideally there aren't many goodbyes so this lookup should be fast
        auto listenerGoodbye = std::find_if(mGoodbyes.begin(), mGoodbyes.end(),
          [&listener](ConversationGoodbye& goodbye)
          {
            return (goodbye.mRoleOfHandler == ConversationRole::Listener)
              && (goodbye.mHandler == listener.mHandler);
          }
        );

        // Only callback if the listener hasn't said goodbye
        if (listenerGoodbye == mGoodbyes.end())
        {
          listener.mCallback->Invoke(e);
        }
      }

      --mActiveInvokeLoops;
      HandleGoodbyes();
    }

    void BaseEventHandler::AddGoodbye(ConversationGoodbye aGoodbye)
    {
      // TODO@@@ (Austin): Switch goodbye string to YTE::String or at least move
      // REVIEW@@@ (Austin): Should this goodbye be handled now if it's safe?
      mGoodbyes.emplace_back(aGoodbye);
    }

    void BaseEventHandler::StopListening(
      const std::string &aEventName, BaseEventHandler& aSpeaker)
    {
      // let the speaker know it should forget this handler's callback
      aSpeaker.AddGoodbye(
        ConversationGoodbye(aEventName, this, ConversationRole::Listener));

      // remove the speaker from this handler's speaker container
      std::vector<BaseEventHandler*>& speakers = mSpeakers[aEventName];
      auto speaker = std::find(speakers.begin(), speakers.end(), &aSpeaker);
      assert(speaker != speakers.end());
      speakers.erase(speaker);
    }

    BaseEventHandler::~BaseEventHandler()
    {
      HandleGoodbyes();

      for (auto iter : mListeners)
      {
        ConversationGoodbye goodbye(iter.first, this,ConversationRole::Speaker);

        for (Listener& listener : mListeners[iter.first])
        {
          listener.mHandler->AddGoodbye(goodbye);
          delete listener.mCallback;
        }
      }

      for (auto iter : mSpeakers)
      {
        ConversationGoodbye goodbye(iter.first,this,ConversationRole::Listener);

        for (BaseEventHandler *handler : mSpeakers[iter.first])
        {
          handler->AddGoodbye(goodbye);
        }
      }
    }

    void ZilchEventCallback::Invoke(Event* e)
    {
      //BoundType* sentEventType = ZilchVirtualTypeId(e);

      //Zilch::TypeBinding::IsA(senteventType,
      //  func.BoundFunction->Type->Parameters[0].ParameterType);

      // The exception report stores any exceptions that may have occurred while
      // executing code. Exceptions include accessing arrays out of bounds,
      // dereferencing null, etc
      Zilch::ExceptionReport report;
      Zilch::Call call(func, state);
      call.DisableParameterChecks();
      call.Set(0, e);
      call.Invoke(report);
    }
  }
}

