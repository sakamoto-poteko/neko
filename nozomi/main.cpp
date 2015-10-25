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

#include <iostream>
#include <vector>
#include <cstdio>

#include <floatfann.h>

#include "../fumino/Metrics_def.h"


bool read_metrics(const char *authorFilename, const char *referenceFilename, fann_train_data &trainData)
{
    FILE *authMtcFile = std::fopen(authorFilename, "r");
    if (authMtcFile == 0) {
        std::cerr << "Failed to open file " << authorFilename << std::endl;
        return false;
    }

    FILE *refMtcFile = std::fopen(referenceFilename, "r");
    if (refMtcFile == 0) {
        std::cerr << "Failed to open file "  << referenceFilename << std::endl;
        std::fclose(authMtcFile);
        return false;
    }

    unsigned int metricsEntrySize = sizeof(MetricsVal);
    const unsigned int metricsSizeOfFloating = metricsEntrySize / sizeof(FLOATING_TYPE);

    std::fseek(authMtcFile, 0, SEEK_END);
    std::fseek(refMtcFile, 0, SEEK_END);
    long authMtcFileLen = std::ftell(authMtcFile);
    long refMtcFileLen = std::ftell(refMtcFile);
    std::rewind(authMtcFile);
    std::rewind(refMtcFile);
    unsigned int authMtcDataSize = authMtcFileLen / metricsEntrySize * metricsEntrySize;
    unsigned int refMtcDataSize = refMtcFileLen / metricsEntrySize * metricsEntrySize;
    unsigned int authMtcEntryCount = authMtcDataSize / metricsEntrySize;
    unsigned int refMtcEntryCount = refMtcDataSize / metricsEntrySize;

    FLOATING_TYPE *mtcData = new FLOATING_TYPE[(authMtcEntryCount + refMtcEntryCount) * metricsSizeOfFloating];
    FLOATING_TYPE *mtcResult = new FLOATING_TYPE[authMtcEntryCount + refMtcEntryCount];  // 1 for author, 0 for reference

    std::fread(mtcData, 1, authMtcDataSize, authMtcFile);
    std::fread(mtcData + authMtcDataSize / sizeof(FLOATING_TYPE), 1, refMtcDataSize, refMtcFile);


    for (unsigned int i = 0; i < authMtcEntryCount; ++i)
        mtcResult[i] = 1.;
    for (unsigned int i = 0; i < refMtcEntryCount; ++i)
        mtcResult[i + authMtcEntryCount] = 0.;

    FLOATING_TYPE **mtcDataIdx = new FLOATING_TYPE*[authMtcEntryCount + refMtcEntryCount];
    FLOATING_TYPE **mtcOutputIdx = new FLOATING_TYPE*[authMtcEntryCount + refMtcEntryCount];

    for (unsigned int i = 0; i < authMtcEntryCount + refMtcEntryCount; ++i) {
        mtcDataIdx[i] = mtcData + i * metricsSizeOfFloating;
        mtcOutputIdx[i] = mtcResult + i;

//        MetricsVal *val = (MetricsVal *)(mtcDataIdx[i]);
//        std::cout << val->Mean_SentenceLength << "\t" << val->Mean_ClauseLength << "\t"
//                  << val->Mean_ClauseNumberPerSentence << "\t" << val->Mean_FirstClauseLength << "\t"
//                  << val->Mode_FirstClauseLength << "\t" << val->Mean_FirstClauseLengthUnder4 << "\t"
//                  << val->Mean_SentencesPerParagraph << "\t" << val->Mean_SecondClauseLengthForFirstClauseUnder5 << "\t"
//                  << " == " << *(mtcOutputIdx[i]) << std::endl
////                  << mtcDataIdx[i][0] << "\t" << mtcDataIdx[i][1] << "\t" << mtcDataIdx[i][2] << "\t"
////                  << mtcDataIdx[i][3] << "\t" << mtcDataIdx[i][4] << "\t" << mtcDataIdx[i][5] << "\t"
////                  << mtcDataIdx[i][6] << "\t" << mtcDataIdx[i][7] << "\t" << std::endl;
    }

    trainData.input = mtcDataIdx;
    trainData.output = mtcOutputIdx;
    trainData.num_data = authMtcEntryCount + refMtcEntryCount;
    trainData.num_input = metricsSizeOfFloating;   // NOTE: Change this when MetricsVal changed
    trainData.num_output = 1;
    trainData.error_log = stderr;

    std::fclose(authMtcFile);
    std::fclose(refMtcFile);

    return true;
}

void destroy_metrics(fann_train_data &trainData)
{
    delete[] trainData.output[0];
    delete[] trainData.output;
    trainData.output = 0;
    delete[] trainData.input[0];
    delete[] trainData.input;
    trainData.input = 0;
}

int main(int argc, char *argv[])
{
    if (argc < 4) {
        std::cerr << "nozomi fumino_output_author fumino_output_not_the_author trained_data" << std::endl;
        return 1;
    }

    const FLOATING_TYPE desired_error = 0.0001;
    const unsigned int max_epochs = 5000000;
    const unsigned int epochs_between_reports = 10000;

    const int layer_num = 3;
    const unsigned int layers[layer_num] = {10, 8, 1};
    // Input, Hidden, Output

    struct fann *ann = fann_create_standard_array(layer_num, layers);

    fann_set_activation_function_hidden(ann, FANN_SIGMOID_SYMMETRIC);
    fann_set_activation_function_output(ann, FANN_SIGMOID_SYMMETRIC);

    fann_train_data trainData;
    bool success = read_metrics(argv[1], argv[2], trainData);
    if (!success)
        return 1;

    fann_train_on_data(ann, &trainData, max_epochs, epochs_between_reports, desired_error);

    destroy_metrics(trainData);

    fann_save(ann, argv[3]);

    fann_destroy(ann);

    return 0;
}

