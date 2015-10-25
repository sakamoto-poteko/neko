#ifndef CPPJIEBA_KEYWORD_EXTRACTOR_H
#define CPPJIEBA_KEYWORD_EXTRACTOR_H

#include "MixSegment.hpp"
#include <cmath>
#include <set>

namespace cppjieba {
using namespace limonp;

/*utf8*/
class KeywordExtractor {
 public:
  KeywordExtractor(const string& dictPath, 
        const string& hmmFilePath, 
        const string& idfPath, 
        const string& stopWordPath, 
        const string& userDict = "") 
    : segment_(dictPath, hmmFilePath, userDict) {
    LoadIdfDict(idfPath);
    LoadStopWordDict(stopWordPath);
  }
  KeywordExtractor(const DictTrie* dictTrie, 
        const HMMModel* model,
        const string& idfPath, 
        const string& stopWordPath) 
    : segment_(dictTrie, model){
    LoadIdfDict(idfPath);
    LoadStopWordDict(stopWordPath);
  }
  ~KeywordExtractor() {
  }

  bool extract(const string& sentence, vector<string>& keywords, size_t topN) const {
    vector<pair<string, double> > topWords;
    if (!extract(sentence, topWords, topN)) {
      return false;
    }
    for (size_t i = 0; i < topWords.size(); i++) {
      keywords.push_back(topWords[i].first);
    }
    return true;
  }

  bool extract(const string& sentence, vector<pair<string, double> >& keywords, size_t topN) const {
    vector<string> words;
    segment_.cut(sentence, words);

    map<string, double> wordmap;
    for (vector<string>::iterator iter = words.begin(); iter != words.end(); iter++) {
      if (IsSingleWord(*iter)) {
        continue;
      }
      wordmap[*iter] += 1.0;
    }

    for (map<string, double>::iterator itr = wordmap.begin(); itr != wordmap.end(); ) {
      if (stopWords_.end() != stopWords_.find(itr->first)) {
        wordmap.erase(itr++);
        continue;
      }

      unordered_map<string, double>::const_iterator cit = idfMap_.find(itr->first);
      if (cit != idfMap_.end()) {
        itr->second *= cit->second;
      } else {
        itr->second *= idfAverage_;
      }
      itr ++;
    }

    keywords.clear();
    std::copy(wordmap.begin(), wordmap.end(), std::inserter(keywords, keywords.begin()));
    topN = min(topN, keywords.size());
    partial_sort(keywords.begin(), keywords.begin() + topN, keywords.end(), Compare);
    keywords.resize(topN);
    return true;
  }
 private:
  void LoadIdfDict(const string& idfPath) {
    ifstream ifs(idfPath.c_str());
    if (!ifs.is_open()) {
      LogFatal("open %s failed.", idfPath.c_str());
    }
    string line ;
    vector<string> buf;
    double idf = 0.0;
    double idfSum = 0.0;
    size_t lineno = 0;
    for (; getline(ifs, line); lineno++) {
      buf.clear();
      if (line.empty()) {
        LogError("line[%d] empty. skipped.", lineno);
        continue;
      }
      split(line, buf, " ");
      if (buf.size() != 2) {
        LogError("line %d [%s] illegal. skipped.", lineno, line.c_str());
        continue;
      }
      idf = atof(buf[1].c_str());
      idfMap_[buf[0]] = idf;
      idfSum += idf;

    }

    assert(lineno);
    idfAverage_ = idfSum / lineno;
    assert(idfAverage_ > 0.0);
  }
  void LoadStopWordDict(const string& filePath) {
    ifstream ifs(filePath.c_str());
    if (!ifs.is_open()) {
      LogFatal("open %s failed.", filePath.c_str());
    }
    string line ;
    while (getline(ifs, line)) {
      stopWords_.insert(line);
    }
    assert(stopWords_.size());
  }

  bool IsSingleWord(const string& str) const {
    Unicode unicode;
    TransCode::decode(str, unicode);
    if (unicode.size() == 1)
      return true;
    return false;
  }

  static bool Compare(const pair<string, double>& lhs, const pair<string, double>& rhs) {
    return lhs.second > rhs.second;
  }

  MixSegment segment_;
  unordered_map<string, double> idfMap_;
  double idfAverage_;

  unordered_set<string> stopWords_;
};
}

#endif


