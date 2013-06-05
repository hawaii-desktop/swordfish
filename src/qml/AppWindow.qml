
import QtQuick 2.1
import QtQuick.Controls 1.0
import QtQuick.Layouts 1.0
import Swordfish 0.1

ApplicationWindow {
	title: "Swordfish"
	width: 640
	height: 480
	minimumWidth: 400
	minimumHeight: 400
	color: palette.window
	toolBar: ToolBar {
		id: toolBar

		RowLayout {
			spacing: 2
			anchors.verticalCenter: parent.verticalCenter

			ToolButton {
				action: actionBack
			}

			ToolButton {
				action: actionForward
			}
		}
	}

	SystemPalette {
		id: palette
	}

	Action {
		id: actionBack
		text: qsTr("&Back")
		tooltip: qsTr("Back")
		shortcut: "Alt+Left"
		iconName: "go-previous"
		enabled: false
	}

	Action {
		id: actionForward
		text: qsTr("&Forward")
		tooltip: qsTr("Forward")
		shortcut: "Alt+Right"
		iconName: "go-next"
		enabled: false
	}

	SplitView {
		anchors.fill: parent

		PlacesView {
			id: placesView
			Layout.minimumWidth: 200
			Layout.maximumWidth: 250
		}

		FolderView {
			id: folderView
		}
	}
}
