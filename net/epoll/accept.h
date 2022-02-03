#pragma once

template <class Service>
class accept
{
public:
	accept(Service* service)
		: service_(service) 
	{
		
	};

private:
	Service* service_;
};