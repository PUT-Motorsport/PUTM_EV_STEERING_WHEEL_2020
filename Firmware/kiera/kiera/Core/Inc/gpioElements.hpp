/*
 * gpio elements.hpp
 *
 *  Created on: 6 maj 2022
 *      Author: Piotr Lesicki
 */

#ifndef INC_GPIOELEMENTS_HPP_
#define INC_GPIOELEMENTS_HPP_

#include "stm32l4xx_hal.h"

class GpioElement
{
	public:
		GpioElement(const GPIO_TypeDef * const port, const uint32_t pin, const bool is_inverted);

		virtual void handle() const { };

		virtual ~GpioElement() = default;

	protected:
		mutable bool state;
		mutable bool is_inverted;

		const uint32_t pin;

		const GPIO_TypeDef * const port;
};

class GpioOutElement : public GpioElement
{
	public:
		GpioOutElement(const GPIO_TypeDef * const port, const uint32_t pin);
		GpioOutElement(const GPIO_TypeDef * const port, const uint32_t pin, const bool is_inverted);

		void activate();
		void deactivate();

		//virtual void handle() override = 0;
		void toggle();

		~GpioOutElement() { };
};

class GpioInElement : public GpioElement
{
	public:
		GpioInElement(const GPIO_TypeDef * const port, const uint32_t pin);
		GpioInElement(const GPIO_TypeDef * const port, const uint32_t pin, const bool is_inverted);

		virtual void handle() const override;

		bool isActive() const;
		bool risingEdge() const;
		bool fallingEdge() const;

		bool const& getStateConstRefForDebug() const;

		~GpioInElement() { };

	private:
		mutable bool rising_edge = false;
		mutable bool falling_edge = false;
};

#endif /* INC_GPIOELEMENTS_HPP_ */
