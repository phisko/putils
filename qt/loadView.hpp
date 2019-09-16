#pragma once

#include <QWidget>
#include <QFile>
#include <QtUiTools/QtUiTools>
#include "meta/for_each.hpp"
#include "meta/type.hpp"

namespace putils {
    namespace qt {
        template<typename View>
        QWidget * loadView(const std::string & file, View & view) {
            QFile f(file);
            f.open(QFile::ReadOnly);
            auto ret = QUiLoader().load(&f);
            f.close();

            pmeta::tuple_for_each(View::get_attributes().getKeyValues(),
                                  [ret, &view](auto && pair) {
                                      auto & attr = view.*(pair.second);
                                      std::string name(pair.first);
                                      attr = ret->findChild<pmeta_typeof(attr)>(name);
                                  }
            );

            return ret;
        }
    }
}
