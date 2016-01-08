/*
 * libretroshare/src/serialiser: rsdiscitems.h
 *
 * Serialiser for RetroShare.
 *
 * Copyright 2004-2008 by Robert Fernie.
 * Copyright (C) 2014 Gioacchino Mazzurco <gio@eigenlab.org>
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



#ifndef RS_DISC_ITEMS_H
#define RS_DISC_ITEMS_H

#include "serialiser/rsserial.h"
#include "serialiser/rstlvidset.h"
#include "serialiser/rstlvaddrs.h"
#include "serialiser/rsserviceids.h"
#include "serialiser/rstlvstring.h"

const uint8_t RS_PKT_SUBTYPE_DISC_PGP_LIST           = 0x01;
const uint8_t RS_PKT_SUBTYPE_DISC_PGP_CERT           = 0x02;
const uint8_t RS_PKT_SUBTYPE_DISC_CONTACT_deprecated = 0x03;
const uint8_t RS_PKT_SUBTYPE_DISC_SERVICES           = 0x04;
const uint8_t RS_PKT_SUBTYPE_DISC_CONTACT            = 0x05;
const uint8_t RS_PKT_SUBTYPE_DISC_CONTACT_URL        = 0x06;

class RsDiscItem: public RsItem
{
protected:
	RsDiscItem(uint8_t subtype) :
		RsItem(RS_PKT_VERSION_SERVICE, RS_SERVICE_TYPE_DISC, subtype) {}
};


#define DISC_PGP_LIST_MODE_NONE		0x00
#define DISC_PGP_LIST_MODE_FRIENDS	0x01
#define DISC_PGP_LIST_MODE_GETCERT	0x02

class RsDiscPgpListItem: public RsDiscItem
{
public:
	RsDiscPgpListItem() : RsDiscItem(RS_PKT_SUBTYPE_DISC_PGP_LIST)
	{ setPriorityLevel(QOS_PRIORITY_RS_DISC_PGP_LIST); }

	virtual ~RsDiscPgpListItem();

	virtual void clear();
	virtual std::ostream &print(std::ostream &out, uint16_t indent = 0);

	uint32_t mode;
	RsTlvPgpIdSet pgpIdSet;
};


class RsDiscPgpCertItem: public RsDiscItem
{
public:
	RsDiscPgpCertItem() : RsDiscItem(RS_PKT_SUBTYPE_DISC_PGP_CERT)
	{ setPriorityLevel(QOS_PRIORITY_RS_DISC_PGP_CERT); }

	virtual ~RsDiscPgpCertItem();

	virtual void clear();
	virtual std::ostream &print(std::ostream &out, uint16_t indent = 0);

	RsPgpId pgpId;
	std::string pgpCert;
};


class RsDiscContactItem: public RsDiscItem
{
public:
	RsDiscContactItem() : RsDiscItem(RS_PKT_SUBTYPE_DISC_CONTACT)
	{ setPriorityLevel(QOS_PRIORITY_RS_DISC_CONTACT); }

	virtual ~RsDiscContactItem();

	virtual void clear();
	virtual std::ostream &print(std::ostream &out, uint16_t indent = 0);

	RsPgpId pgpId;
	RsPeerId sslId;

	// COMMON
	std::string location;
	std::string version;

	uint32_t netMode; // Mandatory
	uint16_t vs_disc; // Mandatory
	uint16_t vs_dht; // Mandatory
	uint32_t lastContact;

	bool isHidden; // not serialised

	// HIDDEN.
	std::string hiddenAddr;
	uint16_t hiddenPort;

	// STANDARD.

	RsTlvIpAddress currentConnectAddress; // used to check!

	RsTlvIpAddress localAddrV4; // Mandatory
	RsTlvIpAddress extAddrV4; // Mandatory

	RsTlvIpAddress localAddrV6; // Mandatory
	RsTlvIpAddress extAddrV6; // Mandatory

	std::string dyndns;

	RsTlvIpAddrSet localAddrList;
	RsTlvIpAddrSet extAddrList;
};

/**
 * @brief The RsDiscContactUrlItem class
 * 2016/01/07 This class aims to obsolete RsDiscContactItem soon
 */
class RsDiscContactUrlItem: public RsDiscItem
{
public:
	RsDiscContactUrlItem() : RsDiscItem(RS_PKT_SUBTYPE_DISC_CONTACT_URL),
	  listenUrlSet(TLV_TYPE_STRINGSET)
	{ setPriorityLevel(QOS_PRIORITY_RS_DISC_CONTACT); }
	~RsDiscContactUrlItem() {}

	void clear();

	RsPgpId pgpId; /// Mandatory
	RsPeerId sslId; /// Mandatory
	std::string location; /// Mandatory
	std::string version; /// Mandatory
	uint32_t lastContact; /// Mandatory
	RsTlvStringSet listenUrlSet; /// Mandatory
};

#if 0
// TODO: 2016/01/01 Dead code?
class RsDiscServicesItem: public RsDiscItem
{
	public:

	RsDiscServicesItem()
        :RsDiscItem(RS_PKT_SUBTYPE_DISC_SERVICES)
	{ 
		setPriorityLevel(QOS_PRIORITY_RS_DISC_SERVICES); 
	}

virtual ~RsDiscServicesItem();

virtual  void clear();
virtual std::ostream &print(std::ostream &out, uint16_t indent = 0);


	std::string version;
	RsTlvServiceIdMap mServiceIdMap;
};
#endif


class RsDiscSerialiser: public RsSerialType
{
public:
	RsDiscSerialiser() :
		RsSerialType(RS_PKT_VERSION_SERVICE, RS_SERVICE_TYPE_DISC) {}
	virtual ~RsDiscSerialiser() {}

	virtual uint32_t size(RsItem *);
	virtual bool serialise (RsItem *item, void *data, uint32_t *size);
	virtual RsItem * deserialise(void *data, uint32_t *size);

private:
	virtual uint32_t sizePgpList(RsDiscPgpListItem *);
	virtual bool serialisePgpList(RsDiscPgpListItem *item, void *data, uint32_t *size);
	virtual RsDiscPgpListItem *deserialisePgpList(void *data, uint32_t *size);

	virtual uint32_t sizePgpCert(RsDiscPgpCertItem *);
	virtual bool serialisePgpCert(RsDiscPgpCertItem *item, void *data, uint32_t *size);
	virtual RsDiscPgpCertItem *deserialisePgpCert(void *data, uint32_t *size);

	virtual uint32_t sizeContact(RsDiscContactItem *);
	virtual bool serialiseContact(RsDiscContactItem *item, void *data, uint32_t *size);
	virtual RsDiscContactItem *deserialiseContact(void *data, uint32_t *size);

	uint32_t sizeContactUrl(const RsDiscContactUrlItem& item);
	bool serialiseContactUrl(const RsDiscContactUrlItem& item, uint8_t data[],
	                         uint32_t& size);
	bool deserialiseContactUrl(RsDiscContactUrlItem& item, const uint8_t data[],
	                           uint32_t &size);

#if 0
// TODO: 2016/01/04 Dead code?
virtual uint32_t    sizeServices(RsDiscServicesItem *);
virtual bool        serialiseServices(RsDiscServicesItem *item, void *data, uint32_t *size);
virtual RsDiscServicesItem *deserialiseServices(void *data, uint32_t *size);

#endif
};


#endif // RS_DISC_ITEMS_H
