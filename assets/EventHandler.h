template <typename ArgType>
class EventHandler
{
	public:
		using FuncType = std::function<void(ArgType)>;

		EventHandler()
		{
			eventList.clear();
		}

		~EventHandler()
		{
			eventList.clear();
		}
		void addEvent(const FuncType& _event)
		{
			for (const auto& e : eventList)
			{
				if (&e == &_event)
				{
					return;
				}
			}
			eventList.push_back(_event);
		}

		void removeEvent(const FuncType& _event)
		{
			eventList.erase(
				std::remove_if(eventList.begin(), eventList.end(),
					[&](const FuncType& e) { return e.target_type() == _event.target_type(); }),
				eventList.end()
			);
		}

		void Fire(ArgType _arg)
		{
			for (const auto& e : eventList)
			{
				e(_arg);
			}
		}

		void clearEvents()
		{
			eventList.clear();
		}

	private:
		std::vector<FuncType> eventList;
};

class EventHandlerNoArgs
{
public:
	using FuncType = std::function<void(void)>;

	EventHandlerNoArgs();
	~EventHandlerNoArgs();
	void addEvent(const FuncType& _event);

	void removeEvent(const FuncType& _event);

	void clearEvents();

	void Fire();
	
	private:
		std::vector<FuncType> eventList;
};
