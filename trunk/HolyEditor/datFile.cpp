

///Code repris de http://www.sfml-dev.org/wiki/fr/tutoriels/formatdat
///Sources : http://www.gamedev.net/reference/programming/features/pak/



#include "globale.h"

#include <stdio.h>
#include <string.h>



cDAT::cDAT (void)
{
    m_buffer = NULL;
}

cDAT::~cDAT (void)
{
    if (m_buffer!=NULL)
        delete (m_buffer);
    m_entries.clear();
}

bool cDAT::Create (std::vector<std::string> files, std::string destination)
{
    //An file entry in order to push it in the object's std::vector
    sFileEntry entry;
    //An input file stream to read each file included
    std::ifstream file;
    //An output file stream to write our DAT file
    std::ofstream datfile;
    //The buffer used to read/write the DAT file
    char buffer[1];

    //DATHeader
    //We start by filling it with 0
    memset (&m_header, 0, sizeof(m_header));
    //Then we copy the ID
    memcpy (m_header.uniqueID, "JGDAT", 5);
    //Then the version
    memcpy (m_header.version, "0.1", 3);
    //Then the number of files to include
    m_header.nb_files = files.size();

    //Next, we open each file in orderto create the File Entries Table
    for (unsigned int i = 0; i<files.size(); i++)
    {
        file.open (files[i].c_str(), std::ifstream::in | std::ifstream::binary);
        if (file.is_open())
        {
            //Filling the FileEntry with 0
            memset (&entry, 0, sizeof(sFileEntry) );
            //We keep the file name
            memcpy (entry.name, files[i].c_str(), strlen ( files[i].c_str() ) );
            //We calculate its size
            file.seekg (0, std::ios::end);
            entry.size = file.tellg();
            //Since we don't know exactly its final position in the DAT file, let's use 0
            entry.offset = 0;
            //We finished with this file
            file.close();

            //Finally, we add this File Entry in our std::vector
            m_entries.push_back(entry);
        }
        else
        {
            //Simple error track
            std::cout<<"File "<<files[i]<<" raise an error."<<std::endl;
            return (false);
        }
    }

    //Now, we know everything about our files, we can update offsets
    long actual_offset = 0;
    actual_offset += sizeof(sDATHeader);
    actual_offset += m_header.nb_files * sizeof(sFileEntry);
    for (unsigned int i=0;i<m_entries.size();i++)
    {
        m_entries[i].offset = actual_offset;
        actual_offset += m_entries[i].size;
    }

    //And finally, we are writing the DAT file
    datfile.open (destination.c_str(), std::ofstream::out | std::ofstream::binary);

    //First, we write the header
    datfile.write ((char*)&m_header, sizeof(sDATHeader) );

    //Then, the File Entries Table
    for (unsigned int i=0;i<m_entries.size();i++)
    {
        datfile.write ((char*)&m_entries[i], sizeof(sFileEntry) );
    }

    //Finally, we write each file
    for (unsigned int i = 0; i<m_entries.size(); i++)
    {
        file.open (m_entries[i].name, std::ifstream::in | std::ifstream::binary);
        if (file.is_open())
        {
            file.seekg (0, std::ios::beg);
            while (file.read (buffer, 1))
            {
                datfile.write (buffer, 1);
            }
            file.close();
        }
        file.clear();
    }
    //And it's finished
    datfile.close();
    return (true);
}

int cDAT::GetNumberFile()
{
    return m_header.nb_files;
}

std::string cDAT::GetFileName(int ID)
{
    if (ID>=0&&ID<(int)m_header.nb_files)
        return m_entries[ID].name;
    return "";
}


void cDAT::ExportFile(int ID)
{
    if (ID>=0&&ID<(int)m_header.nb_files)
    {
        std::ifstream datfile;
        std::ofstream fichier(m_entries[ID].name, std::ios::out | std::ios::trunc);
        if (fichier)
        {
            //We are allocating memory to the buffer
            m_buffer = new char[(m_entries[ID].size)];
            //Simple error catch
            //if (m_buffer==NULL)
            //  return (NULL);
            //Opening the DAT file ot read the file datas needed
            datfile.open (m_datfile.c_str(), std::ifstream::in | std::ifstream::binary);
            if (datfile.is_open())
            {
                //Going to the right position
                datfile.seekg (m_entries[ID].offset, std::ios::beg);
                //  for(int o=0;o<m_entries[i].size;o++)
                //   fichier<<
                //Reading
                datfile.read (m_buffer, m_entries[ID].size);
                fichier<<m_buffer;
                //We can close the DAT file
                datfile.close();
            }

            fichier.close();
        }
    }
}

bool cDAT::Read (std::string source)
{
    bool ok=false;
    //The input file stream from which we want informations
    std::ifstream datfile;
    //A file entry in order to push it in the object's std::vector
    sFileEntry entry;

    //Filling the header with 0
    memset (&m_header, 0, sizeof(m_header));
    //We open the DAT file to read it
    datfile.open (source.c_str(), std::ifstream::in | std::ifstream::binary);
    if (datfile.is_open())
    {
        ok=true;
        //Getting to the Header position
        datfile.seekg (0, std::ios::beg);
        //Reading the DAT Header
        datfile.read ((char*)&m_header, sizeof(sDATHeader));
        //Next we are reading each file entry
        for (unsigned int i=0;i<m_header.nb_files;i++)
        {
            //Reading a File Entry
            datfile.read ((char*)&entry, sizeof(sFileEntry));
            //Pushing it in our std::vector
            m_entries.push_back(entry);
        }
        //Since all seems ok, we keep the DAT file name
        m_datfile = source;
    }
    else
        console->Ajouter("Impossible de lire le fichier : " + source);
    //Closing the DAT file
    datfile.close();

    return ok;
}

bool cDAT::IsFileExist(std::string filename)
{
    for (unsigned int i=0; i<m_header.nb_files;i++)
        if (filename == m_entries[i].name)
            return true;
    return false;
}

std::ifstream* cDAT::GetInfos(std::string filename)
{
    bool ok=false;
    //The input file stream from which we want information
    std::ifstream *datfile;
    datfile = new std::ifstream;

    //Cleaning properly an ancient file loaded
    if (m_buffer != NULL)
    {
        delete[] (m_buffer);
        m_buffer = NULL;
    }

    //First, we have to find the file needed
    for (unsigned int i=0; i<m_header.nb_files;i++)
    {
        //If we found it
        //std::string temp="infos.txt";
        if (filename == m_entries[i].name)
        {
            ok=true;

            //We are allocating memory to the buffer
            // m_buffer = new char[(m_entries[i].size)];
            //Simple error catch
            //if (m_buffer==NULL)
            //return (NULL);
            //Opening the DAT file ot read the file datas needed
            datfile->open (m_datfile.c_str(), std::ifstream::in | std::ifstream::binary);
            if (datfile->is_open())
            {
                //Going to the right position
                datfile->seekg (m_entries[i].offset, std::ios::beg);
            }
        }
    }

    if (!ok)
        console->Ajouter("Fichier introuvable : "+filename);
    //Finally, there is no such file in our DAT file
    return datfile;
}


char* cDAT::GetFile (std::string filename)
{
    //The input file stream from which we want information
    std::ifstream datfile;

    //Cleaning properly an ancient file loaded
    if (m_buffer != NULL)
    {
        delete[] (m_buffer);
        m_buffer = NULL;
    }

    //First, we have to find the file needed
    for (unsigned int i=0; i<m_header.nb_files;i++)
    {
        std::string temp=m_entries[i].name;
        std::string temp2= filename;
        //If we found it
        if (m_entries[i].name == filename)
        {
            //We are allocating memory to the buffer
            m_buffer = new char[(m_entries[i].size)];
            //Simple error catch
            if (m_buffer==NULL)
                return (NULL);
            //Opening the DAT file ot read the file datas needed
            datfile.open (m_datfile.c_str(), std::ifstream::in | std::ifstream::binary);
            if (datfile.is_open())
            {
                //Going to the right position
                datfile.seekg (m_entries[i].offset, std::ios::beg);
                //Reading
                datfile.read (m_buffer, m_entries[i].size);
                //We can close the DAT file
                datfile.close();
                //Returning the buffer
                return (m_buffer);
            }
        }
    }
    console->Ajouter("Fichier introuvable : "+filename,1);
    //Finally, there is no such file in our DAT file
    return (NULL);
}

long int cDAT::GetFileSize (std::string filename)
{
    //First, we have to find the file needed
    for (unsigned int i=0; i<m_header.nb_files;i++)
    {
        //If we found it
        if (m_entries[i].name == filename)
        {

            //Returning the size of the file found
            return (m_entries[i].size);
        }
    }
    return (0);
}
