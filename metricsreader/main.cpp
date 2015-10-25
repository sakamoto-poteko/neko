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
#include <iomanip>
#include <cstdio>
#include <cstring>

#include "../fumino/Metrics_def.h"

int main(int argc, char *argv[])
{
    if (argc < 2) {
        std::cerr << "metricsreader metricsfile" << std::endl;
        return 1;
    }

    FILE *file = std::fopen(argv[1], "r");
    if (file == 0) {
        std::cerr << "cannot open file `" << argv[1] << "'" << std::endl;
        return 2;
    }

    std::fseek(file, 0, SEEK_END);
    long len = std::ftell(file);
    std::rewind(file);

    unsigned mtcEntries = len / sizeof(MetricsVal);
    MetricsVal *entries = new MetricsVal[mtcEntries];
    MetricsVal total;
    std::memset(&total, 0, sizeof(MetricsVal));

    std::fread(entries, 1, mtcEntries * sizeof(MetricsVal), file);

    std::printf("\033[1m");
    std::printf("Ave. SL     Ave. CL     Ave. CN/S    Ave.FCL   ^FCL   Ave. FCL<=4"
                "  Ave. S/P     Ave. SCL w FC<=5  Ave. W/C    Ave. W/FC   ");
    std::printf("\033[0m\n");
    for (unsigned int i = 0; i < mtcEntries; ++i) {
        std::printf("%-9.5f   %-9.5f   %-9.5f    %-9.5f %-3.0f    %-9.5f    %-9.5f    %-9.5f         %-9.4f   %-9.4f\n",
                    entries[i].Mean_SentenceLength, entries[i].Mean_ClauseLength, entries[i].Mean_ClauseNumberPerSentence,
                    entries[i].Mean_FirstClauseLength, entries[i].Mode_FirstClauseLength, entries[i].Mean_FirstClauseLengthUnder4,
                    entries[i].Mean_SentencesPerParagraph, entries[i].Mean_SecondClauseLengthForFirstClauseUnder5,
                    entries[i].Mean_WordsPerClause, entries[i].Mean_WordsPerClauseInFirstClause);

        total.Mean_SentenceLength += entries[i].Mean_SentenceLength;
        total.Mean_ClauseLength += entries[i].Mean_ClauseLength;
        total.Mean_ClauseNumberPerSentence += entries[i].Mean_ClauseNumberPerSentence;
        total.Mean_FirstClauseLength += entries[i].Mean_FirstClauseLength;
        total.Mode_FirstClauseLength += entries[i].Mode_FirstClauseLength;
        total.Mean_FirstClauseLengthUnder4 += entries[i].Mean_FirstClauseLengthUnder4;
        total.Mean_SentencesPerParagraph += entries[i].Mean_SentencesPerParagraph;
        total.Mean_SecondClauseLengthForFirstClauseUnder5 += entries[i].Mean_SecondClauseLengthForFirstClauseUnder5;
        total.Mean_WordsPerClause += entries[i].Mean_WordsPerClause;
        total.Mean_WordsPerClauseInFirstClause += entries[i].Mean_WordsPerClauseInFirstClause;
    }

    std::printf("\033[1m");
    std::printf("%-9.5f   %-9.5f   %-9.5f    %-9.5f %-3.0f    %-9.5f    %-9.5f    %-9.5f         %-9.4f   %-9.4f ",
                total.Mean_SentenceLength / mtcEntries, total.Mean_ClauseLength / mtcEntries,
                total.Mean_ClauseNumberPerSentence / mtcEntries, total.Mean_FirstClauseLength / mtcEntries,
                total.Mode_FirstClauseLength / mtcEntries, total.Mean_FirstClauseLengthUnder4 / mtcEntries,
                total.Mean_SentencesPerParagraph / mtcEntries, total.Mean_SecondClauseLengthForFirstClauseUnder5 / mtcEntries,
                total.Mean_WordsPerClause / mtcEntries, total.Mean_WordsPerClauseInFirstClause / mtcEntries);
    std::printf("\033[0m\n");

    return 0;
}

