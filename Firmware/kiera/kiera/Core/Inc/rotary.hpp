/*
 * rotary.hpp
 *
 *  Created on: 25 mar 2023
 *      Author: pile
 */

#ifndef INC_ROTARY_HPP_
#define INC_ROTARY_HPP_

#include "gpioElements.hpp"
#include "etl/array.h"

const size_t rotary_pin_count = 4;

enum class RotationDirection : int8_t
{
	Backwards = -1,
	None = 0,
	Forward = 1
};

class Rotary
{
	public:
		explicit Rotary(std::array < GpioInElement const*, rotary_pin_count > const &in);

		void handle() const;
		RotationDirection const& getRotationConstRefForDebug() const;

		RotationDirection getRotation();
	private:
		mutable RotationDirection rotation;
		std::array< GpioInElement const*, rotary_pin_count > in;
		mutable std::array< bool, rotary_pin_count > prev_states = { false };
};


#endif /* INC_ROTARY_HPP_ */
