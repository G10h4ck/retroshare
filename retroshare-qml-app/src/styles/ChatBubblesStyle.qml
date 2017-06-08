import QtQuick 2.0
pragma Singleton

QtObject {

	property QtObject bubble: QtObject{
		//	Bubble measures
		readonly property int lMarginBubble: 10
		readonly property int rMarginBubble: 10
		readonly property int tMarginBubble: 5
		readonly property int bMarginBubble: 10
		readonly property int aditionalBubbleHeight: tMarginBubble * 2
		readonly property int aditionalBubbleWidth: 30
		readonly property real bubbleMaxWidth: 0.5 // % from parent

		// BubbleProps
		readonly property int radius: 5


		//	Colors
		readonly property string colorIncoming: "lightGreen"
		readonly property string colorOutgoing: "aliceblue"
		readonly property string colorSenderName: "cornflowerblue"
		readonly property string colorMessageTime: "grey"

	}


	property QtObject inferiorPanel: QtObject{
		// Panel globals
		readonly property int height: 40
		readonly property string backgroundColor: "transparent"
		readonly property string borderColor: "lightGrey"

		property QtObject msgComposer: QtObject{
			readonly property string placeHolder: "Send message..."

			property QtObject background: Rectangle {
				color: "transparent"
			}
		}


		// Button Icon
		property QtObject btnIcon: QtObject{
			readonly property int width: 30
			readonly property int height: 30

			readonly property string sendIconUrl: "/icons/send-message.svg"
			readonly property string attachIconUrl: "/icons/attach.svg"

		}




	}


}