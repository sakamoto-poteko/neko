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


#ifndef METRICS_DEF
#define METRICS_DEF

#define FLOATING_TYPE float

typedef struct MetricsVal_t
{
    FLOATING_TYPE Mean_SentenceLength;
    FLOATING_TYPE Mean_ClauseLength;
    FLOATING_TYPE Mean_ClauseNumberPerSentence;
    //FLOATING_TYPE Mean_WordsPerClause; // Later

    FLOATING_TYPE Mean_FirstClauseLength;
    FLOATING_TYPE Mode_FirstClauseLength;
    FLOATING_TYPE Mean_FirstClauseLengthUnder4;
    FLOATING_TYPE Mean_SentencesPerParagraph;
    FLOATING_TYPE Mean_SecondClauseLengthForFirstClauseUnder5;

    FLOATING_TYPE Mean_WordsPerClause;
    FLOATING_TYPE Mean_WordsPerClauseInFirstClause;
} MetricsVal;

#endif // METRICS_DEF

