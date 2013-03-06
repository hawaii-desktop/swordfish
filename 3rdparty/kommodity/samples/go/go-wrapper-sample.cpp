
#include <glib.h>
#include <glib/gprintf.h>
#include <gio/gio.h>


template <typename T>
class GO
{
public:
    GO() : p(0) {}

    GO(T * gObject)
    {
        p = gObject;
    }

    ~GO()
    {
        if (p != NULL)
        {
            g_object_unref(p);
        }
    }

    GO(const GO &other) : p(other.p)
    {
        if (p != NULL)
        {
            g_object_ref(p);
        }
    }

    GO &operator=(const GO &other)
    {
        if (this==&other)
            return *this;
        
        if (p != NULL)
        {
            g_object_unref(p);
        }
        
        p = other.p;
        if (p != NULL)
        {
            g_object_ref(p);
        }
        return *this;
    }

    bool isNull()
    {
        return p == NULL;
    }

    T * p;
};


int main(int argc, char *argv[])
{
    g_type_init();
    
    GError * err = NULL;
    GO<GFile> gFile = g_file_new_for_uri("ftp://user@server/file");
    GO<GFile> gFile2;
    
    gFile2 = gFile;  // test refcounting
    gFile = gFile2;  
    
    g_printf("gobject type %s\n", G_OBJECT_TYPE_NAME(gFile2.p));
    
    char * basename = g_file_get_basename(gFile2.p);
    
    g_printf("basename: %s\n", basename);
    g_free(basename);
    
    GO<GFileInfo> gFileInfo = g_file_query_info( gFile.p,
                   "*",
                   G_FILE_QUERY_INFO_NOFOLLOW_SYMLINKS,
                   NULL,
                   &err);

    if (err)
    {
         g_printf("error %s\n", err->message);
         g_error_free(err);

         return 1;
    } 
    
    return 0;
}



