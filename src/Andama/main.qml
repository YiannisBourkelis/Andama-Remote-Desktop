import QtQuick 2.3 //2.5
import QtQuick.Window 2.2
import Qt.labs.settings 1.0
import QtQuick.Controls 1.2 //1.4

import engine.andama.com 1.0

Window {
    id:mainWindow
    visible: true

    height: engine.showRemote ? Screen.desktopAvailableHeight : 380
    width:  engine.showRemote ? Screen.desktopAvailableWidth : 728
    property real scaleFactor: Math.min( height, width) / 380

    /*Settings {
        property alias x: mainWindow.x
        property alias y: mainWindow.y
        property alias width: mainWindow.width
        property alias height: mainWindow.height
    }*/


    Image{
        anchors.top: parent.top
        anchors.bottom: status.top
        width:parent.width
        source : "qrc:/images/images/gradient_main.png"

        Column{
            width: parent.width*0.5
            height: parent.height
            anchors.left: parent.left

            Text{
                text:"Local computer"
                color:"white"
                width: parent.width
                height: parent.height * 0.2
                minimumPixelSize: 8
                fontSizeMode: Text.Fit
                horizontalAlignment: Text.AlignHCenter

            }
            Text{
                text:"With the following ID and Password you can \nremotely connect to this computer."
                color:"grey"
                width: parent.width
                height: parent.height * 0.2
                minimumPixelSize: 8
                font.pixelSize: 32
                fontSizeMode: Text.Fit
                horizontalAlignment: Text.AlignJustify

            }
            Row{
                width: parent.width
                height: parent.height * 0.2
                Text{
                    text:"Your ID :"
                    color:"white"
                    width: parent.width* 0.3
                    height: parent.height
                    minimumPixelSize: 8
                    fontSizeMode: Text.Fit
                    horizontalAlignment: Text.AlignRight

                }
                TextInput{
                    id:yourId
                    text:engine.id
                    readOnly: true
                    width: parent.width* 0.7
                    height: parent.height
                    color:"white"

                }
            }
            Row{
                width: parent.width
                height: parent.height * 0.2
                Text{
                    text:"Your Password :"
                    color:"white"
                    width: parent.width* 0.3
                    height: parent.height
                    minimumPixelSize: 8
                    fontSizeMode: Text.Fit
                    horizontalAlignment: Text.AlignRight

                }
                TextInput{
                    id:yourPassword
                    text:engine.password
                    readOnly: true
                    width: parent.width* 0.7
                    height: parent.height
                    color:"white"
                }
            }

        }
        Column{
            id:remotePanel
            width: parent.width*0.5
            height: parent.height
            anchors.right: parent.right


            Text{
                text:"Remote computer"
                color:"white"
                width: parent.width
                height: parent.height * 0.2
                minimumPixelSize: 8
                fontSizeMode: Text.Fit
                horizontalAlignment: Text.AlignHCenter
            }
            Text{
                text: "Enter the ID and Password of the\n remote computer you want to connect."
                color:"grey"
                width: parent.width
                height: parent.height * 0.2
                font.pixelSize: 32
                minimumPixelSize: 8
                fontSizeMode: Text.Fit
                horizontalAlignment: Text.AlignHCenter

                Text{
                    id:remoteErrorId
                    anchors.fill: parent
                    text : engine.remoteErrorIdText
                    visible : text.length
                    color:"red"
                }
                Text{
                    id:remoteErrorPassword
                    anchors.fill: parent
                    text : engine.remoteErrorPasswordText
                    visible : text.length
                    color:"red"
                }

            }
            TextField{
                id:remoteId
                placeholderText: "Remote ID"
                width: parent.width
                height: parent.height * 0.2
            }
            TextField{
                id:remotePassword
                placeholderText: "Remote password"
                width: parent.width
                height: parent.height * 0.2
            }
            Button{
                id:connectButton
                text:"Connect"
                width: parent.width
                height: parent.height * 0.2
                enabled: (remoteId.length > 0) && (remotePassword.length > 0)
                onClicked: engine.connectToRemote(remoteId.text, remotePassword.text);

            }
        }
    }
    Image{
        id:status
        anchors.bottom: ad.top
        height:25 * mainWindow.scaleFactor
        width: parent.width
        source : (engine.statusErrorLevel == Engine.ERROR) ? "qrc:/images/images/status_red.png" :
                 (engine.statusErrorLevel == Engine.WARNING) ? "qrc:/images/images/status_orange.png" :
                 "qrc:/images/images/status_green.png"
        Text{
            id:statusText
            text: engine.statusText
            color:"white"
            anchors.fill: parent
            minimumPixelSize: 8
            fontSizeMode: Text.Fit
            horizontalAlignment: Text.AlignHCenter

        }
    }
    Image{
        id:ad
        anchors.bottom: parent.bottom
        height:90 * mainWindow.scaleFactor
        width: parent.width
        source : "qrc:/images/images/advertise_here.png"
    }

    Image{
        id:remoteScreen
        source:"image://remotescreen/yellow"
        fillMode: Image.PreserveAspectFit
        visible : engine.showRemote
        anchors.fill: parent
        cache:false

        onVisibleChanged: {
            if (visible)
                remoteScreen.focus = true;
        }

        function notifyKey(key, pressedOrReleaseOrElse){
            console.log("sendKeyboard:"+key.key);
            engine.sendKeyboard(key.nativeScanCode, key.key, key.isAutoRepeat, key.count, key.modifiers,pressedOrReleaseOrElse);
        }
        Keys.onPressed: remoteScreen.notifyKey( event,1)
        Keys.onReleased: remoteScreen.notifyKey( event, 2)
        focus: true

        MouseArea{

            function notifyMouse( mouseEventType){
                //FIXME : always send LeftButton
                var pressedButtons2 = ((pressedButtons && Qt.MiddleButton) == Qt.MidButton) ?
                            Qt.LeftButton : ((pressedButtons && Qt.RightButton) == Qt.RightButton) ? Qt.RightButton : Qt.LeftButton
                console.log("mouseX:"+mouseX+"_mouseY:"+mouseY+"_PressedButtons:"+pressedButtons+"_Type:"+mouseEventType)
                engine.sendMouse( mouseX, mouseY, pressedButtons2, mouseEventType,0, 0, 0);
            }

            function notifyWheel(wheel){
                var wheelDelta = wheel.angleDelta.x + wheel.angleDelta.y;
                var wheelDeltaSign = wheelDelta < 0 ? 1 : 2;
                var wheelOrientation = wheel.angleDelta.y > 0 ? 1 : 2;
                engine.sendMouse(
                    0, 0, 0, 5, wheelDelta, wheelDeltaSign , wheelOrientation);
            }
            acceptedButtons: Qt.AllButtons
            hoverEnabled:true
            anchors.fill: parent

            onPositionChanged: notifyMouse(1);
            onPressed: notifyMouse(2);
            onReleased: notifyMouse(3);
            onDoubleClicked: notifyMouse(4);
            onWheel: notifyWheel( wheel)

        }


    }

    Connections{
        target:engine
        onNewFrameAvailable:{
            remoteScreen.source="image://remotescreen/"+frameNumber;
        }
    }


}
