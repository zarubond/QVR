import QtQuick 2.4
import QtQuick.Window 2.2
import Zarubond 1.0 as Zarubond

Zarubond.Item {
    id: root;
    width: 1920;
    height: 1080;
    gui: DistortionGrid{
        parent: root;
        visible: true;
    }
}
