#pragma once
#include<glibmm.h>
namespace Animator{
    /**
     * Used to invoke a function progressively
     * from progress 0 to progress 1
     * then self distructs itself.
     * 
     * Note: This is only used in the declaration of AnimationInterface.
    */
    template<typename TRet,class TObj,class TObj2> class ProgressiveFunctionInvoker{
        protected:
        TObj *obj;
        TRet (TObj::*functionToInvoke)(double progress);
        double duration;
        double frameRate;
        double timeStep;
        double currentTime;
        /**
         * progress = function in terms of timeProgress
         * where timeProgress goes from 0 to 1, deside how progress must go.
        */
        double (TObj2::*progressNormalizationFunction)(double timeProgress);
        TObj2 *progressNormalizationFunctionObject;
        /**
         * using signal c++ lib to connect to the onProgressTimeStep function
         * to call it at the desired intervals.
        */
        sigc::connection con;

        ProgressiveFunctionInvoker(TObj &obj,TRet (TObj::*funToInvoke)(double progress),TObj2 &progressNormalizationFunObject,double (TObj2::*progressNormalizationFun)(double timeProgress),double duration,double frameRate=60);
        ~ProgressiveFunctionInvoker();
        /**
         * starts the animation
        */
        void init();
        void invokeFunctionAtTimeProgress(double timeProgress);
        /**
         * When time gets progressed by the timeStep decided by the frameRate.
        */
        bool onProgressTimeStep();
        /**
         * stops calling the animation
        */
        void disconnect();
        public:
        static ProgressiveFunctionInvoker<TRet,TObj,TObj2>* addNewProgressiveInvocation(TObj &obj,TRet (TObj::*funToInvoke)(double progress),
                                        TObj2 &progressNormalizationFunObject,double (TObj2::*progressNormalizationFun)(double timeProgress),double duration,double frameRate=60);
        /**
         * Invokes progress = 1, and ends the animation.
        */
        void complete();
        /**
         * Ends the animation without invoking progress = 1
        */
        void endWithoutComplete();
        /**
         * destroys this object(frees from memory);
        */
        void destroy();
    };

} // namespace Animator

namespace Animator{

    // Definitions of ProgressiveFunctionInvoker functions

    template<class TRet,class TObj,class TObj2> ProgressiveFunctionInvoker<TRet,TObj,TObj2>::ProgressiveFunctionInvoker(TObj &obj,TRet (TObj::*funToInvoke)(double progress),TObj2 &progressNormalizationFunObject,double (TObj2::*progressNormalizationFun)(double timeProgress),double duration,double frameRate){
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
    template<class TRet,class TObj,class TObj2> ProgressiveFunctionInvoker<TRet,TObj,TObj2>::~ProgressiveFunctionInvoker(){

    }
    template<class TRet,class TObj,class TObj2>void ProgressiveFunctionInvoker<TRet,TObj,TObj2>::init(){
        con=Glib::signal_timeout().connect(sigc::mem_fun(*this,&ProgressiveFunctionInvoker<TRet,TObj,TObj2>::onProgressTimeStep),timeStep);
        invokeFunctionAtTimeProgress(0);
    }

    template<class TRet,class TObj,class TObj2>void ProgressiveFunctionInvoker<TRet,TObj,TObj2>::invokeFunctionAtTimeProgress(double timeProgress){
        (obj->*functionToInvoke)((progressNormalizationFunctionObject->*progressNormalizationFunction)(timeProgress));
    }

    template<class TRet,class TObj,class TObj2>bool ProgressiveFunctionInvoker<TRet,TObj,TObj2>::onProgressTimeStep(){
        currentTime+=timeStep;
        if(currentTime<duration){
            invokeFunctionAtTimeProgress(currentTime/duration);
        }
        else{
            complete();
        }
        return true;
    }
    template<class TRet,class TObj,class TObj2>void ProgressiveFunctionInvoker<TRet,TObj,TObj2>::disconnect(){
        con.disconnect();
    }

    template<class TRet,class TObj,class TObj2>void ProgressiveFunctionInvoker<TRet,TObj,TObj2>::destroy(){
        con.disconnect();
        delete this;
    }

    template<class TRet,class TObj,class TObj2> ProgressiveFunctionInvoker<TRet,TObj,TObj2>* ProgressiveFunctionInvoker<TRet,TObj,TObj2>::addNewProgressiveInvocation(TObj &obj,TRet (TObj::*funToInvoke)(double progress),TObj2 &progressNormalizationFunObject,double (TObj2::*progressNormalizationFun)(double timeProgress),double duration,double frameRate){
        return new ProgressiveFunctionInvoker(obj,funToInvoke,progressNormalizationFunObject,progressNormalizationFun,duration,frameRate);
    }

    template<class TRet,class TObj,class TObj2> void ProgressiveFunctionInvoker<TRet,TObj,TObj2>::complete(){
        disconnect();
        invokeFunctionAtTimeProgress(1);
    }
    template<class TRet,class TObj,class TObj2> void ProgressiveFunctionInvoker<TRet,TObj,TObj2>::endWithoutComplete(){
        disconnect();
    }

} // namespace Animator

