/**************************************************************************
 *
 * Copyright 2015 Canonical Ltd.
 * Copyright 2015 Carlos J Mazieri <carlos.mazieri@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * File: networklistworker.cpp
 * Date: 06/12/2015
 */

#include "networklistworker.h"
#include "locationitemdiriterator.h"
#include "locationurl.h"

NetworkListWorker::NetworkListWorker(LocationItemDirIterator * dirIterator,
                                     DirItemInfo * mainItemInfo, const DirItemInfo *parent) :
    DirListWorker(dirIterator->path(),
                  dirIterator->filters(),
                  dirIterator->flags() == QDirIterator::Subdirectories ? true : false),
    m_dirIterator(dirIterator),
    m_mainItemInfo(mainItemInfo),
    m_parent(parent)
{
     mLoaderType =  NetworkLoader;
}


NetworkListWorker::~NetworkListWorker()
{
    delete m_dirIterator;
    delete m_mainItemInfo;
}


DirItemInfoList NetworkListWorker::getNetworkContent()
{
     DirItemInfoList netContent;
     m_dirIterator->load();
     bool is_parent_of_smb_url = m_parent != 0 && m_parent->urlPath().startsWith(LocationUrl::SmbURL);
     while (m_dirIterator->hasNext())
     {
         m_mainItemInfo->setFile(m_dirIterator->next());
         if (is_parent_of_smb_url)
         {
             setSmbItemAttributes();
         }
         netContent.append(*m_mainItemInfo);
     }
     return netContent;
}

/*!
 * \brief NetworkListWorker::setSmbItemAttributes()
 *
 *  This original implementation regards only to Samba (smb:// protocol),
 *  it will not hurt other protocols implementation.
 */
void NetworkListWorker::setSmbItemAttributes()
{    
    if (m_parent->isHost())      { m_mainItemInfo->setAsShare(); }
    else
    if (m_parent->isWorkGroup()) { m_mainItemInfo->setAsHost(); }
}
