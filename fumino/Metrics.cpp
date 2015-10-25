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

#include <QVector>
#include <QStringList>
#include <QDir>

#include "../Jieba/src/Jieba.hpp"

#include "Metrics.h"

Metrics::Metrics()
{
    if (_cutter == 0) {
        QDir home = QDir::home();
        _cutter = new cppjieba::Jieba(home.filePath(".neko/dict/dict.utf8").toStdString(),
                                      home.filePath(".neko/dict/hmm.utf8").toStdString(),
                                      home.filePath(".neko/dict/user.dict.utf8").toStdString());
    }
}

void Metrics::SimpleStatistics(QString article, MetricsVal &stat)
{
    article = article.remove(QRegExp("：|“|”|《|》|·"));

    QVector<QVector<QVector<QString>>> clauses;
    // par' sent'   clau'

    QStringList paragraphs = article.split(QRegExp("\n|\r\n|\r"), QString::SkipEmptyParts);
    foreach (QString para, paragraphs) {
        QStringList sentences = para.split(QRegExp("。|？|！|…|；"), QString::SkipEmptyParts);

        QVector<QVector<QString>> vecSentence;
        foreach (QString sent, sentences) {
            QStringList clauses = sent.split(QRegExp("，|,"), QString::SkipEmptyParts);

            QVector<QString> vecClause;
            foreach (QString clause, clauses) {
                auto str = clause.simplified();
                if (!str.isEmpty())
                    vecClause.append(str);
            }

            if (!vecClause.isEmpty())
                vecSentence.append(vecClause);
        }

        if (!vecSentence.isEmpty())
            clauses.append(vecSentence);
    }

    int paragraphCount = clauses.size();
    int sentenceCount = std::accumulate(clauses.begin(), clauses.end(), 0,
                                         [](int a, const QVector<QVector<QString>> &b) {
        return a + b.size();
    });
    int clauseCount = std::accumulate(clauses.begin(), clauses.end(), 0,
                                       [](int a, const QVector<QVector<QString>> &b) {
        int accmb = std::accumulate(b.begin(), b.end(), 0, [](int k, const QVector<QString> &j) {
            return k + j.size();
        });
        return a + accmb;
    });
    int charCount = std::accumulate(clauses.begin(), clauses.end(),0,
                                     [](int a, const QVector<QVector<QString>> &b) {
      int accmb = std::accumulate(b.begin(), b.end(), 0, [](int k, const QVector<QString> &j) {
          int accmy = std::accumulate(j.begin(), j.end(), 0, [](int x, const QString &y) {
              return x + y.size();
          });
          return k + accmy;
      });
      return a + accmb;
  });

    stat.Mean_SentencesPerParagraph = (double)sentenceCount / paragraphCount;
    stat.Mean_ClauseNumberPerSentence = (double)clauseCount / sentenceCount;
    stat.Mean_SentenceLength = (double)charCount / sentenceCount;
    stat.Mean_ClauseLength = (double)charCount / clauseCount;

    if (paragraphCount == 0 || sentenceCount == 0 || clauseCount == 0 || charCount == 0) {
        std::cerr << "Error: Invalid file" << std::endl;
    }

    QVector<int> firstClauseLength;
    QVector<int> secondClauseLengthIfFirstClauseShorterThan5;
    foreach (QVector<QVector<QString>> para, clauses) {
        foreach (QVector<QString> sent, para) {
            firstClauseLength.append(sent.at(0).size());

            if (sent.size() > 1 && sent.at(0).size() <= 5) {
                secondClauseLengthIfFirstClauseShorterThan5.append(sent.at(1).size());
            }
        }
    }
    unsigned int firstClauseCount = firstClauseLength.size();

    if (secondClauseLengthIfFirstClauseShorterThan5.size() == 0) {
        stat.Mean_SecondClauseLengthForFirstClauseUnder5 = 0;
        std::cout << "[INFO] No second clause has the prior clause shorter than 5" << std::endl;
    } else {
        stat.Mean_SecondClauseLengthForFirstClauseUnder5 =
                (double)std::accumulate(secondClauseLengthIfFirstClauseShorterThan5.begin(),
                                        secondClauseLengthIfFirstClauseShorterThan5.end(), 0)
                / secondClauseLengthIfFirstClauseShorterThan5.size();
    }

    stat.Mean_FirstClauseLength = std::accumulate(firstClauseLength.begin(), firstClauseLength.end(), 0) / (double)firstClauseCount;

    int sum_fclu4 = 0;
    int num_fclu4 = 0;
    foreach (int len, firstClauseLength) {
        if (len <= 4) {
            sum_fclu4 += len;
            ++num_fclu4;
        }
    }
    if (num_fclu4 != 0) {
        stat.Mean_FirstClauseLengthUnder4 = (double)sum_fclu4 / num_fclu4;
    } else {
        stat.Mean_FirstClauseLengthUnder4 = 0;
        std::cout << "[INFO] No first clause has the size under 4" << std::endl;
    }
    std::map<int, int> fclu_map;
    foreach (int len, firstClauseLength) {
        fclu_map[len]++;
    }

    int mode_fclu_max = 0;
    int mode_fclu_num = 0;

    for (auto &pa : fclu_map) {
        if (mode_fclu_max <= pa.second) {
            if (pa.first > mode_fclu_num && mode_fclu_max == pa.second)
                continue;
            mode_fclu_max = pa.second;
            mode_fclu_num = pa.first;
        }
    }
    stat.Mode_FirstClauseLength = mode_fclu_num;




    std::vector<std::string> fullContentStrippedCut;
    std::vector<std::string> firstClauseContentStrippedCut;
    QString fullContentStripped;
    QString firstClauseContentStripped;
    fullContentStripped.reserve(charCount);

    foreach (QVector<QVector<QString>> para, clauses) {
        foreach (QVector<QString> sent, para) {
            for (int i = 0; i < sent.size(); ++i) {
                fullContentStripped.append(sent.at(i));
                if (i == 0)
                    firstClauseContentStripped.append(sent.at(0));
            }
        }
    }

    _cutter->Cut(fullContentStripped.toUtf8().constData(), fullContentStrippedCut, true);
    _cutter->Cut(firstClauseContentStripped.toUtf8().constData(), firstClauseContentStrippedCut, true);

    stat.Mean_WordsPerClause = (double)fullContentStrippedCut.size() / clauseCount;
    stat.Mean_WordsPerClauseInFirstClause = (double)firstClauseContentStrippedCut.size() / firstClauseCount;
}

MetricsVal Metrics::calculate(QString article)
{
    MetricsVal stat;

    SimpleStatistics(article, stat);

    return stat;
}
