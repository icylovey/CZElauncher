#ifndef OBSERVER_IMPL_BASE2_HPP
#define OBSERVER_IMPL_BASE2_HPP

#include <map>

template <typename ReturnT, typename ParamT>
class ReceiverImplBase2;

template <typename ReturnT, typename ParamT>
class ObserverImplBase2
{
public:
	virtual void AddReceiver(ReceiverImplBase2<ReturnT, ParamT>* receiver) = 0;
	virtual void RemoveReceiver(ReceiverImplBase2<ReturnT, ParamT>* receiver) = 0;
	virtual ReturnT Broadcast(ParamT param) = 0;
	virtual ReturnT RBroadcast(ParamT param) = 0;
	virtual ReturnT Notify(ParamT param) = 0;
};

template <typename ReturnT, typename ParamT>
class ReceiverImplBase2
{
public:
	virtual void AddObserver(ObserverImplBase2<ReturnT, ParamT>* observer) = 0;
	virtual void RemoveObserver() = 0;
	virtual ReturnT Receive(ParamT param) = 0;
	virtual ReturnT Respond(ParamT param, ObserverImplBase2<ReturnT, ParamT>* observer) = 0;
};

template <typename ReturnT, typename ParamT>
class ReceiverImpl2;

template <typename ReturnT, typename ParamT>
class ObserverImpl2 : public ObserverImplBase2<ReturnT, ParamT>
{
	template <typename ReturnT, typename ParamT>
	friend class Iterator2;
public:
	ObserverImpl2()
	{}

	virtual ~ObserverImpl2()	{}

	virtual void AddReceiver(ReceiverImplBase2<ReturnT, ParamT>* receiver)
	{
		if (receiver == NULL)
			return;

		receivers_.push_back(receiver);
		receiver->AddObserver(this);
	}

	virtual void RemoveReceiver(ReceiverImplBase2<ReturnT, ParamT>* receiver)
	{
		if (receiver == NULL)
			return;

		ReceiversVector2::iterator it = receivers_.begin();
		for (; it != receivers_.end(); ++it)
		{
			if (*it == receiver)
			{
				receivers_.erase(it);
				break;
			}
		}
	}

	virtual ReturnT Broadcast(ParamT param)
	{
		ReceiversVector2::iterator it = receivers_.begin();
		for (; it != receivers_.end(); ++it)
		{
			(*it)->Receive(param);
		}

		return ReturnT();
	}

	virtual ReturnT RBroadcast(ParamT param)
	{
		ReceiversVector::reverse_iterator it = receivers_.rbegin();
		for (; it != receivers_.rend(); ++it)
		{
			(*it)->Receive(param);
		}

		return ReturnT();
	}

	virtual ReturnT Notify(ParamT param)
	{
		ReceiversVector::iterator it = receivers_.begin();
		for (; it != receivers_.end(); ++it)
		{
			(*it)->Respond(param, this);
		}

		return ReturnT();
	}

	template <typename ReturnT, typename ParamT>
	class Iterator2
	{
		ObserverImpl2<ReturnT, ParamT> & _tbl;
		DWORD index;
		ReceiverImplBase2<ReturnT, ParamT>* ptr;
	public:
		Iterator2( ObserverImpl & table )
			: _tbl( table ), index(0), ptr(NULL)
		{}

		Iterator2( const Iterator & v )
			: _tbl( v._tbl ), index(v.index), ptr(v.ptr)
		{}

		ReceiverImplBase2<ReturnT, ParamT>* next()
		{
			if ( index >= _tbl.receivers_.size() )
				return NULL;

			for ( ; index < _tbl.receivers_.size(); )
			{
				ptr = _tbl.receivers_[ index++ ];
				if ( ptr )
					return ptr;
			}
			return NULL;
		}
	};

protected:
	typedef std::vector<ReceiverImplBase2<ReturnT, ParamT>*> ReceiversVector2;
	ReceiversVector2 receivers_;
};


template <typename ReturnT, typename ParamT>
class ReceiverImpl2 : public ReceiverImplBase2<ReturnT, ParamT>
{
public:
	ReceiverImpl2()
	{}

	virtual ~ReceiverImpl2()	{}

	virtual void AddObserver(ObserverImplBase2<ReturnT, ParamT>* observer)
	{
		observers_.push_back(observer);
	}

	virtual void RemoveObserver()
	{
		ObserversVector2::iterator it = observers_.begin();
		for (; it != observers_.end(); ++it)
		{
			(*it)->RemoveReceiver(this);
		}
	}

	virtual ReturnT Receive(ParamT param)
	{
		return ReturnT();
	}

	virtual ReturnT Respond(ParamT param, ObserverImplBase2<ReturnT, ParamT>* observer)
	{
		return ReturnT();
	}

protected:
	typedef std::vector<ObserverImplBase2<ReturnT, ParamT>*> ObserversVector2;
	ObserversVector2 observers_;
};

#endif // OBSERVER_IMPL_BASE_HPP