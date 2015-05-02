#ifndef DGDEBUG_HPP
#define DGDEBUG_HPP

//For those of us who can't use a debugger with Qt Creator.

#include <cassert>

#include <QDebug>

#undef NDEBUG

#define DEBUG_EMIT(val) qDebug() << val << '\n';
#define DEBUG_EMIT_TEST(cond,val) if(cond) {qDebug() << val << '\n';}
#define DEBUG_EMIT_LIST(list) for(const auto& val : list) {qDebug() << '\t' << val << '\n';}
#define DEBUG_ASSERT(test) assert(test);
#define DEBUG_ASSERT_TEST(cond,test) if(cond) assert(test);

#endif // DGDEBUG_HPP
