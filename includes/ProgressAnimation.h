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
        static ProgressiveFunctionInvoker<class TRet,class TObj,class TObj2>* addNewProgressiveInvocation(TObj &obj,TRet (TObj::*funToInvoke)(double progress),
                                        TObj2 &progressNormalizationFunObject,double (TObj2::*progressNormalizationFun)(double timeProgress),double duration,double framerate=60);
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

    template<class TRet,class TObj> class AnimationInterface{
        private:
        ProgressiveFunctionInvoker<class TRet,class TObj,AnimationInterface>* currentProgressiveFunctionInvoker=nullptr;
        TObj *obj;
        TRet (TObj::*functionToInvoke)(double progress);
        double duration;
        double framerate;
        protected:
        virtual double progressNormalization(double timeProgress)=0;
        public:
        /**
         * While implementing the interface the first two parameters are to be passed to the AnimationInterface.
        */
        AnimationInterface(TObj &obj,TRet (TObj::*funToInvoke)(double progress),double duration,double framerate=60);
        ~AnimationInterface();
        virtual void start() final;
        virtual void stop() final;
        virtual void complete() final;
    };

} // namespace Animator
