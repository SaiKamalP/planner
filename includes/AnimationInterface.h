#pragma once
#include "ProgressFunctionInvoker.h"
namespace Animator
{
    template<class TRet,class TObj> class AnimationInterface{
        private:
        ProgressiveFunctionInvoker<TRet,TObj,AnimationInterface>* currentProgressiveFunctionInvoker=nullptr;
        TObj *obj;
        TRet (TObj::*functionToInvoke)(double progress);
        double duration;
        double framerate;
        protected:
        virtual double progressNormalization(double timeProgress)=0;
        public:
        AnimationInterface(TObj &obj,TRet (TObj::*funToInvoke)(double progress),double duration,double framerate);
        ~AnimationInterface();
        virtual void start() final;
        virtual void stop() final;
        virtual void complete() final;
    };

} // namespace Animator


namespace Animator{
    // Definitions of AnimationInterface
    template<class TRet,class TObj> AnimationInterface<TRet,TObj>::AnimationInterface(TObj &obj,TRet (TObj::*funToInvoke)(double progress),double duration,double framerate){
        this->obj=&obj;
        this->functionToInvoke=funToInvoke;
        this->duration=duration;
        this->framerate=framerate;
    }

    template<class TRet,class TObj> AnimationInterface<TRet,TObj>::~AnimationInterface(){
        stop();
    }
    template<class TRet,class TObj> void AnimationInterface<TRet,TObj>::start(){
        stop();
        currentProgressiveFunctionInvoker=ProgressiveFunctionInvoker<TRet,TObj,AnimationInterface>::addNewProgressiveInvocation(*obj,functionToInvoke,*this,&AnimationInterface::progressNormalization,duration,framerate);
    }
    template<class TRet,class TObj> void AnimationInterface<TRet,TObj>::stop(){
        if(currentProgressiveFunctionInvoker){
            currentProgressiveFunctionInvoker->endWithoutComplete();
            currentProgressiveFunctionInvoker->destroy();
            currentProgressiveFunctionInvoker=nullptr;
        }
    }
    template<class TRet,class TObj> void AnimationInterface<TRet,TObj>::complete(){
        if(currentProgressiveFunctionInvoker){
            currentProgressiveFunctionInvoker->complete();
            currentProgressiveFunctionInvoker->destroy();
            currentProgressiveFunctionInvoker=nullptr;
        }
    }
}