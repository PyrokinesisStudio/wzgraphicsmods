/*
	Copyright 2010 Warzone 2100 Project

	This file is part of WMIT.

	WMIT is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	WMIT is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with WMIT.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef IGLTEXTUREMANAGER_HPP
#define IGLTEXTUREMANAGER_HPP

#include <QString>

#include "GLTexture.hpp"

class QString;
class IGLTextureManager
{
public:
	IGLTextureManager(){}
	virtual ~IGLTextureManager(){}
	virtual GLTexture createTexture(const QString& fileName) = 0;
	virtual void deleteTexture(GLuint id) = 0;
	virtual void deleteTexture(const QString& fileName) = 0;
	virtual void deleteAllTextures() = 0;
	virtual QString idToFilePath(GLuint id) = 0;
};
#endif // IGLTEXTUREMANAGER_HPP
