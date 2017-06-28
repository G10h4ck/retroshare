/*
 * RetroShare Android Autologin Service
 * Copyright (C) 2017  Gioacchino Mazzurco <gio@eigenlab.org>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

import QtQml 2.2
import org.retroshare.qml_components.LibresapiLocalClient 1.0
import "." //Needed for TokensManager singleton

QtObject
{
	id: notifyRoot

	property alias coreReady: coreWatcher.coreReady
	onCoreReadyChanged: if(coreReady) refreshUnread()

	Component.onCompleted:
	{
		addUriHandler("/networkstatus", networkStatusCb)

	}

	property var uriHandlersRegister: ({})
	property var pendingUriRegister: []
	function addUriHandler(path, fun) { uriHandlersRegister[path] = fun }
	function delUriHandler(path, fun) { delete uriHandlersRegister[path] }

	property AutologinManager coreWatcher: AutologinManager { id: coreWatcher }

	function refreshUnreadCallback(par)
	{
		console.log("notifyRoot.refreshUnreadCB()")
		var json = JSON.parse(par.response)
		TokensManager.registerToken(json.statetoken, refreshUnread)
		var convCnt = json.data.length
		if(convCnt > 0)
		{
			console.log("notifyRoot.refreshUnreadCB() got", json.data.length,
						"unread conversations")
			notificationsBridge.notify(
						qsTr("New message!"),
						qsTr("Unread messages in %1 %2").arg(convCnt).arg(
							convCnt > 1 ?
							qsTr("conversations") : qsTr("conversation")
							)
						)
		}
	}
	function refreshUnread()
	{
		console.log("notifyRoot.refreshUnread()")
		rsApi.request("/chat/unread_msgs", "", refreshUnreadCallback)
	}

	function networkStatusCb (uriStr)
	{

	}

	function handleIntentUri(uriStr)
	{
		console.log("handleIntentUri(uriStr)")

		if(!Array.isArray(uriStr.match(/:\/\/[a-zA-Z.-]*\//g)))
		{
			/* RetroShare GUI produces links without hostname and only two
			 * slashes after scheme causing the first piece of the path part
			 * being interpreted as host, this is awckard and should be fixed in
			 * the GUI, in the meantime we add a slash for easier parsing, in
			 * case there is no hostname and just two slashes, we might consider
			 * to use +hostname+ part for some trick in the future, for example
			 * it could help other application to recognize retroshare link by
			 * putting a domain name there that has no meaning for retroshare
			 */
			uriStr = uriStr.replace("://", ":///")
		}

		var uri = new UriJs.URI(uriStr)
		var hPath = uri.path() // no nesting ATM segmentCoded()
		console.log(hPath)

		if(typeof uriHandlersRegister[hPath] == "function")
		{
			console.log("handleIntentUri(uriStr)", "found handler for path",
						hPath, uriHandlersRegister[hPath])
			uriHandlersRegister[hPath](uriStr)
		}
	}
}
