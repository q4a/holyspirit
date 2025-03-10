
///Code repris de http://www.sfml-dev.org/wiki/fr/tutoriels/formatdat
///Sources : http://www.gamedev.net/reference/programming/features/pak/

#include <iostream>
#include <fstream>
#include <vector>

#ifndef DATFILEH
#define DATFILEH

struct sDATHeader
{
    char uniqueID[5]; /// Unique ID used to know if this file is a DAT File from this class
    char version[3]; /// Version of the DAT file format
    unsigned int nb_files; /// Number of files in the DAT file
};

struct sFileEntry
{
    char name[255]; /// Name of the data file
    long size; /// Size of the data file
    long offset; /// Offset, in the DAT file where the file is
};


class cDAT
{
    private :
        std::string m_datfile; /// name of the DAT file
        sDATHeader m_header; /// file header
        std::vector <sFileEntry> m_entries; /// vector of files entries
        char* m_buffer; /// Buffer pointing on a file in memory
    public :
        cDAT (void);
        ~cDAT (void);
        bool Create (std::vector<std::string> files, std::string destination);
        bool Read (std::string source);
        char* GetFile (std::string filename);
        std::ifstream* GetInfos(std::string filename);
        bool IsFileExist(std::string filename);
        long int GetFileSize (std::string filename);

        int GetNumberFile();
        std::string GetFileName(int ID);
        void ExportFile(int ID);

};


#endif
