#if !defined(__MISC_STATE_MACHINE_HPP__)
#define __MISC_STATE_MACHINE_HPP__

#include <vector>
#include <boost/function.hpp>

namespace misc {;

template < class StateType, int StatesCount = StateType::StatesCount >
struct StateMachine
{
	StateMachine(typename StateType::type initialState = static_cast<typename StateType::type>(0))
		: _currentState(initialState),
		_upTransitions(StatesCount),
		_downTransitions(StatesCount)
	{}

	typedef boost::function< bool () > TransitionFunc;

	void add_transition_up_func(typename StateType::type startState, TransitionFunc fn)
	{
		size_t newState = static_cast<size_t>(startState);
		//if(_upTransitions.size() <= newState)
		//	_upTransitions.resize(newState+1);
		_upTransitions[newState] = fn;
	}

	void add_transition_down_func(typename StateType::type startState, TransitionFunc fn)
	{
		size_t newState = static_cast<size_t>(startState);
		//if(_downTransitions.size() <= newState)
		//	_downTransitions.resize(newState+1);
		_downTransitions[newState] = fn;
	}

	bool set_state(typename StateType::type targetState)
	{
		typename StateType::type initialTargetState = targetState;
		while(targetState < _currentState)
		{
			if(!(_downTransitions[static_cast<size_t>(_currentState)])())
				return false;
			--_currentState;
		}
		while(targetState > _currentState)
		{
			if(!(_upTransitions[static_cast<size_t>(_currentState)])())
				return false;
			++_currentState;
		}

		return true;
	}

	typename StateType::type get_state() const { return _currentState; }

private:
	StateMachine(const StateMachine& other);
	void operator=(const StateMachine& other);

	typename StateType::type _currentState;
	std::vector < TransitionFunc > _upTransitions, _downTransitions;
};

}

#endif // __MISC_STATE_MACHINE_HPP__