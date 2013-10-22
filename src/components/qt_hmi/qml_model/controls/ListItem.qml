/**
 * @file Entry.qml
 * @brief Entry with icon and text for list.
 * Copyright (c) 2013, Ford Motor Company
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following
 * disclaimer in the documentation and/or other materials provided with the
 * distribution.
 *
 * Neither the name of the Ford Motor Company nor the names of its contributors
 * may be used to endorse or promote products derived from this software
 * without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

import QtQuick 2.0
import "../models/Constants.js" as Constants

Rectangle {
    id: main
    color: Constants.transparentColor

    property alias text: label.text
    property alias fontSize: label.font.pixelSize
    property alias icon: image.source

    Icon {
        id: image
        width: Constants.iconItemListSize
        height: Constants.iconItemListSize
        anchors.left: parent.left
        anchors.leftMargin: Constants.generalSpasing
        anchors.verticalCenter: parent.verticalCenter
        visible: source ? true : false
    }
    Text {
        id: label
        anchors.verticalCenter: parent.verticalCenter
        z: 50
        verticalAlignment: Text.AlignVCenter
        font.pixelSize: Constants.fontSize
        text: "Name Entry"
        anchors.left: image.right
        anchors.leftMargin: Constants.generalSpasing
        anchors.verticalCenterOffset: 0
        visible: text !== ""
        color: Constants.primaryColor
    }
}
