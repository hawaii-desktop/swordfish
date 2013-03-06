/*
   Copyright (C) 2008 Norbert Frese <nf2 at scheinwelt at>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License (LGPL) as published by the Free Software Foundation;
   either version 2 of the License, or (at your option) any later
   version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/


#ifndef KGIOWRAPPEDOBJECTUTILS_P_H_
#define KGIOWRAPPEDOBJECTUTILS_P_H_

#include "kgio-file.h"
#include "kgio-mount.h"
#include "kgio-loadableicon.h"
#include "kgio-fileicon.h"
#include "kgio-volumemonitor.h"

namespace Kommodity {
namespace GIO {
    
class WO
{
public:
    
    template <typename T, typename G>
    static void set(T * t, G * g)
    {
        t->setObjectPtr(g);
    }

    template <typename T, typename G>
    static G * get(const T * t)
    {
        return (G *)t->getObjectPtr();
    }
    
    /////
    
    static void setGFile(File * file, GFile * gFile)
    {
        file->setObjectPtr(gFile);
    }
        
    static GFile * getGFile(const File * file)
    {
        return (GFile*)file->getObjectPtr();
    }
    
    /////
    
    static void setGInputStream(InputStream * inputStream, GInputStream * gInputStream)
    {
        inputStream->setObjectPtr(gInputStream);
    }
        
    static GInputStream * getGInputStream(const InputStream * inputStream)
    {
        return (GInputStream*)inputStream->getObjectPtr();
    }
    
    /////

    static void setGOutputStream(OutputStream * outputStream, GOutputStream * gOutputStream)
    {
        outputStream->setObjectPtr(gOutputStream);
    }
        
    static GOutputStream * getGOutputStream(const OutputStream * outputStream)
    {
        return (GOutputStream*)outputStream->getObjectPtr();
    }    
    
    /////
    
    static void setGFileInputStream(FileInputStream * fileInputStream, GFileInputStream * gFileInputStream)
    {
        fileInputStream->setObjectPtr(gFileInputStream);
    }
        
    static GFileInputStream * getGFileInputStream(const FileInputStream * fileInputStream)
    {
        return (GFileInputStream*)fileInputStream->getObjectPtr();
    }
    
    /////
    
    static void setGFileOutputStream(FileOutputStream * fileOutputStream, GFileOutputStream * gFileOutputStream)
    {
        fileOutputStream->setObjectPtr(gFileOutputStream);
    }
        
    static GFileOutputStream * getGFileOutputStream(const FileOutputStream * fileOutputStream)
    {
        return (GFileOutputStream*)fileOutputStream->getObjectPtr();
    }    
    
    /////
    
    static void setGCancellable(Cancellable * cancellable, GCancellable * gCancellable)
    {
        cancellable->setObjectPtr(gCancellable);
    }

    static GCancellable * getGCancellable(const Cancellable * cancellable)
    {
        if (!cancellable) return 0;
        return (GCancellable*)cancellable->getObjectPtr();
    }

    /////
    
    static void setGSeekable(Seekable * seekable, GSeekable * gSeekable)
    {
        seekable->setObjectPtr(gSeekable);
    }

    static GSeekable * getGSeekable(const Seekable * seekable)
    {
        return (GSeekable*)seekable->getObjectPtr();
    }
    
    /////
    
    static void setGFileInfo(FileInfo * fileInfo, GFileInfo * gFileInfo)
    {
        fileInfo->setObjectPtr(gFileInfo);
    }
        
    static GFileInfo * getGFileInfo(const FileInfo * fileInfo)
    {
        return (GFileInfo*)fileInfo->getObjectPtr();
    }    

    /////
    
    static void setGMount(Mount * mount, GMount * gMount);
//    {
//        mount->setObjectPtr(gMount);
//    }
        
    static GMount * getGMount(const Mount * mount);
//    {
//        return (GMount*)mount->getObjectPtr();
//    }    

    /////
    
    static void setGDrive(Drive * drive, GDrive * gDrive);
//    {
//        drive->setObjectPtr(gDrive);
//    }
        
    static GDrive * getGDrive(const Drive * drive);
//    {
//        return (GDrive*)drive->getObjectPtr();
//    }        

    /////
    
    static void setGVolume(Volume * volume, GVolume * gVolume);
//    {
//        volume->setObjectPtr(gVolume);
//    }
        
    static GVolume * getGVolume(const Volume * volume);
//    {
//        return (GVolume*)volume->getObjectPtr();
//    }        
    
    /////
    
    static void setGVolumeMonitor(VolumeMonitor * volumeMonitor, GVolumeMonitor * gVolumeMonitor);
//    {
//        volumeMonitor->setObjectPtr(gVolumeMonitor);
//    }
        
    static GVolumeMonitor * getGVolumeMonitor(const VolumeMonitor * volumeMonitor);
//    {
//        return (GVolumeMonitor*)volumeMonitor->getObjectPtr();
//    }      
    
    /////
    
    static void setGIcon(Icon * icon, GIcon * gIcon)
    {
        icon->setObjectPtr(gIcon);
    }
        
    static GIcon * getGIcon(const Icon * icon)
    {
        return (GIcon*)icon->getObjectPtr();
    }    
    
    /////
    
    static void setGLoadableIcon(LoadableIcon * loadableIcon, GLoadableIcon * gLoadableIcon)
    {
        loadableIcon->setObjectPtr(gLoadableIcon);
    }
        
    static GLoadableIcon * getGLoadableIcon(const LoadableIcon * loadableIcon)
    {
        return (GLoadableIcon*)loadableIcon->getObjectPtr();
    }        
    
    /////
    
    static void setGFileIcon(FileIcon * fileIcon, GFileIcon * gFileIcon)
    {
        fileIcon->setObjectPtr(gFileIcon);
    }
        
    static GFileIcon * getGFileIcon(const FileIcon * fileIcon)
    {
        return (GFileIcon*)fileIcon->getObjectPtr();
    }       
    
    /////
    
    static void setGAppInfo(AppInfo * appInfo, GAppInfo * gAppInfo)
    {
        appInfo->setObjectPtr(gAppInfo);
    }
        
    static GAppInfo * getGAppInfo(const AppInfo * appInfo)
    {
        return (GAppInfo*)appInfo->getObjectPtr();
    }    
    
    /////
    
    static void setGAppLaunchContext(AppLaunchContext * appLaunchContext, GAppLaunchContext * gAppLaunchContext)
    {
        appLaunchContext->setObjectPtr(gAppLaunchContext);
    }
        
    static GAppLaunchContext * getGAppLaunchContext(const AppLaunchContext * appLaunchContext)
    {
        return (GAppLaunchContext*)appLaunchContext->getObjectPtr();
    }     
    
    /////
    
    static void setGMountOperation(MountOperation * mountOperation, GMountOperation * gMountOperation)
    {
        mountOperation->setObjectPtr(gMountOperation);
    }
        
    static GMountOperation * getGMountOperation(const MountOperation * mountOperation)
    {
        return (GMountOperation*)mountOperation->getObjectPtr();
    }    
    
    
    /////
    
    static void setGFileEnumerator(FileEnumerator * fileEnumerator, GFileEnumerator * gFileEnumerator)
    {
        fileEnumerator->setObjectPtr(gFileEnumerator);
    }
        
    static GFileEnumerator * getGFileEnumerator(const FileEnumerator * fileEnumerator)
    {
        return (GFileEnumerator*)fileEnumerator->getObjectPtr();
    }    
    
    /////
    
    static void setGFileMonitor(FileMonitor * fileMonitor, GFileMonitor * gFileMonitor)
    {
        fileMonitor->setObjectPtr(gFileMonitor);
    }
        
    static GFileMonitor * getGFileMonitor(const FileMonitor * fileMonitor)
    {
        return (GFileMonitor*)fileMonitor->getObjectPtr();
    }     
    
    /////
    
    static GError ** getGErrorPtr(Error * error) 
    {
        return (GError **)error->getErrorPtr();
    }
    
    ///// -------
    
    static GList * glistFromQStringList(const QList<QString> & qStrList)
    {
        GList * gList = NULL;
        
        for (int i = 0; i < qStrList.size(); ++i) {
             QString qstr = qStrList.at(i);
             char * cstr = g_strdup(qstr.toUtf8().data());
             gList = g_list_append(gList, cstr);
        }
        return gList;
    }    
    
    static GList * glistFromQByteArrayList(const QList<QByteArray> & qStrList)
    {
        GList * gList = NULL;
        
        for (int i = 0; i < qStrList.size(); ++i) {
             QByteArray qstr = qStrList.at(i);
             char * cstr = g_strdup(qstr.data());
             gList = g_list_append(gList, cstr);
        }
        return gList;
    }     

    static GList * gFileListFromFileList(const QList<File> & qFileList)
    {
        GList * gList = NULL;
        
        for (int i = 0; i < qFileList.size(); ++i) {
             File file = qFileList.at(i);
             GFile * gFile = WO::getGFile(&file);
             g_object_ref(gFile);
             gList = g_list_append(gList, gFile);
        }
        return gList;
    }     
    
    static QString qStringFromUtf8Freed(char * cstr)
    {
        if (!cstr) return QString();
        QString result = QString::fromUtf8(cstr);
        g_free(cstr);
        return result;
    }
    
    static QByteArray qByteArrayFromCStrFreed(char * cstr)
    {
        if (!cstr) return QByteArray();
        QByteArray result = QByteArray(cstr);
        g_free(cstr);
        return result;
    }    
    
    static QList<QByteArray> qByteArrayListFromCharVector(char ** cstrArr, bool free)
    {
        QList<QByteArray> result;
        if (!cstrArr) return result;
        
        int cnt=0;
        while(cstrArr[cnt])
        {
            result.append(QByteArray(cstrArr[cnt++]));
        }
        if (free)
        {
            g_strfreev(cstrArr);
        }
        
        return result;
    }
    
    
    static void initGIO()
    {
        g_type_init();
    }
    
    
};

}}

#endif /*KGIOWRAPPEDOBJECTUTILS_P_H_*/
