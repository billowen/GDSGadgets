/*
 * This file is a part of GDSGadgets
 * boundingrect.h -- A header file which declares the apis to get 
 *                   bouding rect of GDS elements.
 *
 * Copyright (c) 2015 Kangpeng Shao <billowen035@gmail.com>
 *
 * GDSII is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at you option) any later version.
 *
 * GDSII is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABLILTY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
 * for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with GDSII. If not, see <http://www.gnu.org/licenses/>.
 **/

#ifndef GDSBOUNDINGRECT_H
#define GDSBOUNDINGRECT_H

namespace GDS
{
	class Structure;
	class Boundary;
	class Path;
	class ARef;
	class SRef;

	bool boundingRect(Structure* data, int& x, int& y, int& width, int& height);
	bool boundingRect(Boundary* data, int& x, int& y, int& width, int& height);
	bool boundingRect(Path* data, int& x, int& y, int& width, int& height);
	bool boundingRect(ARef* data, int& x, int& y, int& width, int& height);
	bool boundingRect(SRef* data, int& x, int& y, int& width, int& height);
}
#endif // !GDSBOUNDINGRECT_H
