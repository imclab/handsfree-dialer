/*
 * hfdialer - Hands Free Voice Call Manager
 * Copyright (c) 2012, Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 */

#ifndef COMMON_H
#define COMMON_H

class QString;

#ifdef VERBOSE 
#include <QDebug>
#define TRACE qDebug() << "[" << __FILE__ << "]" << __func__ << "():" << __LINE__;
#else
#define TRACE
#endif

QString stripLineID(QString lineid);

#endif // COMMON_H

/* Local Variables:      */
/* mode:c++              */
/* c-basic-offset:4      */
/* indent-tabs-mode: nil */
/* End:                  */
