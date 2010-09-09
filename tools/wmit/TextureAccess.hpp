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

#ifndef TEXTUREACCESS_HPP
#define TEXTUREACCESS_HPP

#include <string>

#include <QString>

#include "GLTexture.hpp"
#include "IGLTextureManager.hpp"

class TextureAccess
{
	IGLTextureManager * m_texMan;
protected:
	TextureAccess():m_texMan(NULL){}
	TextureAccess(IGLTextureManager * manager):m_texMan(manager){}
	virtual ~TextureAccess(){}

	virtual void setTextureManager(IGLTextureManager * manager){m_texMan=manager;}

	std::string idToFilePath(GLuint id)
	{
		if(m_texMan!=NULL){return m_texMan->idToFilePath(id).toStdString();}
		else{return std::string();}
	}

	bool hasTextureManager()const{return m_texMan!=NULL;}

	GLTexture createTexture(std::string fileName) const
	{
		if(m_texMan!=NULL){return m_texMan->createTexture(QString().fromStdString(fileName));}
		else{return GLTexture();}
	}

	GLTexture createTexture(const QString& fileName) const
	{
		if(m_texMan!=NULL){return m_texMan->createTexture(fileName);}
		else{return GLTexture();}
	}

	void deleteTexture(GLuint id) const {if(m_texMan!=NULL){m_texMan->deleteTexture(id);}}
};

#endif // TEXTUREACCESS_HPP
