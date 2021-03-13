/**
 * @file dmxmonitor.cpp
 *
 */
/* Copyright (C) 2016-2020 by Arjan van Vught mailto:info@orangepi-dmx.nl
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

#include <stdint.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <cassert>

#include "dmxmonitor.h"

DMXMonitor::DMXMonitor(void) {
	for (uint32_t i = 0; i < DMXMONITOR_MAX_PORTS; i++) {
		m_bIsStarted[i] = false;
	}
}

DMXMonitor::~DMXMonitor(void) {
}

void DMXMonitor::DisplayDateTime(uint8_t nPortId, const char *pString) {
	assert(nPortId < DMXMONITOR_MAX_PORTS);

	struct timeval tv;
	gettimeofday(&tv, NULL);
	struct tm tm = *localtime(&tv.tv_sec);

	printf("%.2d-%.2d-%.4d %.2d:%.2d:%.2d.%.6d %s:%c\n", tm.tm_mday,
			tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec,
			static_cast<int>(tv.tv_usec), pString, nPortId + 'A');
}

void DMXMonitor::SetMaxDmxChannels(uint16_t nMaxChannels) {
	m_nMaxChannels = nMaxChannels;
}

uint16_t DMXMonitor::GetDmxFootprint(void) {
	return m_nMaxChannels;
}

bool DMXMonitor::SetDmxStartAddress(uint16_t nDmxStartAddress) {
	if (nDmxStartAddress  > (512 - m_nMaxChannels)) {
		return false;
	}

	m_nDmxStartAddress = nDmxStartAddress;
	return true;
}

uint16_t DMXMonitor::GetDmxStartAddress(void) {
	return m_nDmxStartAddress;
}

void DMXMonitor::Start(uint8_t nPort) {
	assert(nPort < DMXMONITOR_MAX_PORTS);

	if(m_bIsStarted[nPort]) {
		return;
	}

	m_bIsStarted[nPort] = true;
	DisplayDateTime(nPort, "Start");
}

void DMXMonitor::Stop(uint8_t nPort) {
	assert(nPort < DMXMONITOR_MAX_PORTS);

	if(!m_bIsStarted[nPort]) {
		return;
	}

	m_bIsStarted[nPort] = false;
	DisplayDateTime(nPort, "Stop");
}

void DMXMonitor::SetData(uint8_t nPortId, const uint8_t *pData, uint16_t nLength) {
	assert(nPortId < DMXMONITOR_MAX_PORTS);

	struct timeval tv;
	uint32_t i, j;

	gettimeofday(&tv, NULL);
	struct tm tm = *localtime(&tv.tv_sec);

	printf("%.2d-%.2d-%.4d %.2d:%.2d:%.2d.%.6d DMX:%c %d:%d:%d ", tm.tm_mday,
			tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec,
			static_cast<int>(tv.tv_usec), nPortId + 'A',
			static_cast<int>(nLength),
			static_cast<int>(m_nMaxChannels),
			static_cast<int>(m_nDmxStartAddress));

	for (i = static_cast<uint32_t>(m_nDmxStartAddress - 1), j = 0; (i < nLength) && (j < m_nMaxChannels); i++, j++) {
		switch (m_tFormat) {
		case DMXMonitorFormat::DMX_MONITOR_FORMAT_PCT:
			printf("%3d ", ((pData[i] * 100)) / 255);
			break;
		case DMXMonitorFormat::DMX_MONITOR_FORMAT_DEC:
			printf("%3d ", pData[i]);
			break;
		default:
			printf("%.2x ", pData[i]);
			break;
		}
	}

	for (; j < m_nMaxChannels; j++) {
		printf("-- ");
	}

	printf("\n");
}

