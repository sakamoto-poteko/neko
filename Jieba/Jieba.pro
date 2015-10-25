#-------------------------------------------------
#
# Project created by QtCreator 2015-10-24T22:48:07
#
#-------------------------------------------------

QT       -= core gui

TARGET = Jieba
TEMPLATE = lib
CONFIG += staticlib


unix {
    target.path = /usr/lib
    INSTALLS += target
}

HEADERS += \
    src/DictTrie.hpp \
    src/FullSegment.hpp \
    src/HMMModel.hpp \
    src/HMMSegment.hpp \
    src/Jieba.hpp \
    src/KeywordExtractor.hpp \
    src/LevelSegment.hpp \
    src/MixSegment.hpp \
    src/MPSegment.hpp \
    src/PosTagger.hpp \
    src/PreFilter.hpp \
    src/QuerySegment.hpp \
    src/SegmentBase.hpp \
    src/TransCode.hpp \
    src/Trie.hpp \
    src/limonp/ArgvContext.hpp \
    src/limonp/BlockingQueue.hpp \
    src/limonp/BoundedQueue.hpp \
    src/limonp/CastFloat.hpp \
    src/limonp/Colors.hpp \
    src/limonp/Condition.hpp \
    src/limonp/Config.hpp \
    src/limonp/HandyMacro.hpp \
    src/limonp/InitOnOff.hpp \
    src/limonp/LocalVector.hpp \
    src/limonp/Logger.hpp \
    src/limonp/Md5.hpp \
    src/limonp/MutexLock.hpp \
    src/limonp/MysqlClient.hpp \
    src/limonp/NonCopyable.hpp \
    src/limonp/StdExtension.hpp \
    src/limonp/StringUtil.hpp \
    src/limonp/Thread.hpp \
    src/limonp/ThreadPool.hpp
