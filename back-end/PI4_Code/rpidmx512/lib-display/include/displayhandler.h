/**
 * @file displayhandler.h
 *
 */
/* Copyright (C) 2020 by Arjan van Vught mailto:info@orangepi-dmx.nl
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:

 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef DISPLAYHANDLER_H_
#define DISPLAYHANDLER_H_

#include <stdint.h>
#include <stdio.h>

#include "ledblinkdisplay.h"

#include "ledblink.h"
#include "display.h"

class DisplayHandler: public LedBlinkDisplay {
public:
	DisplayHandler(): m_bHaveDisplay(Display::Get() != nullptr) {}
	~DisplayHandler() {}

	void Print(uint32_t nState) {
		if (m_bHaveDisplay) {
			char c;
			switch (static_cast<tLedBlinkMode>(nState)) {
				case LEDBLINK_MODE_OFF_OFF:
					c = 'O';
					break;
				case LEDBLINK_MODE_OFF_ON:
					c = 'O';
					break;
				case LEDBLINK_MODE_NORMAL:
					c = 'N';
					break;
				case LEDBLINK_MODE_DATA:
					c = 'D';
					break;
				case LEDBLINK_MODE_FAST:
					c = 'F';
					break;
				default:
					c = 'U';
					break;
			}

			Display::Get()->SetCursorPos(20,7);
			Display::Get()->PutChar(c);
		}
	}

private:
	bool m_bHaveDisplay;
};

#endif /* DISPLAYHANDLER_H_ */
