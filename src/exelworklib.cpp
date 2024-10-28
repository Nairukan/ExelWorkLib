#include "exelworklib.h"

//ExelWorkLib::ExelWorkLib()
//{
//}

ExelFile* ExelFile::read_CSVs(std::vector<std::string>& filepaths){
    ExelFile* file = new ExelFile;
    for (auto filepath : filepaths){
        if (std::filesystem::is_regular_file(filepath)){
            std::string sheetName=filepath.substr(filepath.find_last_of('/'));
            sheetName=sheetName.substr(0, sheetName.find_last_of('.'));
            if (sheetName.length() && sheetName[0]=='/') sheetName=sheetName.substr(1);
            (*file)[sheetName];
            std::ifstream fileIn(filepath);
            //std::cout << sheetName << "\n";
            std::string buffer;
            for(unsigned int y=1; !fileIn.eof(); y++){
                getline(fileIn, buffer);
                if (buffer=="") continue;
                //std::cout << "\n";
                auto line=ExelFile::splitBySeparators(buffer, {";"});
                //std::cout << "endSep\n";
                for (unsigned int x=1; x<=line.size(); x++){
                    //std::cout << "<" << line[x-1] << "> " ;
                    if (line[x-1]!=""){
                        //if (std::regex_search(line[x-1], std::smatch, "&"))
                        line[x-1]=std::regex_replace(line[x-1], std::regex("&quot."), "&quot;");
                        line[x-1]=std::regex_replace(line[x-1], std::regex("&amp."), "&amp;");
                        line[x-1]=std::regex_replace(line[x-1], std::regex("&lt."), "&lt;");
                        line[x-1]=std::regex_replace(line[x-1], std::regex("&gt."), "&gt;");
                        line[x-1]=std::regex_replace(line[x-1], std::regex(">"), "&gt;");
                        line[x-1]=std::regex_replace(line[x-1], std::regex("<"), "&lt;");
                        *(*file)[sheetName][{x,y}]=line[x-1];
                    }
                }
                //std::cout << (*file)[sheetName].GridSize.first << "x" << (*file)[sheetName].GridSize.second  << "\n";
                //std::cout << "\nend\n\n";

            }
        }
    }
    return file;
}
