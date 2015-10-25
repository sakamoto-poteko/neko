/**************************************************************************
 * Copyright (c) 2015 Afa.L Cheng <afa@afa.moe>
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 ***************************************************************************/


#ifndef READMETRICS_H
#define READMETRICS_H

#include <cstdio>
#include <iostream>

#include "Metrics_def.h"

FLOATING_TYPE* ReadMetrics(const char *filename, unsigned int &entries)
{
    FILE *authMtcFile = std::fopen(filename, "r");
    if (authMtcFile == 0) {
        std::cerr << "Failed to open file " << filename << std::endl;
        return false;
    }

    unsigned int metricsEntrySize = sizeof(MetricsVal);
    const unsigned int metricsSizeOfFloating = metricsEntrySize / sizeof(FLOATING_TYPE);

    std::fseek(authMtcFile, 0, SEEK_END);
    long authMtcFileLen = std::ftell(authMtcFile);
    std::rewind(authMtcFile);
    unsigned int authMtcDataSize = authMtcFileLen / metricsEntrySize * metricsEntrySize;
    unsigned int authMtcEntryCount = authMtcDataSize / metricsEntrySize;

    FLOATING_TYPE *mtcData = new FLOATING_TYPE[(authMtcEntryCount + refMtcEntryCount) * metricsSizeOfFloating];
    std::fread(mtcData, 1, authMtcDataSize, authMtcFile);


}


#endif // READMETRICS_H
