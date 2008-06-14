#ifndef RETROSHARE_GUI_INTERFACE_H
#define RETROSHARE_GUI_INTERFACE_H

/*
 * "$Id: rsiface.h,v 1.9 2007-04-21 19:08:51 rmf24 Exp $"
 *
 * RetroShare C++ Interface.
 *
 * Copyright 2004-2006 by Robert Fernie.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License Version 2 as published by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
 * USA.
 *
 * Please report all bugs and problems to "retroshare@lunamutt.com".
 *
 */


#include "rstypes.h"

#include <map>

class NotifyBase;
class RsIface;
class RsControl;
//class RsServer;
class RsInit;
class Expression;

/* declare single RsIface for everyone to use! */

extern RsIface   *rsiface;
extern RsControl *rsicontrol;

/* RsInit -> Configuration Parameters for RetroShare Startup
 */

RsInit *InitRsConfig();
/* extract various options for GUI */
const char   *RsConfigDirectory(RsInit *config);
bool    RsConfigStartMinimised(RsInit *config);
void    CleanupRsConfig(RsInit *);


// Called First... (handles comandline options) 
int InitRetroShare(int argc, char **argv, RsInit *config);

// This Functions are used for Login.
bool ValidateCertificate(RsInit *config, std::string &userName);
bool ValidateTrustedUser(RsInit *config, std::string fname, std::string &userName);
bool LoadPassword(RsInit *config, std::string passwd);
bool RsGenerateCertificate(RsInit *config, std::string name, std::string org,
                        std::string loc, std::string country, std::string passwd, std::string &errString);

/* Auto Login Fns */
bool  RsTryAutoLogin(RsInit *config);
bool  RsStoreAutoLogin(RsInit *config);
bool  RsClearAutoLogin(std::string basedir);

// Handle actual Login.
int LoadCertificates(RsInit *config, bool autoLoginNT);

RsIface   *createRsIface  (NotifyBase &notify);
RsControl *createRsControl(RsIface &iface, NotifyBase &notify);


class RsIface /* The Main Interface Class - create a single one! */
{
public:
	RsIface(NotifyBase &callback)
	:cb(callback) { return; }
	virtual ~RsIface() { return; }

/****************************************/

	/* Stubs for Very Important Fns -> Locking Functions */
virtual	void lockData() = 0;
virtual	void unlockData() = 0;

	const std::list<FileTransferInfo> &getTransferList()
		{ return mTransferList; }

	const std::list<PersonInfo> &getRemoteDirectoryList()
		{ return mRemoteDirList; }

	const std::list<PersonInfo> &getLocalDirectoryList()
		{ return mLocalDirList; }

	const PersonInfo *getPerson(std::string id);
	const DirInfo *getDirectory(std::string id, std::string path);

	const std::map<RsChanId, ChannelInfo> &getChannels()
		{ return mChannelMap; }

	const std::map<RsChanId, ChannelInfo> &getOurChannels()
		{ return mChannelOwnMap; }

	//const MessageInfo *getChannelMsg(std::string chId, std::string mId);

	//std::list<ChatInfo> getChatNew()
	//	{ 
	//		std::list<ChatInfo> newList = mChatList; 
	//		mChatList.clear(); 
	//		return newList;
	//	}

	const std::list<FileInfo> &getRecommendList()
		{ return mRecommendList; }

	const RsConfig &getConfig()
		{ return mConfig; }
/****************************************/


	/* Flags to indicate used or not */
	enum DataFlags
	{
		Neighbour = 0,
		Friend = 1,
		DirLocal = 2,  /* Not Used - QModel instead */
		DirRemote = 3, /* Not Used - QModel instead */
		Transfer = 4,
		Message = 5,
		Channel = 6,
		Chat = 7,
		Recommend = 8,
		Config = 9,
		NumOfFlags = 10
	};


	/* 
	 * Operations for flags
	 */

bool	setChanged(DataFlags set); /* set to true */
bool	getChanged(DataFlags set); /* leaves it */
bool	hasChanged(DataFlags set); /* resets it */

	private:

	/* Internal Fn for getting the Directory Entry */
	PersonInfo *getPersonMod(std::string id);
	DirInfo *getDirectoryMod(std::string id, std::string path);

void	fillLists(); /* create some dummy data to display */

		/* Internals */
	std::list<PersonInfo>    mRemoteDirList;
	std::list<PersonInfo>    mLocalDirList;
	std::list<FileTransferInfo> mTransferList;
	//std::list<MessageInfo>   mMessageList;
	std::map<RsChanId, ChannelInfo> mChannelMap;
	std::map<RsChanId, ChannelInfo> mChannelOwnMap;
	//std::list<ChatInfo>      mChatList;
	std::list<FileInfo>      mRecommendList;

	bool mChanged[NumOfFlags];

	RsConfig mConfig;

	NotifyBase &cb;

	/* Classes which can update the Lists! */
	friend class RsControl;
	friend class RsServer; 
};


class RsControl /* The Main Interface Class - for controlling the server */
{
	public:

	RsControl(RsIface &i, NotifyBase &callback)
	:cb(callback), rsIface(i) { return; }

virtual ~RsControl() { return; }

	/* Real Startup Fn */
virtual int StartupRetroShare(RsInit *config) = 0;

/****************************************/
	/* Directory Actions */
virtual int RequestDirDetails(std::string uid, std::string path, DirDetails &details) = 0;
virtual int RequestDirDetails(void *ref, DirDetails &details, uint32_t flags) = 0;

virtual bool ConvertSharedFilePath(std::string path, std::string &fullpath) = 0;
virtual void ForceDirectoryCheck() = 0;
virtual bool InDirectoryCheck() = 0;

/****************************************/
	/* Search Actions */
virtual int SearchKeywords(std::list<std::string> keywords, std::list<FileDetail> &results) = 0;
virtual int SearchBoolExp(Expression *exp, std::list<FileDetail> &results) = 0;

/****************************************/
	/* Actions For Upload/Download */

//virtual int FileDelete(std::string uId, std::string fname)    = 0;
//virtual int FileMove(std::string uId, std::string src, std::string dest) = 0;

virtual	int FileRecommend(std::string fname, std::string hash, int size) = 0;
virtual int FileRequest(std::string fname, std::string hash, uint32_t size, std::string dest) = 0;
virtual int FileCancel(std::string fname, std::string hash, uint32_t size) = 0;

// Transfer control.
virtual int FileClearCompleted()                              = 0;
virtual	int FileSetBandwidthTotals(float outkB, float inkB)   = 0;

/****************************************/
	/* Message Items */
//virtual	int MessageSend(MessageInfo &info)                 = 0;
//virtual int MessageDelete(std::string mid)                 = 0;
//virtual int MessageRead(std::string mid)                   = 0;

	/* Channel Items */
virtual	int ChannelCreateNew(ChannelInfo &info)            = 0;
virtual	int ChannelSendMsg(ChannelInfo &info)              = 0;

/****************************************/
	/* Chat */
//virtual	int 	ChatSend(ChatInfo &ci)                     = 0;

/****************************************/

	/* Flagging Persons / Channels / Files in or out of a set (CheckLists) */
virtual int 	SetInChat(std::string id, bool in) = 0;		/* friend : chat msgs */
virtual int 	SetInMsg(std::string id, bool in)  = 0;		/* friend : msg receipients */
virtual int 	SetInBroadcast(std::string id, bool in) = 0;	/* channel : channel broadcast */
virtual int 	SetInSubscribe(std::string id, bool in) = 0;	/* channel : subscribed channels */
virtual int 	SetInRecommend(std::string id, bool in) = 0;	/* file : recommended file */
virtual int 	ClearInChat() = 0;
virtual int 	ClearInMsg() = 0;
virtual int 	ClearInBroadcast() = 0;
virtual int 	ClearInSubscribe() = 0;
virtual int 	ClearInRecommend() = 0;

virtual bool 	IsInChat(std::string id) = 0;		/* friend : chat msgs */
virtual bool 	IsInMsg(std::string id) = 0;		/* friend : msg recpts*/

/****************************************/
        /* RsIface Networking */
//virtual int     NetworkDHTActive(bool active)  = 0;
//virtual int     NetworkUPnPActive(bool active) = 0;

/****************************************/
	/* Config */
virtual	int 	ConfigAddSharedDir( std::string dir )      = 0;
virtual	int 	ConfigRemoveSharedDir( std::string dir )   = 0;
virtual	int 	ConfigSetIncomingDir( std::string dir )    = 0;

virtual int     ConfigSetDataRates( int total, int indiv ) = 0;
virtual int     ConfigGetDataRates( float &inKb, float &outKb) = 0;
virtual	int 	ConfigSetBootPrompt( bool on ) = 0;
virtual void    ConfigFinalSave( ) 			   = 0;
virtual void 	rsGlobalShutDown( )			   = 0;

/****************************************/

	NotifyBase &getNotify() { return cb; }
	RsIface    &getIface()  { return rsIface; }

	private:
	NotifyBase &cb;
	RsIface    &rsIface;
};


/********************** Overload this Class for callback *****************/


class NotifyBase
{
	public:
	NotifyBase() { return; }
	virtual ~NotifyBase() { return; }
	virtual void notifyListPreChange(int list, int type) { (void) list; (void) type; return; }
	virtual void notifyListChange(int list, int type) { (void) list; (void) type; return; }
	virtual void notifyErrorMsg(int list, int sev, std::string msg) { (void) list; (void) sev; (void) msg; return; }
	virtual void notifyChat() { return; }
};

const int NOTIFY_LIST_NEIGHBOURS = 1;
const int NOTIFY_LIST_FRIENDS = 2;
const int NOTIFY_LIST_DIRLIST = 3;
const int NOTIFY_LIST_SEARCHLIST = 4;
const int NOTIFY_LIST_MESSAGELIST = 5;
const int NOTIFY_LIST_CHANNELLIST = 6;
const int NOTIFY_LIST_TRANSFERLIST = 7;

const int NOTIFY_TYPE_SAME   = 0x01;
const int NOTIFY_TYPE_MOD    = 0x02; /* general purpose, check all */
const int NOTIFY_TYPE_ADD    = 0x04; /* flagged additions */
const int NOTIFY_TYPE_DEL    = 0x08; /* flagged deletions */



#endif
