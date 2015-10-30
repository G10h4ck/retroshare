/*
 * This file is part of libretroshare.
 *
 * Copyright (C) 2016 Gioacchino Mazzurco <gio@eigenlab.org>
 *
 * libretroshare is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * libretroshare is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with libretroshare. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef PQIADDRESS_H
#define PQIADDRESS_H

#include <string>

/**
 * @brief Generic listen address representation class
 * Represents address in a protocol agnostic form so we can implement
 * pqi components for variuos protocols without altering the API.
 * It is common practice tu use URL to represent this kind of information.
 */
class pqiaddress
{
public:
	virtual std::string toStdString() = 0;
};

class pqiUrlAddress : public pqiaddress
{
public:
	std::string toStdString() { return value; }
	std::string value;
};

#endif // PQIADDRESS_H
