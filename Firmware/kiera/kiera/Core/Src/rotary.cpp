/*
 * rotary.cpp
 *
 *  Created on: 25 mar 2023
 *      Author: pile
 */

#include "rotary.hpp"

Rotary::Rotary(std::array < GpioInElement const* , rotary_pin_count > const &in) : in(in) { }

void Rotary::handle() const
{
	for(auto ptr : in) ptr->handle();

	auto ret = RotationDirection::None;

	std::array< bool, rotary_pin_count > curr_states;
	for(size_t i = 0; i < rotary_pin_count; i++) curr_states[i] = in[i]->isActive();

	for(int i = 0; i < int(rotary_pin_count); i++)
	{
		int im = i == 0 ? rotary_pin_count - 1 : i - 1;
		int ip = i == rotary_pin_count - 1 ? 0 : i + 1;

		if(curr_states[i] && prev_states[ip])
		{
			ret = RotationDirection::Forward;
			break;
		}
		if(curr_states[i] && prev_states[im])
		{
			ret = RotationDirection::Backwards;
			break;
		}
	}

	this->prev_states = curr_states;
	this->rotation = ret;
}

RotationDirection Rotary::getRotation()
{
	return this->rotation;
}

RotationDirection const& Rotary::getRotationConstRefForDebug() const
{
	return this->rotation;
}
