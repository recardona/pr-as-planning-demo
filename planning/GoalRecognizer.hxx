#ifndef __GOAL_RECOGNIZER__
#define __GOAL_RECOGNIZER__

#include <QObject>
#include <planning/Goal.hxx>
#include <planning/STRIPS_Problem.hxx>
#include <planning/Action.hxx>
#include <planning/Fluent.hxx>
#include <planning/State.hxx>
#include <planning/Conditional_Effect.hxx>
#include <vector>
#include <fstream>

using aig_tk::STRIPS_Problem;
using aig_tk::Action;
using aig_tk::Fluent;
using aig_tk::Action_Ptr_Vec;
using aig_tk::Fluent_Vec;
using aig_tk::Fluent_Ptr_Vec;
using aig_tk::Conditional_Effect_Vec;
using aig_tk::Conditional_Effect;

namespace Planning
{

class GoalRecognizer : public QObject
{
	Q_OBJECT
public:

	GoalRecognizer( STRIPS_Problem& p, Goal& g, Action_Ptr_Vec& obs, Fluent_Vec& init );
	~GoalRecognizer();

	float 	obsCompliantLikelihood() const { return mObsCompliantLikelihood; }
	float	notObsCompliantLikelihood() const { return mNotObsCompliantLikelihood; }

	void	evaluateLikelihoods();
	bool	checkSolvable( STRIPS_Problem& prob );
	bool 	checkReachability( STRIPS_Problem& prob );
protected:

	void	copyFluents();
	void	compileObservations();	
	void	setInitialStates(STRIPS_Problem& p, Fluent_Ptr_Vec& notObsFluents);
	void	setGoals(STRIPS_Problem& p, Fluent_Ptr_Vec& obsFluents);
	void	createObsFluents(STRIPS_Problem& prob, Fluent_Ptr_Vec& obsFluents, Fluent_Ptr_Vec& notObsFluents);
	void	createActions( STRIPS_Problem& prob, Fluent_Ptr_Vec& obsFluents, Fluent_Ptr_Vec& notObsFluents );

	float	solve( STRIPS_Problem& prob );

	void	printInitAndGoal( STRIPS_Problem& p );
	void	printState( STRIPS_Problem& p, aig_tk::State& s );
	void	printFluentVec( STRIPS_Problem& p, Fluent_Vec& v );
	void	printActions( STRIPS_Problem& prob );
	void	printAction( STRIPS_Problem& prob, Action& a );
	void	printPlan( STRIPS_Problem& prob, std::vector< Action* >& a );

	bool	finished();
protected slots:

	void	notifyCompFinished();
	void	notifyNotCompFinished();
	
protected:

	float		mBeta;
	Goal&		mHypGoal;
	Action_Ptr_Vec&	mObsSequence;
	Fluent_Vec&	mInitialState;
	STRIPS_Problem&	mOriginalProblem;
	STRIPS_Problem	mCompliantProblem;
	STRIPS_Problem	mNotCompliantProblem;
	float		mObsCompliantLikelihood;
	float		mNotObsCompliantLikelihood;
	float		mObsCompliantCost;
	float		mNotObsCompliantCost;
	Fluent_Ptr_Vec	mObsFluentsComp;
	Fluent_Ptr_Vec	mNotObsFluentsComp;
	Fluent_Ptr_Vec	mObsFluentsNotComp;
	Fluent_Ptr_Vec	mNotObsFluentsNotComp;
	bool		mFinishedComp;
	bool		mFinishedNotComp;
};

}

#endif // GoalRecognizer.hxx