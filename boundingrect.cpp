/*
 * This file is a part of GDSGadgets.
 * boundingRect.cpp -- A source file which defines some api 
 *                     to get the bounding rect of GDS elements.
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

#include <QPolygon>
#include <QPainterPath>
#include <QTransform>
#include <limits>
#include "GDS/library.h"
#include "GDS/structures.h"
#include "GDS/boundary.h"
#include "GDS/path.h"
#include "GDS/sref.h"
#include "GDS/aref.h"

#include "boundingrect.h"

namespace GDS
{
	bool boundingRect(Structure* data, int& x, int& y, int& width, int& height)
	{
		if (data == nullptr)
			return false;

		if (data->size() <= 0)
			return false;

		int x1 = std::numeric_limits<int>::max();
		int y1 = std::numeric_limits<int>::max();
		int x2 = std::numeric_limits<int>::min();
		int y2 = std::numeric_limits<int>::min();
		bool success = false;
		for (size_t i = 0; i < data->size(); i++)
		{
			Element* tmp = data->get(i);
			if (tmp == nullptr)
				continue;
			int tx, ty, t_w, t_h;
			bool flag;
			switch (tmp->tag())
			{
			case BOUNDARY:
			{
				if (Boundary* node = dynamic_cast<Boundary*>(tmp))
				{
					flag = boundingRect(node, tx, ty, t_w, t_h);
				}
				break;
			}
			case PATH:
			{
				if (Path* node = dynamic_cast<Path*>(tmp))
				{
					flag = boundingRect(node, tx, ty, t_w, t_h);
				}
				break;
			}
			case SREF:
			{
				if (SRef* node = dynamic_cast<SRef*>(tmp))
				{
					flag = boundingRect(node, tx, ty, t_w, t_h);
				}
				break;
			}
			case AREF:
			{
				if (ARef* node = dynamic_cast<ARef*>(tmp))
				{
					flag = boundingRect(node, tx, ty, t_w, t_h);
				}
				break;
			}
			default:
				break;
			}
			if (flag)
			{
				success = true;
				x1 = (tx < x1 ? tx : x1);
				y1 = (ty < y1 ? ty : y1);
				x2 = ((tx + t_w) > x2 ? (tx + t_w) : x2);
				y2 = ((ty + t_h) > y2 ? (ty + t_h) : y2);
			}
		}
		x = x1;
		y = y1;
		width = x2 - x1;
		height = y2 - y1;

		return success;
	}

	bool boundingRect(Boundary* data, int& x, int& y, int& width, int& height)
	{
		if (data == nullptr)
			return false;

		std::vector<int> v_x, v_y;
		data->xy(v_x, v_y);
		QPolygon polygon;
		for (size_t i = 0; i < v_x.size() && i < v_y.size(); i++)
		{
			polygon << QPoint(v_x[i], v_y[i]);
		}

		QRect rect = polygon.boundingRect();
		x = rect.x();
		y = rect.y();
		width = rect.width();
		height = rect.height();

		return true;
	}

	bool boundingRect(Path* data, int& x, int& y, int& width, int& height)
	{
		if (data == nullptr)
			return false;

		std::vector<int> v_x, v_y;
		data->xy(v_x, v_y);
		if (v_x.size() != v_y.size())
			return false;
		for (size_t i = 0; i < v_x.size() - 1;)
		{
			if (v_x[i] == v_x[i + 1] && v_y[i] == v_y[i + 1])
			{
				v_x.erase(v_x.begin() + i + 1);
				v_y.erase(v_y.begin() + i + 1);
			}
			else
			{
				i++;
			}
		}
		if (v_x.size() < 2 || v_y.size() < 2)
			return false;
		int path_width = data->width();
		if (path_width < 0)
			path_width = -path_width;

		QPainterPath path;
		path.moveTo(v_x[0], v_y[0]);
		for (size_t i = 1; i < v_x.size() && i < v_y.size(); i++)
		{
			path.lineTo(v_x[i], v_y[i]);
		}
		QPainterPathStroker stroker;
		stroker.setJoinStyle(Qt::MiterJoin);
		stroker.setWidth(path_width);

		QRectF rect = stroker.createStroke(path).boundingRect();
		x = rect.x();
		y = rect.y();
		width = rect.width();
		height = rect.height();

		return true;
	}

	bool boundingRect(SRef* data, int& x, int& y, int& width, int& height)
	{
		if (data == nullptr)
			return false;
		std::string sname = data->structName();
		Library* lib = Library::getInstance();
		Structure* reference = lib->get(sname);
		if (reference == nullptr)
			return false;

		if (!boundingRect(reference, x, y, width, height))
			return false;

		QRect rect(x, y, width, height);
		int mag = data->mag();
		int angle = data->angle();
		QTransform transform;
		transform.translate(x + width / 2.0, y + height / 2.0);
		transform.scale(mag, mag);
		transform.rotate(angle);
		transform.translate(-(x + width / 2.0), -(y + height / 2.0));
		QRect rect2 = transform.mapRect(rect);

		x = rect2.x();
		y = rect2.y();
		width = rect2.width();
		height = rect2.height();

		return true;
	}

	bool boundingRect(ARef* data, int& x, int& y, int& width, int& height)
	{
		if (data == nullptr)
			return false;
		std::string sname = data->structName();
		Library* lib = Library::getInstance();
		Structure* reference = lib->get(sname);
		if (reference == nullptr)
			return false;

		std::vector<int> v_x, v_y;
		if (v_x.size() != 3 || v_y.size() != 3)
			return false;
		QPolygon polygon;
		for (size_t i = 0; i < 3; i++)
		{
			polygon << QPoint(v_x[i], v_y[i]);
		}
		QRect rect = polygon.boundingRect();

		x = rect.x();
		y = rect.y();
		width = rect.width();
		height = rect.height();

		return true;
	}



	
}