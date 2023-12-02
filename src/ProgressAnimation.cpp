#include "ProgressAnimation.h"

namespace Animator{

    // Definitions of ProgressiveFunctionInvoker class functions

    ProgressiveFunctionInvoker<class TRet,class TObj,class TObj2>::ProgressiveFunctionInvoker(TObj &obj,TRet (TObj::*funToInvoke)(double progress),TObj2 &progressNormalizationFunObject,double (TObj2::*progressNormalizationFun)(double timeProgress),double duration,double framerate=60){
        this->obj=&obj;
        this->functionToInvoke=funToInvoke;
        this->progressNormalizationFunctionObject=&progressNormalizationFunObject;
        this->progressNormalizationFunction=progressNormalizationFun;
        this->duration=duration;
        this->frameRate=frameRate;
        this->timeStep=1000/frameRate;
        currentTime=0;
        init();
    }
    
    void ProgressiveFunctionInvoker<class TRet,class TObj,class TObj2>::init(){
        con=Glib::signal_timeout().connect(sigc::mem_fun(*this,&ProgressiveFunctionInvoker<class TRet,class TObj,class TObj2>::onProgressTimeStep),timeStep);
        invokeFunctionAtTimeProgress(0);
    }

    void ProgressiveFunctionInvoker<class TRet,class TObj,class TObj2>::invokeFunctionAtTimeProgress(double timeProgress){
        (obj->*functionToInvoke)((progressNormalizationFunctionObject->*progressNormalizationFunction)(0));
    }

    bool ProgressiveFunctionInvoker<class TRet,class TObj,class TObj2>::onProgressTimeStep(){
        currentTime+=timeStep;
        if(currentTime<duration){
            invokeFunctionAtTimeProgress(currentTime/duration);
        }
        else{
            complete();
        }
        return true;
    }
    void ProgressiveFunctionInvoker<class TRet,class TObj,class TObj2>::disconnect(){
        con.disconnect();
    }

    void ProgressiveFunctionInvoker<class TRet,class TObj,class TObj2>::destroy(){
        con.disconnect();
        delete this;
    }

    ProgressiveFunctionInvoker<class TRet,class TObj,class TObj2>* ProgressiveFunctionInvoker<class TRet,class TObj,class TObj2>::addNewProgressiveInvocation(TObj &obj,TRet (TObj::*funToInvoke)(double progress),TObj2 &progressNormalizationFunObject,double (TObj2::*progressNormalizationFun)(double timeProgress),double duration,double frameRate=60){
        return new ProgressiveFunctionInvoker(obj,funToInvoke,progressNormalizationFunObject,progressNormalizationFun,duration,frameRate);
    }

    void ProgressiveFunctionInvoker<class TRet,class TObj,class TObj2>::complete(){
        disconnect();
        invokeFunctionAtTimeProgress(1);
    }
    void ProgressiveFunctionInvoker<class TRet,class TObj,class TObj2>::endWithoutComplete(){
        disconnect();
    }


    // Definitions of AnimationInterface
    AnimationInterface<class TRet,class TObj>::AnimationInterface(TObj &obj,TRet (TObj::*funToInvoke)(double progress),double duration,double framerate=60){
        this->obj=&obj;
        this->functionToInvoke=funToInvoke;
        this->duration=duration;
        this->framerate=framerate;
    }

    AnimationInterface<class TRet,class TObj>::~AnimationInterface(){
        stop();
    }
    void AnimationInterface<class TRet,class TObj>::start(){
        stop();
        currentProgressiveFunctionInvoker=ProgressiveFunctionInvoker<TRet,TObj,AnimationInterface>::addNewProgressiveInvocation(*obj,functionToInvoke,*this,&AnimationInterface::progressNormalization,duration,framerate);
    }
    void AnimationInterface<class TRet,class TObj>::stop(){
        if(currentProgressiveFunctionInvoker){
            currentProgressiveFunctionInvoker->endWithoutComplete();
            currentProgressiveFunctionInvoker->destroy();
            currentProgressiveFunctionInvoker=nullptr;
        }
    }
    void AnimationInterface<class TRet,class TObj>::complete(){
        if(currentProgressiveFunctionInvoker){
            currentProgressiveFunctionInvoker->complete();
            currentProgressiveFunctionInvoker->destroy();
            currentProgressiveFunctionInvoker=nullptr;
        }
    }

} // namespace Animator
