#include <exception>
#include "caf/all.hpp"
#include "caf/mixin/actor_widget.hpp"

CAF_PUSH_WARNINGS
#include <QGraphicsObject>
#include <QWidget>
#include <QLineEdit>
#include <QTextEdit>
CAF_POP_WARNINGS

#ifndef ACTORDEMO_ACTORGRAPHICSOBJECT_H
#define ACTORDEMO_ACTORGRAPHICSOBJECT_H


class ActorGraphicsObject : public caf::mixin::actor_widget<QGraphicsObject> {

};


#endif //ACTORDEMO_ACTORGRAPHICSOBJECT_H
